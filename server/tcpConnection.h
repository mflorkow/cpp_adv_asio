#ifndef TCPCONNECTION_H
#    define	TCPCONNECTION_H

#    include <boost/shared_ptr.hpp>
#    include <boost/enable_shared_from_this.hpp>
#    include <boost/asio.hpp>

#    include "message.h"
#    include <deque>

//Forward declaration
class tcpServer;

typedef std::deque<message> tcpMessageQueue;

/**
 * The class tcpConnection creates a socket-connection to a target. 
 * As soon as the connection is established you may send and receive messages. 
 */
class tcpConnection : public boost::enable_shared_from_this<tcpConnection>
{
public:

    typedef boost::shared_ptr<tcpConnection> tcp_pointer;

    /**
     * Creates a new boost::shared_ptr<tcpConnection>  from the given IO service.
     * @param io_service The IO service the socket should use.
     * @param sessionID The unique ID of this connection/session
     * @return a new Connection.
     */
    static tcp_pointer createConnection ( boost::asio::io_service & io_service_ , uint32_t sessionID );


    /**
     * Returns the socket encapsulated in this connection. 
     * @return The socket used for communication.
     */
    boost::asio::ip::tcp::socket& getSocket ( );

    /**
     * Opens the sockets and enables sending and receiving messages.
     */
    void start ( );

    /**
     * Sends the message through the socket to the recipient. 
     * @param message The message we want to send.
     */
    void sendData ( const std::string & message );

    /**
     * Returns the session ID of this connection.
     * @return ID of this session/connection.
     */
    uint32_t getSessionID ( );
private:

    /**
     * Private constructor  
     * @param io_service The IO Service running in the background.
     * @param sessionID The IF of this session/connection.
     */
    tcpConnection ( boost::asio::io_service & io_service_ , uint32_t sessionID );

    /**
     * Async Method the write bytes into the socket.
     * @param ec
     */
    void handle_write ( const boost::system::error_code& ec );

    /**
     * Async Method to read the head from the socket.
     * @param ec
     */
    void handle_read_head ( const boost::system::error_code& ec );

    /**
     * Async Method to read the message from the socket.
     * @param ec
     */
    void handle_read_body ( const boost::system::error_code& ec );

    /**
     * This method is called if an error occurred while sending or receiving a message.
     */
    void connectionInterrupted ( );

    /** The address of the connection partner */
    boost::asio::ip::address partnerAdress_;

    /** The port of the connection partner*/
    uint8_t partnerPort_;

    /** The socket used for sending and receiving.*/
    boost::asio::ip::tcp::socket socket_;

    /** The latest message read from the socket. */
    message readMessage_;

    /** A queue containing all messages which will be send in chronological order.*/
    tcpMessageQueue writeMessage_;

    /** The ID of this connection/session. */
    uint32_t sessionID_;

    /** Is the connection started*/
    bool started_ = false;
} ;

#endif	/* TCPCONNECTION_H */