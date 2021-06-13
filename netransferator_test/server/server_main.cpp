#include "Server.h"
#include "string_tools.h"


int main()
{
	Server server;

	try
	{
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		std::setlocale(LC_ALL, "ru_RU.UTF-8");

		server.GetImpl()->start("localhost", 8888, 8889, 10);

		const auto ip = zx::wstring(server.GetImpl()->get_receiver()->get_local()->ip);
		std::wcout << L"Server started at " 
					<< ip << L":["
					<< server.GetImpl()->get_receiver()->get_local()->port << L", "
					<< server.GetImpl()->get_sender()->get_local()->port << L"]. "
					<< std::endl;
		std::wcout << L"Type message to send, enter to escape. " << std::endl << std::endl;

		while (true)
		{
			std::wstring cmd;
			std::getline(std::wcin, cmd);

			if (cmd.empty())
			{
				break;
			}

			zx::socket *client = nullptr;
			const auto receivers = server.GetImpl()->get_receivers();
			auto pos = cmd.find(L" >>> ");
			if (pos != std::string::npos)
			{
				auto addr = cmd.substr(0, pos);
				cmd = cmd.substr(pos + 5, cmd.length() - 1);
				pos = addr.find(L":");
				const auto ipStr = addr.substr(0, pos);
				const auto portStr = addr.substr(pos + 1, addr.length() - 1);

# pragma warning (push)
# pragma warning (disable : 4244)
				const auto ipStrAnsi = std::string(ipStr.begin(), ipStr.end());
# pragma warning (pop)

				const unsigned short port = std::stoi(portStr);
				const auto inputedEp = zx::endpoint(ipStrAnsi, port);

				for (auto receiver : receivers)
				{
					if (*receiver->get_remote() == inputedEp)
					{
						client = receiver;
						break;
					}
				}
			}
			else
			{
				if (receivers.size())
				{
					client = *receivers.begin();
				}
			}

			if (client == nullptr)
			{
				std::wcout << L"There are no connected clients" << std::endl;
			}
			else
			{
				const auto request = zx::datarange(sizeof(wchar_t) * (int)wcslen(cmd.c_str()), (byte *)cmd.c_str());
				zx::strong_payload response;
				if (server.GetImpl()->send(client, request, response))
				{
					const auto answer = zx::wstring(response.data(), response.size());
					std::wcout << L"Answer is: " << answer << std::endl << std::endl;
				}
			}
		}

		server.GetImpl()->stop();
	}
	catch (zx::netranexception &ex)
	{
		const auto what = zx::wstring(ex.what());
		const auto stackTrace = zx::wstring(ex.stack_trace_str());
		std::wcout << what << std::endl;
		std::wcout << stackTrace << std::endl;
		getchar();
	}
	catch (std::exception &ex)
	{
		const auto what = zx::wstring(ex.what());
		std::wcout << what << std::endl;
		getchar();
	}
}

