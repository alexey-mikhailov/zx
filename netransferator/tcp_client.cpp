# include "tcp_client.h"
# include "zx_socket.h"
# include "netranexception.h"

namespace zx
{
	tcp_client::tcp_client() :
		server_disconnected(_server_disconnected),
		error(_error),
		received(_received),
		_buffer_size(1024)
	{
	}

	tcp_client::~tcp_client()
	{
		delete _sender;
		delete _receiver;
	}

	void tcp_client::setup(int min_local_point,
						   int max_local_point)
	{
		_min_local_port = min_local_point;
		_max_local_port = max_local_point;

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			throw std::logic_error("Couldn't start Win Socket. ");
		}
	}

	void tcp_client::connect(const std::string& localhost,
							 const std::string& remotehost,
							 int rcv_port,
							 int snd_port)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_sender = nullptr;
		_receiver = nullptr;

		if (_min_local_port == 0 && _max_local_port == 0)
		{
			throw std::logic_error("Min or max local port are not set. Please invoke Setup before Connect. ");
		}

		auto rcv_endpoint = endpoint(remotehost, rcv_port);
		auto snd_endpoint = endpoint(remotehost, snd_port);

		bind_and_connect(
			&_sender,
			localhost,
			_min_local_port,
			rcv_endpoint);

		bind_and_connect(
			&_receiver,
			localhost,
			_sender->get_local()->port,
			snd_endpoint);

		_is_connected = true;
		std::thread t([&]() { receive_loop(); });
		t.detach();
	}

	bool tcp_client::send(const datarange& request, strong_payload& response)
	{
		if (request.size < 1)
		{
			return false;
		}

		if (send_request(request.size_ptr(), sizeof(int)))
		{
			if (send_request(request.data, request.size))
			{
				int response_size;
				if (receive_response(reinterpret_cast<byte*>(&response_size), sizeof(int)))
				{
					if (response_size > 0)
					{
						response.resize(response_size);
						if (receive_response(response.data(), response_size))
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	void tcp_client::disconnect()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		try
		{
			_receiver->shutdown_close();
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAENOTSOCK)
			{
				// It seems socket was already closed
				// Swallow exception
			}
			else
			{
				throw;
			}
		}

		try
		{
			_sender->shutdown_close();
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAENOTSOCK)
			{
				// It seems socket was already closed
				// Swallow exception
			}
			else
			{
				throw;
			}
		}

		_is_connected = false;
	}

	bool tcp_client::send_request(const byte* request,
								  int requestSize)
	{
		try
		{
			const auto sent = _sender->send(request, requestSize);

			if (_sender->is_disconnected_after_send(requestSize, sent))
			{
				_server_disconnected(socket_closure);
				disconnect();
				return false;
			}

			return true;
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAECONNRESET)
			{
				if (_is_connected)
				{
					_server_disconnected(termination);
				}

				disconnect();
			}
			else
			{
				_error(ex.error());
			}

			return false;
		}
	}

	bool tcp_client::send_response(const byte* response,
								   int responseSize)
	{
		try
		{
			auto const sent = _receiver->send(response, responseSize);

			if (_receiver->is_disconnected_after_send(responseSize, sent))
			{
				_server_disconnected(socket_closure);
				disconnect();
				return false;
			}

			return true;
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAECONNRESET)
			{
				if (_is_connected)
				{
					_server_disconnected(termination);
				}

				disconnect();
			}
			else
			{
				_error(ex.error());
			}

			return false;
		}
	}

	bool tcp_client::receive_request(byte* request,
									 int requestSize)
	{
		try
		{
			int total = 0;;
			auto is_disconnected = false;

			do
			{
				auto to_receive = _buffer_size;
				if (total + to_receive > requestSize)
				{
					to_receive = requestSize - total;
				}

				const auto received_ = _receiver->receive(request + total, to_receive);

				total += received_;
				is_disconnected = _receiver->is_disconntected_after_receive(to_receive, received_);
			} 
			while (!is_disconnected && total < requestSize);

			if (is_disconnected)
			{
				_server_disconnected(socket_closure);
				disconnect();
				return false;
			}

			return true;
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAECONNRESET)
			{
				if (_is_connected)
				{
					_server_disconnected(termination);
				}

				disconnect();
			}
			else
			{
				_error(ex.error());
			}

			return false;
		}
	}

	bool tcp_client::receive_response(byte* response, 
									  int response_size)
	{
		try
		{
			int total = 0;;
			auto is_disconnected = false;

			do
			{
				auto to_receive = _buffer_size;
				if (total + to_receive > response_size)
				{
					to_receive = response_size - total;
				}

				const auto received_ = _sender->receive(response + total, to_receive);

				total += received_;
				is_disconnected = _sender->is_disconntected_after_receive(to_receive, received_);
			} 
			while (!is_disconnected && total < response_size);

			if (is_disconnected)
			{
				_server_disconnected(termination);
				disconnect();
				return false;
			}

			return true;
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAECONNRESET)
			{
				if (_is_connected)
				{
					_server_disconnected(termination);
				}

				disconnect();
			}
			else
			{
				_error(ex.error());
			}

			return false;
		}
	}

	void tcp_client::receive_loop()
	{
		bool response_sent;

		do
		{
			response_sent = false;

			int request_size;
			if (receive_request(reinterpret_cast<byte*>(&request_size), sizeof(int)))
			{
				const auto request = strong_payload(request_size);
				if (receive_request(request.data(), request_size))
				{
					std::shared_ptr<strong_payload> response;
					if (_received)
					{
						response = _received(request);
					}

					if (send_response(response->size_ptr(), sizeof(int)))
					{
						if (response->size() > 0)
						{
							if (send_response(response->data(), response->size()))
							{
								response_sent = true;
							}
						}
						else
						{
							response_sent = true;
						}
					}
				}
			}
		} 
		while (response_sent);
	}

	void tcp_client::bind_and_connect(socket** socket,
									  const std::string& localhost,
									  unsigned short local_port,
									  endpoint& remote_endpoint)
	{
		if (local_port > _max_local_port)
		{
			local_port = _min_local_port;
		}

		if (local_port == _first_busy_local_port.second)
		{
			throw std::logic_error("Port range is busy. ");
		}

		try
		{
			delete (*socket);
			(*socket) = new zx::socket();
			(*socket)->bind(localhost, local_port);
			(*socket)->connect(remote_endpoint);
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAEADDRINUSE || ex.error() == WSAEINVAL)
			{
				if (!_first_busy_local_port.first)
				{
					_first_busy_local_port.second = local_port;
				}

				bind_and_connect(socket, localhost, local_port + 1, remote_endpoint);
			}
			else
			{
				throw;
			}
		}
	}
}
