#ifndef TCPSERVER_H
#    define	TCPSERVER_H

#    include "tcpConnection.h"
#    include <boost/thread.hpp>
#    include <unordered_map>

/**
 * The class tcpServer is used to handle the communication between the chat server and the tcp clients.
 */
class tcpServer : public boost::noncopyable
{
public:

    /**
     * Static method to get access to the only instance of this class.
     * @return The instance of this class.
     */
    static tcpServer &
    instance ( )
    {
        static tcpServer inst;
        return inst;
    }

    /**
     * Starts the Server on the given port.
     * @param port The port on which the server should listen.
     */
    void start ( const uint32_t port );

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
     * Reports a disconnect of a client.
     * @param sessionID The session ID that disconnected.
     */
    void reportDisconnect ( uint32_t sessionID );

    /**
     * Returns the IO Service used in the acceptor.
     * @return 
     */
    boost::asio::io_service & getIOService ( );

private:
    /**
     * Private-constructor
     */
    tcpServer ( );

    /**
     * Tells the server to wait for a new connection.
     */
    void start_accept ( );

    /**
     * Once a new connection is established, this method will activate the async components of the connection.
     * @param new_connection The connection to launch.
     * @param error The error code if an error occurred somewhere.
     */
    void handle_accept ( tcpConnection::tcp_pointer new_connection , const boost::system::error_code& error );

    /**
     * Adds a new connection to the map of active connection.
     * @param c The new connection to add.
     */
    void addConnection ( tcpConnection::tcp_pointer c );

    /** Current status of the server */
    bool running_ = false;
    /** The next ID of a new connection */
    uint32_t nextSessionID_ = 0;
    /** Used to make the list of connection thread save */
    boost::mutex connectionsLock_;
    /** The IO Service we use */
    boost::asio::io_service io_service_;
    /** The acceptor listening for new connections */
    boost::asio::ip::tcp::acceptor acceptor_;
    /** The map containing all connections */
    std::unordered_map<int , tcpConnection::tcp_pointer> connections_;
} ;

#endif	/* TCPSERVER_H */