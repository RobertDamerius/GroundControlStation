/**
 *  @file Network.hpp
 *  @brief The network header.
 *  @details Version 20200702.
 */
#pragma once


/* Default module namespace */
namespace RD {
namespace Network {


/* Some forward declarations for "friendly classes" */
class SocketBinding;
class TCPSocketBase;
class TCPServerSocket;
class TCPClientSocket;
class TCPClientConnection;
class UDPSocket;
class Endpoint;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Prototypes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**
 *  @brief Initialize the network module (OS specific).
 *  @return True if success, false otherwise.
 *  @details Call this before using any net prototype or class.
 */
bool InitializeNetwork(void);

/**
 *  @brief Terminate the network module (OS specific).
 */
void TerminateNetwork(void);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Classes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**
 *  @brief Abstract Class: SocketBase
 *  @details Holds the actual socket and provides basic close function.
 */
class SocketBase {
        friend SocketBinding;
        friend UDPSocket;
        friend TCPSocketBase;
        friend TCPClientSocket;
        friend TCPServerSocket;
        friend TCPClientConnection;

    public:
        /**
         *  @brief Create the @ref SocketBase.
         */
        SocketBase();

        /**
         *  @brief Destroy the @ref SocketBase.
         *  @details Calls the @ref Close() member function.
         */
        virtual ~SocketBase();

        /**
         *  @brief Close the socket.
         */
        virtual void Close(void);

        /**
         *  @brief Specify whether the socket is open or not.
         *  @return True if socket is open, false otherwise.
         */
        bool IsOpen(void);

        /**
         *  @brief Set socket options using the setsockopt() function.
         *  @param [in] level The level at which the option is defined (for example, SOL_SOCKET).
         *  @param [in] optname The socket option for which the value is to be set (for example, SO_BROADCAST).
         *  @param [in] optval A pointer to the buffer in which the value for the requested option is specified.
         *  @param [in] optlen The size, in bytes, of the buffer pointed to by the optval parameter.
         *  @return If no error occurs, zero is returned.
         */
        int SetOption(int level, int optname, const void *optval, int optlen);

        /**
         *  @brief Get socket option using the getsockopt() function.
         *  @param [in] level The level at which the option is defined (for example, SOL_SOCKET).
         *  @param [in] optname The socket option for which the value is to be retrieved (for example, SO_ACCEPTCONN).
         *  @param [in] optval A pointer to the buffer in which the value for the requested option is to be returned.
         *  @param [in] optlen A pointer to the size, in bytes, of the optval buffer.
         *  @return If not error occurs, zero is returned.
         */
        int GetOption(int level, int optname, void *optval, int *optlen);

        /**
         *  @brief Set socket option to reuse the address.
         *  @param [in] reuse True if address reuse should be enabled, false otherwise.
         *  @return If no error occurs, zero is returned.
         */
        int ReuseAddr(bool reuse);

        /**
         *  @brief Set socket option to reuse the port to allow multiple sockets to use the same port number.
         *  @param [in] reuse True if address reuse should be enabled, false otherwise.
         *  @return If no error occurs, zero is returned.
         */
        int ReusePort(bool reuse);

    private:
        std::atomic<int> _socket; ///< Socket value.

}; /* class: SocketBase */


/**
 *  @brief Abstract Class: SocketBinding
 *  @details Augments the base class with the possibility of binding a socket.
 */
class SocketBinding: public SocketBase {
        friend UDPSocket;
        friend TCPSocketBase;

    public:
        /**
         *  @brief Create the @ref SocketBinding.
         */
        SocketBinding();

        /**
         *  @brief Destroy the @ref SocketBinding.
         */
        virtual ~SocketBinding();

        /**
         *  @brief Close the socket.
         *  @details Resets the port and calls the Close member function of the @ref SocketBase class.
         */
        void Close(void);

        /**
         *  @brief Get the port bound to the socket.
         *  @return The port if success, < 0 if failure or no port was bound.
         */
        int GetPort(void);

        /**
         *  @brief Bind a port to a socket. The socket must be opened.
         *  @param [in] port A port that should be bound to the socket.
         *  @param [in] ip The IP for this socket/interface, default to nullptr. Example: "127.0.0.1". If nullptr is set as parameter, INADDR_ANY will be used.
         *  @return 0 if success, < 0 if failure or already bound.
         */
        int Bind(uint16_t port, const char *ip = nullptr);

        /**
         *  @brief Bind the next free port between portBegin and portEnd to the socket. The socket must be opened. If portBegin is greater than portEnd they will be swapped.
         *  @param [in] portBegin The beginning port that should be bound to the socket.
         *  @param [in] portEnd The ending port that should be bound to the socket.
         *  @param [in] ip The IP for this socket/interface, default to nullptr. Example: "127.0.0.1". If nullptr is set as parameter, INADDR_ANY will be used.
         *  @return The port that was bound with success, < 0 if failure or already bound.
         */
        int Bind(uint16_t portBegin, uint16_t portEnd, const char *ip = nullptr);

    private:
        std::atomic<int> _port; ///< Port value.

}; /* class: SocketBinding */


/**
 *  @brief Abstract Class: TCPSocketBase
 *  @details Augments the @ref SocketBinding class by an TCP specific Open() member function.
 */
class TCPSocketBase: public SocketBinding {
    public:
        /**
         *  @brief Create the @ref TCPSocketBase.
         */
        TCPSocketBase();

        /**
         *  @brief Destroy the @ref TCPSocketBase.
         */
        virtual ~TCPSocketBase();

        /**
         *  @brief Open the socket.
         *  @return 0 if success, < 0 if failure.
         */
        int Open(void);

}; /* class: TCPSocketBase */


/**
 *  @brief Class: Endpoint
 *  @details Stores information like IPv4 address and port for an endpoint.
 */
class Endpoint {
        friend UDPSocket;
        friend TCPClientSocket;

    public:
        /**
         *  @brief Create an endpoint object.
         */
        Endpoint();

        /**
         *  @brief Default copy constructor.
         */
        Endpoint(const Endpoint& e) = default;

        /**
         *  @brief Create an endpoint object.
         *  @param [in] ipv4_A Byte one of IPv4 address.
         *  @param [in] ipv4_B Byte two of IPv4 address.
         *  @param [in] ipv4_C Byte three of IPv4 address.
         *  @param [in] ipv4_D Byte four of IPv4 address.
         *  @param [in] port Port value.
         */
        Endpoint(uint8_t ipv4_A, uint8_t ipv4_B, uint8_t ipv4_C, uint8_t ipv4_D, uint16_t port);

        /**
         *  @brief Destroy the endpoint object.
         */
        ~Endpoint();

        /**
         *  @brief Reset the address.
         */
        void Reset(void);

        /**
         *  @brief Set address by a given sockaddr_in struct.
         *  @param [in] address The sockaddr_in address structure.
         */
        void SetAddress(sockaddr_in address);

        /**
         *  @brief Set address by a given in_addr struct.
         *  @param [in] address The in_addr address structure.
         */
        void SetAddress(in_addr address);

        /**
         *  @brief Set the port.
         *  @param [in] port Port value.
         */
        void SetPort(uint16_t port);

        /**
         *  @brief Set the IPv4 address.
         *  @param [in] ipv4_A Byte one of IPv4 address.
         *  @param [in] ipv4_B Byte two of IPv4 address.
         *  @param [in] ipv4_C Byte three of IPv4 address.
         *  @param [in] ipv4_D Byte four of IPv4 address.
         */
        void SetIPv4(uint8_t ipv4_A, uint8_t ipv4_B, uint8_t ipv4_C, uint8_t ipv4_D);

        /**
         *  @brief Get the current port.
         *  @return Port of this endpoint.
         */
        uint16_t GetPort(void);

        /**
         *  @brief Get the current IPv4 address.
         *  @param [out] ipv4 Pointer to array of 4x uint8_t where to store the IPv4 address.
         */
        void GetIPv4(uint8_t *ipv4);

        /**
         *  @brief Get the ID.
         *  @return The 8-byte ID: 0x00, 0x00, ipv4_A, ipv4_B, ipv4_C, ipv4_D, msb(port), lsb(port).
         */
        inline uint64_t GetID(void)const{ return this->id; }

        /**
         *  @brief Compare IPv4 address of this endpoint to a given IPv4 address.
         *  @param [in] ipv4_A Byte one of IPv4 address.
         *  @param [in] ipv4_B Byte two of IPv4 address.
         *  @param [in] ipv4_C Byte three of IPv4 address.
         *  @param [in] ipv4_D Byte four of IPv4 address.
         *  @return True, if IPv4 addresses match, false otherwise.
         */
        bool CompareIPv4(const uint8_t ipv4_A, const uint8_t ipv4_B, const uint8_t ipv4_C, const uint8_t ipv4_D);

        /**
         *  @brief Compare IPv4 address of this endpoint to a given endpoint.
         *  @param [in] endpoint A reference to another endpoint that should be compared with this endpoint.
         *  @return True, if IPv4 addresses match, false otherwise.
         */
        bool CompareIPv4(const Endpoint& endpoint);

        /**
         *  @brief Compare port of this endpoint to a given port.
         *  @param [in] port A port that should be compared to the port of this endpoint.
         *  @return True, if ports match, false otherwise.
         */
        bool ComparePort(const uint16_t port);

        /**
         *  @brief Compare port of this endpoint to a port of a given endpoint.
         *  @param [in] endpoint A reference to another endpoint that should be compared with this endpoint.
         *  @return True, if ports match, false otherwise.
         */
        bool ComparePort(const Endpoint& endpoint);

        /**
         *  @brief Operator=
         *  @return Reference to this endpoint.
         *  @details Copy address and port from another endpoint.
         */
        Endpoint& operator=(const Endpoint& rhs);

        /**
         *  @brief Operator==
         *  @return Returns true, if IPv4 address and port matches.
         *  @details Internally, the @ref id is compared.
         */
        bool operator==(const Endpoint& rhs)const;

        /**
         *  @brief Operator!=
         *  @return Returns false, if IPv4 address and port matches.
         *  @details Internally, the @ref id is compared.
         */
        bool operator!=(const Endpoint& rhs)const;

        /**
         *  @brief Convert to a string.
         *  @return String, containing IPv4 address and port. Format: "A.B.C.D:Port".
         */
        std::string ToString(void);

        /**
         *  @brief Decode the address and port from a string.
         *  @param [in] strEndpoint The IPv4-(port) string that should be decoded. The Format can be either "A.B.C.D" or "A.B.C.D:Port".
         *  @return True if success, false otherwise.
         */
        bool DecodeFromString(std::string strEndpoint);

    private:
        sockaddr_in addr;
        uint64_t id;

        /**
         *  @brief Update the private @ref id attribute.
         */
        void UpdateID(void);

}; /* class: Endpoint */


/**
 *  @brief Class: TCPClientConnection
 *  @details If a new TCP client is accepted by the @ref TCPServerSocket, this instance can be used to handle the communication to that client.
 */
class TCPClientConnection: public Endpoint, public SocketBase {
    public:
        /**
         *  @brief Create a TCP client connection object.
         */
        TCPClientConnection();

        /**
         *  @brief Destroy the TCP client connection object.
         */
        ~TCPClientConnection();

        /**
         *  @brief Send bytes.
         *  @param [in] bytes Bytes that should be sent.
         *  @param [in] size Number of bytes.
         *  @return Number of bytes that have been sent. If an error occurred, the return value is < 0.
         */
        int Send(uint8_t *bytes, int size);

        /**
         *  @brief Receive bytes.
         *  @param [out] bytes Pointer to data array, where received bytes should be stored.
         *  @param [in] size Maximum size of data array.
         *  @return Number of bytes that have been received. If an error occurred, the return value is < 0.
         */
        int Receive(uint8_t *bytes, int size);

        /**
         *  @brief Receive bytes our time out.
         *  @param [out] bytes bytes Pointer to data array, where received bytes should be stored.
         *  @param [in] size Maximum size of data array.
         *  @param [in] timeout_s Timeout in seconds.
         *  @return Number of bytes that have been received. If an error or timeout occurred, the return value is < 0.
         */
        int Receive(uint8_t *bytes, int size, uint32_t timeout_s);

}; /* class: TCPClientConnection */


/**
 *  @brief Class: TCPClientSocket
 *  @details Can be used to connect a client to a server.
 */
class TCPClientSocket: public TCPSocketBase {
    public:
        /**
         *  @brief Create a TCP client socket object.
         */
        TCPClientSocket();

        /**
         *  @brief Destroy the TCP client socket object.
         */
        ~TCPClientSocket();

        /**
         *  @brief Connect to an endpoint.
         *  @param [in] endpoint An endpoint to where the connection should be established.
         *  @return 0 if success, < 0 if failure.
         */
        int Connect(Endpoint& endpoint);

        /**
         *  @brief Send bytes.
         *  @param [in] bytes Bytes that should be sent.
         *  @param [in] size Number of bytes.
         *  @return Number of bytes that have been sent. If an error occurred, the return value is < 0.
         */
        int Send(uint8_t *bytes, int size);

        /**
         *  @brief Receive bytes.
         *  @param [out] bytes Pointer to data array, where received bytes should be stored.
         *  @param [in] size Maximum size of data array.
         *  @return Number of bytes that have been received. If an error occurred, the return value is < 0.
         */
        int Receive(uint8_t *bytes, int size);

        /**
         *  @brief Receive bytes our time out.
         *  @param [out] bytes bytes Pointer to data array, where received bytes should be stored.
         *  @param [in] size Maximum size of data array.
         *  @param [in] timeout_s Timeout in seconds.
         *  @return Number of bytes that have been received. If an error or timeout occurred, the return value is < 0.
         */
        int Receive(uint8_t *bytes, int size, uint32_t timeout_s);

}; /* class: TCPClientSocket */


/**
 *  @brief Class: TCPServerSocket
 *  @details Can be used as a TCP server, that listens and accepts clients. The communication to a client is handled via the @ref TCPClientConnection class.
 */
class TCPServerSocket: public TCPSocketBase {
    public:
        /**
         *  @brief Create a TCP server socket object.
         */
        TCPServerSocket();

        /**
         *  @brief Destroy the TCP server socket object.
         */
        ~TCPServerSocket();

        /**
         *  @brief Listen for a connection request.
         *  @param [in] backlog The maximum length of the queue of pending connections.
         *  @return 0 if success, < 0 if failure.
         */
        int Listen(int backlog);

        /**
         *  @brief Accept a client connection.
         *  @param [out] client Pointer to a client socket instance that represents the accepted connection.
         *  @return 0 if success, < 0 if failure.
         */
        int Accept(TCPClientConnection *client);

}; /* class: TCPServerSocket */


/**
 *  @brief Class: UDPSocket
 *  @details Can be used as a server or a client socket. This class provides a SendTo() member function to send bytes to a specified endpoint. The ReceiveFrom() member functions returns sender information.
 */
class UDPSocket: public SocketBinding {
    public:
        /**
         *  @brief Create a UDP socket object.
         */
        UDPSocket();

        /**
         *  @brief Destroy the UDP socket object.
         */
        ~UDPSocket();

        /**
         *  @brief Open the socket.
         *  @return 0 if success, < 0 if failure.
         */
        int Open(void);

        /**
         *  @brief Send bytes to endpoint.
         *  @param [in] endpoint The endpoint where to send the bytes to.
         *  @param [in] bytes Bytes that should be sent.
         *  @param [in] size Number of bytes.
         *  @return Number of bytes that have been sent. If an error occurred, the return value is < 0.
         */
        int SendTo(Endpoint& endpoint, uint8_t *bytes, int size);

        /**
         *  @brief Send a single broadcast message.
         *  @param [in] destinationPort The destination port for the broadcast message.
         *  @param [in] bytes Bytes that should be sent.
         *  @param [in] size Number of bytes.
         *  @return Number of bytes that have been sent. If an error occurred, the return value is < 0.
         *  @details The socket option will be changed from unicast to broadcast. After transmission the socket option will be changed back to unicast.
         */
        int Broadcast(uint16_t destinationPort, uint8_t *bytes, int size);

        /**
         *  @brief Receive bytes from endpoint.
         *  @param [out] endpoint Endpoint, where to store the sender information.
         *  @param [out] bytes Pointer to data array, where received bytes should be stored.
         *  @param [in] size The size of the data array.
         *  @return Number of bytes that have been received. If an error occurred, the return value is < 0.
         */
        int ReceiveFrom(Endpoint& endpoint, uint8_t *bytes, int size);

        /**
         *  @brief Receive bytes from endpoint or time out.
         *  @param [out] endpoint Endpoint, where to store the sender information.
         *  @param [out] bytes Pointer to data array, where received bytes should be stored.
         *  @param [in] size The size of the data array.
         *  @param [in] timeout_s Timeout in seconds.
         *  @return Number of bytes that have been received. If an error or timeout occurred, the return value is < 0.
         *  @note IMPORTANT: DO NOT USE THIS MEMBER FUNCTION TO RECEIVE MULTICAST MESSAGES.
         */
        int ReceiveFrom(Endpoint& endpoint, uint8_t *bytes, int size, uint32_t timeout_s);

        /**
         *  @brief Join a multicast group.
         *  @param [in] strGroup IPv4 address of the group.
         *  @param [in] strInterface IPv4 address of the interface to be used for multicasting or nullptr if default interface should be used.
         *  @return If no error occurs, zero is returned.
         */
        int JoinMulticastGroup(const char* strGroup,  const char* strInterface = nullptr);

        /**
         *  @brief Leave a multicast group.
         *  @param [in] strGroup IPv4 address of the group.
         *  @param [in] strInterface IPv4 address of the interface to be used for multicasting or nullptr if default interface should be used.
         *  @return If no error occurs, zero is returned.
         */
        int LeaveMulticastGroup(const char* strGroup, const char* strInterface = nullptr);

        /**
         *  @brief Set the interface for the transmission of multicast messages.
         *  @param [in] ip IPv4 interface.
         *  @param [in] strInterface IPv4 address of the interface to be used for multicasting or nullptr if default interface should be used.
         *  @return If no error occurs, zero is returned.
         */
        int SetMulticastInterface(const char* ip, const char* strInterface = nullptr);

        /**
         *  @brief Set time-to-live multicast messages.
         *  @param [in] ttl Time-to-live.
         *  @return If no error occurs, zero is returned.
         */
        int SetMulticastTTL(uint8_t ttl);

}; /* class: UDPSocket */


} /* namespace: Network */
} /* namespace: RD */

