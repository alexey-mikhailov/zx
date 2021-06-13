# include "netransferator.h"
# include "string_tools.h"

zx::tcp_client *_client = nullptr;

std::shared_ptr<zx::strong_payload> OnReceived(const zx::strong_payload &request);
void OnError(int error);
void OnServerDisconnected(zx::disconnect_reason reason);

int main()
{
	try 
	{
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		std::setlocale(LC_ALL, "ru_RU.UTF-8");

		_client = new zx::tcp_client();
		_client->setup(8900, 8910);
		_client->received += OnReceived;
		_client->error += OnError;
		_client->server_disconnected += OnServerDisconnected;
		_client->connect("localhost", "localhost", 8888, 8889);

		const auto localRcvHost = zx::wstring(_client->get_receiver()->get_local()->ip);
		const auto localRcvPort = _client->get_receiver()->get_local()->port;
		const auto localSndPort = _client->get_sender()->get_local()->port;
		
		std::wcout << L"Connected to server as " 
					<< localRcvHost << L":[" 
					<< localRcvPort << L", " 
					<< localSndPort << L"]. " << std::endl;
		std::wcout << L"Type message to send, " 
					<< L"[reconnect] to reconnect or " 
					<< L"enter to escape. " << std::endl << std::endl;

		while (true)
		{
			std::wstring cmd;
			std::getline(std::wcin, cmd);
			if (cmd.empty())
			{
				break;
			}

			if (cmd == L"reconnect")
			{
				_client->connect("localhost", "localhost", 8888, 8889);
			}
			else
			{
				if (_client->is_connected())
				{
					const auto request = zx::datarange(
						sizeof(wchar_t) * (int)wcslen(cmd.c_str()),
						(byte*)cmd.c_str());

					zx::strong_payload response;
					if (_client->send(request, response))
					{
						const auto responseString = zx::wstring(response.data(), response.size());
						std::wcout << L"Answer is: " << responseString << std::endl << std::endl;
					}
				}
				else
				{
					std::wcout << L"Client is not connected to server. " << std::endl;
				}
			}
		}

		_client->disconnect();
	}
	catch(zx::netranexception &ex)
	{
		const auto what = zx::wstring(ex.what());
		const auto stackTrace = zx::wstring(ex.stack_trace_str());
		std::wcout << what << std::endl;
		std::wcout << stackTrace << std::endl;
		getchar();
	}
	catch(std::exception &ex)
	{
		const auto what = zx::wstring(ex.what());
		std::wcout << what << std::endl;
		getchar();
	}

	delete _client;
}

std::shared_ptr<zx::strong_payload> OnReceived(const zx::strong_payload &response)
{
	const auto responseString = zx::wstring(response.data(), response.size());
	std::wcout << L"Received from server: " << std::endl;
	std::wcout << responseString << std::endl << std::endl;
	const auto answer = L"Client is OK";
	const auto answerSize = sizeof(wchar_t) * wcslen(answer);
	return zx::strong_payload::create_shared((int)answerSize, (void*)answer);
}

void OnError(int error)
{
	std::wcout << L"Error: " << error << std::endl;
}

void OnServerDisconnected(zx::disconnect_reason reason)
{
	std::wcout << L"Server disconnected. Reason is " << reason << std::endl;
}
