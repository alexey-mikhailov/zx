# include "net_tools.h"

namespace zx
{
	void get_addr_by_host(const std::string& host, sockaddr_in* result)
	{
		addrinfo hints{};
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		addrinfo* addrinfo_;
		if (getaddrinfo(host.c_str(), nullptr, &hints, &addrinfo_))
		{
			auto error = WSAGetLastError();
			throw;
		}

		*result = *reinterpret_cast<sockaddr_in*>(addrinfo_->ai_addr);
		freeaddrinfo(addrinfo_);
	}

	sockaddr_in get_addr_by_host(const std::string& host)
	{
		sockaddr_in address{};
		get_addr_by_host(host, &address);
		return address;
	}

	sockaddr_in get_addr_by_host_port(const std::string& host, unsigned short port)
	{
		sockaddr_in address{};
		get_addr_by_host(host, &address);
		address.sin_port = htons(port);
		return address;
	}

	std::string get_host_by_addr(const sockaddr_in& address)
	{
		char hostname[NI_MAXHOST];
		char servInfo[NI_MAXSERV];

		if (getnameinfo(
			(sockaddr*)& address,
			sizeof(sockaddr),
			hostname,
			NI_MAXHOST,
			servInfo,
			NI_MAXSERV,
			NI_NUMERICSERV))
		{
			auto error = WSAGetLastError();
			throw;
		}

		return hostname;
	}

	void get_remote_addr_by_socket(SOCKET socket, sockaddr_in* result)
	{
		auto addressSize = static_cast<int>(sizeof(sockaddr_in));
		if (getpeername(socket, reinterpret_cast<sockaddr*>(result), &addressSize))
		{
			auto error = WSAGetLastError();
			throw;
		}
	}

	void get_local_addr_by_socket(SOCKET socket, sockaddr_in* result)
	{
		auto addressSize = static_cast<int>(sizeof(sockaddr_in));
		if (getsockname(socket, reinterpret_cast<sockaddr*>(result), &addressSize))
		{
			auto error = WSAGetLastError();
			throw;
		}
	}

	std::string get_ip_from_binary(const in_addr& ip)
	{
		char result[15];
		sprintf_s(
			result,
			15,
			"%d.%d.%d.%d",
			ip.S_un.S_un_b.s_b1,
			ip.S_un.S_un_b.s_b2,
			ip.S_un.S_un_b.s_b3,
			ip.S_un.S_un_b.s_b4);

		return result;
	}
}
