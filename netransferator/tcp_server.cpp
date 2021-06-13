# include "tcp_server.h"
# include "zx_socket.h"
# include "netranexception.h"

namespace zx
{
	tcp_server::tcp_server() :
		_sender(nullptr),
		_receiver(nullptr),
		_is_started(false), 
		_buffer_size(1024),
		_last_accepted_sender(nullptr),
		client_connected(_client_connected),
		client_disconnected(_client_disconnected),
		received(_received)
	{
	}

	tcp_server::~tcp_server()
	{
		stop();
		delete _sender;
		delete _receiver;
	}
	
	void tcp_server::start(
		const std::string& localhost,
		int rcvPort,
		int sndPort,
		int connectionNumber)
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			throw std::logic_error("Couldn't start Win Socket. ");
		}

		// Receive socket
		_receiver = new socket();
		_receiver->bind(localhost, rcvPort);
		auto error = WSAGetLastError();

		// Send socket
		_sender = new socket();
		_sender->bind(localhost, sndPort);
		error = WSAGetLastError();

		// Listen sockets
		_receiver->listen(connectionNumber);
		_sender->listen(connectionNumber);
		error = WSAGetLastError();

		// Start accept threads
		_is_started = true;
		std::thread t1([&]() { accept_receivers_loop(); });
		std::thread t2([&]() { accept_senders_loop(); });
		t1.detach();
		t2.detach();
	}

	void tcp_server::stop()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto sender : _senders)
		{
			sender->shutdown_close();
		}

		for (auto receiver : _receivers)
		{
			receiver->shutdown_close();
		}

		_senders.clear();
		_receivers.clear();
		_sender_map.clear();
		_receiver_map.clear();

		try 
		{
			_sender->close();
		}
		catch (std::exception)
		{
			// swallow it
		}

		try
		{
			_receiver->close();
		}
		catch (std::exception)
		{
			// swallow it
		}

		_is_started = false;
	}

	socket* tcp_server::get_sender_by_receiver(socket* receiver) const
	{
		return _sender_map.at(receiver);
	}

	socket* tcp_server::get_receiver_by_sender(socket* sender) const
	{
		return _receiver_map.at(sender);
	}



	bool tcp_server::send(
		const std::string& host,
		int port,
		const datarange& request,
		strong_payload& response)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		const auto targetEndpoint = endpoint(host, port);
		socket* receiver = nullptr;
		for (auto current : _receivers)
		{
			const auto currentRemote = current->get_remote();
			if (*currentRemote == targetEndpoint)
			{
				receiver = current;
				break;
			}
		}

		return send(receiver, request, response);
	}

	bool tcp_server::send(
		socket* receiver,
		const datarange& request,
		strong_payload& response)
	{
		std::lock_guard <std::mutex> lock(_mutex);

		if (request.size < 1)
		{
			return false;
		}

		if (send_request(receiver, request.size_ptr(), sizeof(int)))
		{
			if (send_request(receiver, request.data, request.size))
			{
				int responseSize;
				if (receive_response(
					receiver,
					reinterpret_cast<byte*>(&responseSize),
					sizeof(int)))
				{
					if (responseSize > 0)
					{
						response.resize(responseSize);
						if (receive_response(receiver, response.data(), responseSize))
						{
							return true;
						}
					}
				}
			}
		}

		return false;
	}


	bool tcp_server::send_request(
		socket* receiver,
		const byte* request,
		int requestSize)
	{
		try
		{
			const auto sent = receiver->send(request, requestSize);

			if (receiver->is_disconnected_after_send(requestSize, sent))
			{
				_mutex.lock();
				if (std::find(_receivers.begin(), _receivers.end(), receiver) != _receivers.end())
				{
					_client_disconnected(receiver, socket_closure);
				}

				close_ticket(receiver);
				_mutex.unlock();
				return false;
			}

			return true;
		}
		catch (netranexception & ex)
		{
			_mutex.lock();
			if (ex.error() == WSAECONNRESET)
			{
				if (std::find(_receivers.begin(), _receivers.end(), receiver) != _receivers.end())
				{
					_client_disconnected(receiver, termination);
				}

				close_ticket(receiver);
			}

			_mutex.unlock();
			return false;
		}
	}

	bool tcp_server::send_response(
		socket* sender,
		const byte* response,
		int responseSize)
	{
		try
		{
			const auto sent = sender->send(response, responseSize);

			if (sender->is_disconnected_after_send(responseSize, sent))
			{
				_mutex.lock();
				if (std::find(_senders.begin(), _senders.end(), sender) != _senders.end())
				{
					_client_disconnected(sender, socket_closure);
				}

				close_ticket(sender);
				_mutex.unlock();
				return false;
			}

			return true;
		}
		catch (netranexception & ex)
		{
			if (ex.error() == WSAECONNRESET)
			{
				_mutex.lock();
				if (std::find(_senders.begin(), _senders.end(), sender) != _senders.end())
				{
					_client_disconnected(sender, termination);
				}

				close_ticket(sender);
				_mutex.unlock();
			}

			return false;
		}
	}

	bool tcp_server::receive_request(
		socket* sender,
		byte* request,
		int request_size)
	{
		try
		{
			int total = 0;;
			auto isDisconnected = false;

			do
			{
				auto toReceive = _buffer_size;
				if (total + toReceive > request_size)
				{
					toReceive = request_size - total;
				}

				const auto received_ = sender->receive(request + total, toReceive);

				total += received_;
				isDisconnected = sender->is_disconntected_after_receive(toReceive, received_);
			} while (!isDisconnected && total < request_size);

			if (isDisconnected)
			{
				_mutex.lock();
				if (std::find(_senders.begin(), _senders.end(), sender) != _senders.end())
				{
					_client_disconnected(sender, socket_closure);
				}

				close_ticket(sender);
				_mutex.unlock();
				return false;
			}

			return true;
		}
		catch (netranexception& ex)
		{
			_mutex.lock();
			if (ex.error() == WSAECONNRESET)
			{
				if (std::find(_senders.begin(), _senders.end(), sender) != _senders.end())
				{
					_client_disconnected(sender, termination);
				}

				close_ticket(sender);
			}

			_mutex.unlock();
			return false;
		}
	}

	bool tcp_server::receive_response(socket* receiver, 
									  byte* response, 
									  int response_size)
	{
		try
		{
			int total = 0;;
			auto to_disconnected = false;

			do
			{
				auto to_receive = _buffer_size;
				if (total + to_receive > response_size)
				{
					to_receive = response_size - total;
				}

				const auto received_ = receiver->receive(response + total, to_receive);

				total += received_;
				to_disconnected = receiver->is_disconntected_after_receive(to_receive, received_);
			} 
			while (!to_disconnected && total < response_size);

			if (to_disconnected)
			{
				_mutex.lock();
				if (std::find(_receivers.begin(), _receivers.end(), receiver) != _receivers.end())
				{
					_client_disconnected(receiver, socket_closure);
				}

				close_ticket(receiver);
				_mutex.unlock();
				return false;
			}

			return true;
		}
		catch (netranexception & ex)
		{
			_mutex.lock();
			if (ex.error() == WSAECONNRESET)
			{
				if (std::find(_receivers.begin(), _receivers.end(), receiver) != _receivers.end())
				{
					_client_disconnected(receiver, termination);
				}

				close_ticket(receiver);
			}

			_mutex.unlock();
			return false;
		}
	}


	void tcp_server::accept_senders_loop()
	{
		while (_is_started)
		{
			try
			{
				auto client = _receiver->accept();

				_last_accepted_sender = client;

				_mutex.lock();
				_senders.push_back(client);
				_mutex.unlock();

				// Do not fire ClientConnected event here because it is too early

				std::thread t([&]() { receive_loop(client); });
				t.detach();
			}
			catch (netranexception & ex)
			{
				// Interrupted call means socket was shutted down
				if (ex.error() == WSAEINTR)
				{
					stop();
					break;
				}
			}
		}
	}

	void tcp_server::accept_receivers_loop()
	{
		while (_is_started)
		{
			try
			{
				auto client = _sender->accept();
				_mutex.lock();
				_receivers.push_back(client);
				_receiver_map[_last_accepted_sender] = client;
				_sender_map[client] = _last_accepted_sender;
				_mutex.unlock();

				_client_connected(client);
			}
			catch (netranexception & ex)
			{
				// Interrupted call means socket was shutted down
				if (ex.error() == WSAEINTR)
				{
					stop();
					break;
				}
			}
		}
	}

	void tcp_server::receive_loop(socket* sender)
	{
		bool response_sent;

		do
		{
			response_sent = false;

			int request_size;
			if (receive_request(sender, reinterpret_cast<byte*>(&request_size), sizeof(int)))
			{
				const auto request = strong_payload(request_size);
				if (receive_request(sender, request.data(), request_size))
				{
					std::shared_ptr<strong_payload> response;
					if (_received)
					{
						response = _received(sender, request);
					}

					if (send_response(sender, response->size_ptr(), sizeof(int)))
					{
						if (response->size() > 0)
						{
							if (send_response(sender, response->data(), response->size()))
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

	void tcp_server::close_ticket(socket* client)
	{
		socket* sender = nullptr;
		socket* receiver = nullptr;

		const auto sender_map_it = _sender_map.find(client);
		const auto receiver_map_it = _receiver_map.find(client);
		if (sender_map_it != _sender_map.end())
		{
			// sender case
			receiver = sender_map_it->first;
			sender = sender_map_it->second;
		}
		else
		{
			// receiver case
			sender = receiver_map_it->first;
			receiver = receiver_map_it->second;
		}

		auto sender_it = std::find(_senders.begin(), _senders.end(), sender);
		if (sender_it != _senders.end())
		{
			_senders.erase(sender_it);
		}

		auto receiver_it = std::find(_receivers.begin(), _receivers.end(), receiver);
		if (receiver_it != _receivers.end())
		{
			_receivers.erase(receiver_it);
		}

		_receiver_map.erase(sender);
		_sender_map.erase(receiver);

		if (sender)
		{
			sender->shutdown_close();
		}

		if (receiver)
		{
			receiver->shutdown_close();
		}
	}
}


