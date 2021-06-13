# pragma once

namespace zx
{
	void get_addr_by_host(const std::string& host, sockaddr_in* result);

	sockaddr_in get_addr_by_host(const std::string& host);

	sockaddr_in get_addr_by_host_port(const std::string& host, unsigned short port);

	std::string get_host_by_addr(const sockaddr_in& address);

	void get_remote_addr_by_socket(SOCKET socket, sockaddr_in* result);

	void get_local_addr_by_socket(SOCKET socket, sockaddr_in* result);

	std::string get_ip_from_binary(const in_addr& ip);
}
