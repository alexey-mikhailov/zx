# pragma once
# include "netransferator_defines.h"

namespace zx
{
	struct endpoint
	{
		NETRANSFERATOR_API endpoint(
			std::string host_,
			unsigned short port_,
			const ::sockaddr_in& address_);

		NETRANSFERATOR_API endpoint(
			std::string host_,
			unsigned short port_);

		NETRANSFERATOR_API endpoint(const endpoint& source) = default;

		const std::string host;
		const unsigned short port;
		const sockaddr_in address;
		const std::string ip;
	};

	NETRANSFERATOR_API bool operator==(const endpoint& left, const endpoint& right);

	/// IP Windows socket
	class socket
	{
	public:
		explicit socket(bool reuseAddress = true);
		explicit socket(SOCKET socket);
		virtual ~socket();

		int bind(const endpoint& ep);
		int bind(const std::string& host, unsigned short port);

		int connect(const endpoint& ep);
		int connect(const std::string& host, unsigned short port);

		int listen(int connectionNumber) const;
		socket* accept();

		int send(const byte* data, int size) const;
		int receive(byte* data, int size) const;

		int shutdown(int how = SD_BOTH) const;
		int close();
		void shutdown_close(int how = SD_BOTH);

		endpoint* get_local() const { return _local; }
		endpoint* get_remote() const { return _remote; }
		SOCKET get_socket() const { return _socket; }

		/**
		 * \brief Determines whether socket disconnected after escape from blocking call or not.
		 * \param bytesSentExpected Amount of sent bytes was expected.
		 * \param bytesSentActually Amount of sent bytes was processed actually.
		 * \return
		 */
		bool is_disconnected_after_send(int bytesSentExpected, int bytesSentActually) const;

		/**
		 * \brief Determines whether socket disconnected after escape from blocking call or not.
		 * \param bytesReadExpected Amount of read bytes was expected.
		 * \param bytesReadActually Amount of read bytes was processed actually.
		 * \return
		 */
		bool is_disconntected_after_receive(int bytesReadExpected, int bytesReadActually) const;

	private:
		void set_local(const std::string& host, unsigned short port);
		void set_remote(const std::string& host, unsigned short port);
		void set_remote(const sockaddr_in& address);

		const SOCKET _socket;
		endpoint* _remote;
		endpoint* _local;
		bool _isConnected;
	};
}
