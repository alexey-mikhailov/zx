# pragma once
# include "netransferator_defines.h"
# include "netransferator_enums.h"
# include "zx_event.h"
# include "zx_payload.h"

namespace zx
{
	struct endpoint;
	class socket;

	class tcp_client
	{
	public:
		event<void(disconnect_reason)>   server_disconnected;
		event<void(int)>				error;
		event<std::shared_ptr<strong_payload>(const strong_payload&)> received;

		NETRANSFERATOR_API explicit tcp_client();
		NETRANSFERATOR_API virtual ~tcp_client();

		NETRANSFERATOR_API void setup(
			int minLocalPort,
			int maxLocalPort);

		NETRANSFERATOR_API void connect(
			const std::string& localhost,
			const std::string& remotehost,
			int rcvPort,
			int sndPort);

		NETRANSFERATOR_API bool send(const datarange& request, strong_payload& response);

		NETRANSFERATOR_API void disconnect();

		NETRANSFERATOR_API int get_buffer_size() const { return _buffer_size; }
		NETRANSFERATOR_API void set_buffer_size(int value) { _buffer_size = value; }

		NETRANSFERATOR_API socket* get_receiver() const { return _receiver; }
		NETRANSFERATOR_API socket* get_sender() const { return _sender; }

		NETRANSFERATOR_API bool is_connected() const { return _is_connected; }

	private:
		bool send_request(const byte* request, int requestSize);
		bool send_response(const byte* response, int responseSize);
		bool receive_request(byte* request, int requestSize);
		bool receive_response(byte* response, int responseSize);
		void receive_loop();
		void bind_and_connect(
			socket** socket,
			const std::string& localhost,
			unsigned short localPort,
			endpoint& remoteEndpoint);

		multicast_delegate<void(disconnect_reason)>  _server_disconnected;
		multicast_delegate<void(int)>				_error;
		multicast_delegate<std::shared_ptr<strong_payload>(const strong_payload&)> _received;
		std::mutex							_mutex;
		int									_min_local_port;
		int									_max_local_port;
		std::pair<bool, int>				_first_busy_local_port;

		socket* _receiver;
		socket* _sender;

		bool								_is_connected;
		int									_buffer_size;
	};
}
