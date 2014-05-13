#ifndef UDPCLIENT_H
#    define	UDPCLIENT_H

#    include <boost/asio.hpp>
#    include <vector>
#    include <boost/asio/buffer.hpp>

/**
 * The class udpClient is used to asynchronously communicate with a server.
 */
class udpClient : public boost::noncopyable
{
public:

    /**
     * Static method to get access to the only instance of this class.
     * @return The instance of this class.
     */
    static udpClient &
    instance ( )
    {
        static udpClient inst;
        return inst;
    }

    /**
     * Opens a connection to the given host.
     * @param p_host_ The host we want to connect to.
     * @param p_port_ The port of the host to connect to.
     */
    void openConnection ( std::string p_host_ , uint16_t p_port_ );

    /**
     * Closes the socket and stops all asynchronous operations.
     */
    void shutdown ( );

    /**
     * Sends a message through the connection.
     * @param p_msg_ The message we want to send.
     */
    void sendMessage ( std::string p_msg_ );

    /**
     * Returns the IO-Service used by this client.
     * @return The IO-Service the object is using.
     */
    boost::asio::io_service & getIOService ( );
private:

    /**
     * Private constructor
     */
    udpClient ( );

    /**
     * Asynchronous method waiting for new messages.
     * @param error The error that occurred.
     * @param bytes_transferred The amount of bytes transfered.
     */
    void handle_receive ( const boost::system::error_code& error , std::size_t bytes_transferred );

    /** This boolean is used to protect the client from opening new connections while an old one is still active.*/
    bool running = false;

    /** The size of the buffer we are going to use. */
    static const int BUFFERSIZE = 512;

    /** Buffer used to store the incoming messages. */
    std::vector<char> message_buffer;

    /** The IO-Service used by this client.*/
    boost::asio::io_service io_service_;

    /** The UDP socket used for communication.*/
    boost::asio::ip::udp::socket socket_;
} ;

#endif	/* UDPCLIENT_H */
