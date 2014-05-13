#ifndef TCPCONNECTION_H
#    define	TCPCONNECTION_H

#    include <boost/shared_ptr.hpp>
#    include <boost/enable_shared_from_this.hpp>
#    include <boost/asio.hpp>

#    include "tcpMessage.h"
#    include <deque>

typedef std::deque<tcpMessage> tcpMessageQueue;

/**
 * The class tcpConnection creates a socket-connection to a target. 
 * As soon as the connection is established you may send and receive messages. 
 */
class tcpConnection : public boost::enable_shared_from_this<tcpConnection>
{
public:
    typedef boost::shared_ptr<tcpConnection> pointer;

    /**
     * Creates a new boost::shared_ptr<tcpConnection>  from the given IO service.
     * @param io_service the IO service the socket should use.
     * @return a new Connection.
     */
    static pointer createConnection ( boost::asio::io_service& io_service );

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

private:
    /**
     * Private constructor  
     * @param io_service The IO Service running in the background.
     */
    tcpConnection ( boost::asio::io_service& io_service );

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

    /** The socket used for sending and receiving.*/
    boost::asio::ip::tcp::socket socket_;

    /** The latest message read from the socket. */
    tcpMessage readMessage_;

    /** A queue containing all messages which will be send in chronological order.*/
    tcpMessageQueue writeMessage_;
} ;

#endif	/* TCPCONNECTION_H */