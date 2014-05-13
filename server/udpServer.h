#ifndef UDPSERVER_H
#    define	UDPSERVER_H

#    include "chatServer.h"
#    include <set>

/**
 * The class tcpServer is used to handle the communication between the chat server and the tcp clients.
 */
class udpServer : public boost::noncopyable
{
public:

    /**
     * Static method to get access to the only instance of this class.
     * @return The instance of this class.
     */
    static udpServer &
    instance ( )
    {
        static udpServer inst;
        return inst;
    }

    /**
     * Starts the Server on the given port.
     * @param port The port on which the server should listen.
     */
    void start ( const uint16_t port );

    /**
     * Closes all connection and shuts the acceptor down.
     */
    void shutdown ( );

    /**
     * Sends a message to all clients
     * @param message The message to send.
     */
    void sendAtAll ( std::string message );

    /**
     * Returns the IO Service used in the acceptor.
     * @return 
     */
    boost::asio::io_service & getIOService ( );

private:
    /**
     * Private constructor
     */
    udpServer ( );

    /**
     * Adds a new client to the set of endpoints
     * @param p_address_ The ip address of the client.
     * @param p_port_ The port the client is using.
     */
    void addClient ( boost::asio::ip::address p_adress_ , uint16_t p_port_ );

    /**
     * Removes a client from the set of endpoints.
     * @param p_adress_ The IP address of the client.
     * @param p_port_ The port the client is using.
     */
    void removeClient ( boost::asio::ip::address p_adress_ , uint16_t p_port_ );

    /**
     * Start the listening.
     */
    void start_receive ( );

    /**
     * Asynchronous operation listening for new incoming messages.
     * @param error
     * @param bytes_transferred
     */
    void handle_receive ( const boost::system::error_code & error , std::size_t bytes_transferred );

    /**
     * Handles incoming messages.
     * @param p_message_ The message to handle.
     * @param p_host The host who send this.
     * @param p_port The port from where it came.
     */
    void handle_message ( std::string p_message_ , boost::asio::ip::address p_host , uint16_t p_port );

    /**
     * Sends a message to an endpoint.
     * @param p_message_ The message which should be send.
     * @param p_endpoint_ The recipient.
     */
    void sendToOne ( std::string p_message_ , boost::asio::ip::udp::endpoint p_endpoint_ );

    /** Current status of the server */
    bool running_ = false;

    /** Used to the program thread save */
    boost::mutex connectionsLock_;

    /** The IO Service we use */
    boost::asio::io_service io_service_;

    /** The socket listening for new connections */
    boost::asio::ip::udp::socket socket_;

    /** The endpoint used for receiving*/
    boost::asio::ip::udp::endpoint sending_endpoint_;

    /** The set containing all clients */
    std::set<std::pair<boost::asio::ip::address , uint16_t>> endpoints_;

    /** The size of the buffer we are going to use. */
    static const int BUFFERSIZE = 512;

    /** Buffer used to store the incoming messages. */
    std::vector<char> message_buffer_;

} ;

#endif	/* UDPSERVER_H */
