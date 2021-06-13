# pragma once
# include "netransferator_defines.h"
# include "netransferator_enums.h"
# include "zx_event.h"
# include "zx_payload.h"

namespace zx
{
	struct endpoint;
	class socket;

	class tcp_server final
	{
		multicast_delegate<void(socket*)>	_client_connected;
		multicast_delegate<void(socket*, disconnect_reason)> _client_disconnected;
		multicast_delegate<std::shared_ptr<strong_payload>(socket*, const strong_payload&)> _received;
		std::mutex							_mutex;

		socket*								_sender;
		socket*								_receiver;

		bool								_is_started;
		std::vector<socket*>				_receivers;
		std::vector<socket*>				_senders;
		std::map<socket*, socket*>			_receiver_map;
		std::map<socket*, socket*>			_sender_map;

		int									_buffer_size;

		socket*								_last_accepted_sender;

	public:
		event<void(socket*)>						client_connected;
		event<void(socket*, disconnect_reason)>		client_disconnected;
		event<std::shared_ptr<strong_payload>(socket*, const strong_payload&)>	received;

		NETRANSFERATOR_API explicit tcp_server();
		NETRANSFERATOR_API ~tcp_server();

		/// Starts TCP server.  
		NETRANSFERATOR_API void start(
			const std::string& localhost,
			int rcvPort,
			int sndPort,
			int connectionNumber);

		/// Stops TCP server. 
		NETRANSFERATOR_API void stop();

		NETRANSFERATOR_API socket* get_sender_by_receiver(socket* receiver) const;
		NETRANSFERATOR_API socket* get_receiver_by_sender(socket* sender) const;

		NETRANSFERATOR_API bool send(
			const std::string& host,
			int port,
			const datarange& request,
			strong_payload& response);

		NETRANSFERATOR_API bool send(
			socket* receiver,
			const datarange& request,
			strong_payload& response);

		NETRANSFERATOR_API int get_buffer_size() const { return _buffer_size; }
		NETRANSFERATOR_API void set_buffer_size(int value) { _buffer_size = value; }

		NETRANSFERATOR_API socket* get_receiver() const { return _receiver; }
		NETRANSFERATOR_API socket* get_sender() const { return _sender; }

		NETRANSFERATOR_API std::vector<socket*> get_receivers() const { return _receivers; }
		NETRANSFERATOR_API std::vector<socket*> get_senders() const { return _senders; }

	private:
		bool send_request(socket* receiver, const byte* request, int requestSize);
		bool send_response(socket* sender, const byte* response, int responseSize);
		bool receive_request(socket* sender, byte* request, int requestSize);
		bool receive_response(socket* receiver, byte* response, int responseSize);

		void accept_senders_loop();
		void accept_receivers_loop();

		void receive_loop(socket* sender);
		void close_ticket(socket* client);
	};
}
