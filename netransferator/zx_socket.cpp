# include "zx_socket.h"
# include "netranexception.h"
# include "net_tools.h"

namespace zx
{
	endpoint::endpoint(std::string host_, const unsigned short port_, const ::sockaddr_in& address_):
		host(std::move(host_)),
		port(port_),
		address(address_),
		ip(get_ip_from_binary(address.sin_addr))
	{
	}

	endpoint::endpoint(std::string host_, const unsigned short port_):
		host(std::move(host_)),
		port(port_),
		address(get_addr_by_host_port(host, port)),
		ip(get_ip_from_binary(address.sin_addr))
	{
	}

	bool operator==(const endpoint& left, const endpoint& right)
	{
		return
			left.address.sin_addr.s_addr == right.address.sin_addr.s_addr &&
			left.address.sin_port == right.address.sin_port;
	}

	socket::socket(bool reuseAddress) :
		_socket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)),
		_remote(nullptr),
		_local(nullptr)
	{
		if (setsockopt(
			_socket,
			SOL_SOCKET,
			SO_REUSEADDR,
			reinterpret_cast<const char*>(&reuseAddress),
			sizeof(bool)))
		{
			const auto error = WSAGetLastError();
			throw netranexception("Socket reuse address setting failure with ", error);
		}
	}

	socket::socket(SOCKET socket) :
		_socket(socket),
		_remote(nullptr),
		_local(nullptr)
	{
	}

	socket::~socket()
	{
		if (_isConnected)
		{
			try
			{
				shutdown();
				close();
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
					// Please handle another cases
				}
			}
		}

		delete _local;
		delete _remote;
	}

	int socket::bind(const endpoint& ep)
	{
		delete _local;
		_local = new endpoint(ep);
		const auto result = ::bind(
			_socket,
			reinterpret_cast<const sockaddr*>(&_local->address),
			sizeof(sockaddr_in));

		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Bind failure with error ", error);
		}

		return result;
	}

	int socket::bind(const std::string& host, unsigned short port)
	{
		delete _local;
		_local = new endpoint(host, port);
		const auto result = ::bind(
			_socket,
			reinterpret_cast<const sockaddr*>(&_local->address),
			sizeof(sockaddr_in));

		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Bind failure with error ", error);
		}

		return result;
	}

	int socket::connect(const endpoint& ep)
	{
		delete _remote;
		_remote = new endpoint(ep);
		const auto result = ::connect(
			_socket,
			reinterpret_cast<const sockaddr*>(&_remote->address),
			sizeof(sockaddr_in));

		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Couldn't connect to remote host: ", error);
		}
		else
		{
			_isConnected = true;
		}

		return result;
	}

	int socket::connect(const std::string& host, unsigned short port)
	{
		set_remote(host, port);
		const auto result = ::connect(
			_socket,
			reinterpret_cast<const sockaddr*>(&_remote->address),
			sizeof(sockaddr_in));

		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Connect failure with error ", error);
		}
		else
		{
			_isConnected = true;
		}

		return result;
	}

	int socket::listen(int connectionNumber) const
	{
		const auto result = ::listen(_socket, connectionNumber);
		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Listen failure with error ", error);
		}

		return result;
	}

	socket* socket::accept()
	{
		sockaddr_in address{};
		int addressSize = sizeof(address);

		const auto remoteSocket = ::accept(
			_socket,
			reinterpret_cast<sockaddr*>(&address),
			&addressSize);

		if (int(remoteSocket) == SOCKET_ERROR)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Accept failure with error ", error);
		}
		else
		{
			_isConnected = true;
		}

		auto newRemote = new socket(remoteSocket);
		newRemote->set_remote(address);
		newRemote->_local = new endpoint(*_local);;
		return newRemote;
	}

	int socket::send(const byte* data, int size) const
	{
		const auto result = ::send(_socket, reinterpret_cast<const char*>(data), size, NULL);
		if (result == SOCKET_ERROR)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Send failure with error ", error);
		}

		return result;
	}

	int socket::receive(byte* data, int size) const
	{
		const auto result = recv(_socket, reinterpret_cast<char*>(data), size, NULL);
		if (result == SOCKET_ERROR)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Receive failure with error ", error);
		}

		return result;
	}

	int socket::shutdown(int how) const
	{
		const auto result = ::shutdown(_socket, how);
		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Shutdown failure with error ", error);
		}

		return result;
	}

	int socket::close()
	{
		const auto result = closesocket(_socket);
		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Close failure with error ", error);
		}

		_isConnected = false;
		return result;
	}

	void socket::shutdown_close(int how)
	{
		auto result = ::shutdown(_socket, how);
		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Shutdown failure with error ", error);
		}

		result = closesocket(_socket);
		if (result)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Close failure with error ", error);
		}

		_isConnected = false;
	}

	bool socket::is_disconnected_after_send(int bytesSentExpected, int bytesSentActually) const
	{
		fd_set toWrite;
		FD_ZERO(&toWrite);
		FD_SET(_socket, &toWrite);
		const auto result = select(NULL, nullptr, &toWrite, nullptr, nullptr);
		if (result == SOCKET_ERROR)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Select failure with error ", error);
		}

		return FD_ISSET(_socket, &toWrite) && bytesSentExpected != bytesSentActually;
	}

	bool socket::is_disconntected_after_receive(int bytesReadExpected, int bytesReadActually) const
	{
		fd_set toRead;
		timeval timeout{};
		timeout.tv_usec = 10;

		FD_ZERO(&toRead);
		FD_SET(_socket, &toRead);
		const auto result = select(NULL, &toRead, nullptr, nullptr, &timeout);
		if (result == SOCKET_ERROR)
		{
			const auto error = WSAGetLastError();
			throw netranexception("Select failure with error ", error);
		}

		return FD_ISSET(_socket, &toRead) && bytesReadExpected != bytesReadActually;
	}

	void socket::set_local(const std::string& host, unsigned short port)
	{
		sockaddr_in address{};
		get_addr_by_host(host, &address);
		address.sin_port = htons(port);
		delete _local;
		_local = new endpoint(host, port, address);
	}

	void socket::set_remote(const std::string& host, unsigned short port)
	{
		sockaddr_in address{};
		get_addr_by_host(host, &address);
		address.sin_port = htons(port);
		delete _remote;
		_remote = new endpoint(host, port, address);
	}

	void socket::set_remote(const sockaddr_in& address)
	{
		const auto host = get_host_by_addr(address);
		const unsigned short port = ntohs(address.sin_port);
		delete _remote;
		_remote = new endpoint(host, port, address);
	}
}