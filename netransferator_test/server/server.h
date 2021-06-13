# pragma once
# include "netransferator.h"

class zx::tcp_server;

class Server
{
public:
	Server();
	~Server();

	zx::tcp_server *GetImpl() const { return _server; }

private:
	std::shared_ptr<zx::strong_payload> 
		OnReceived(zx::socket* socket,
				   const zx::strong_payload& request);

	void OnConnected(zx::socket *socket);
	void OnDisconnected(zx::socket *socket, zx::disconnect_reason reason);

	zx::tcp_server *			_server;
};
