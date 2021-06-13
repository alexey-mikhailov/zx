# include "Server.h"
# include "string_tools.h"

Server::Server()
{
	_server = new zx::tcp_server();
	_server->client_connected +=
		zx::mem_fn(this, &Server::OnConnected);
	_server->client_disconnected +=
		zx::mem_fn(this, &Server::OnDisconnected);
	_server->received +=
		zx::mem_fn(this, &Server::OnReceived);
}

Server::~Server()
{
	_server->client_connected -= zx::mem_fn(this, &Server::OnConnected);
	_server->client_disconnected -= zx::mem_fn(this, &Server::OnDisconnected);
	delete _server;
}

std::shared_ptr<zx::strong_payload> Server::OnReceived(zx::socket *socket, const zx::strong_payload &request)
{
	const auto host = zx::wstring(socket->get_remote()->host);
	const auto port = socket->get_remote()->port;
	std::wcout << L"Received from " << host << ":" << port << ":" << std::endl;
	const auto requestString = zx::wstring(request.data(), request.size());
	std::wcout << requestString << std::endl << std::endl;
	const auto result = L"Server is OK";
	const auto resultSize = sizeof(wchar_t) * wcslen(result);
	return zx::strong_payload::create_shared((int)resultSize, (void*)result);
}

void Server::OnConnected(zx::socket *socket)
{
	const auto host = zx::wstring(socket->get_remote()->host);
	const auto port = socket->get_remote()->port;
	std::wcout << host << ":" << port << L" connected. " << std::endl << std::endl;
}

void Server::OnDisconnected(zx::socket *socket, zx::disconnect_reason reason)
{
	const auto host = zx::wstring(socket->get_remote()->host);
	const auto port = socket->get_remote()->port;
	std::wcout << host << L":" << port << L" disconnected. ";
	std::wcout << L"Reason is " << reason << std::endl << std::endl;
}

