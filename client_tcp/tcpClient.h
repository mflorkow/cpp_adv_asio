#ifndef TCPCLIENT_H
#    define	TCPCLIENT_H

#    include "tcpConnection.h"
#    include <boost/asio.hpp>
#    include "tcpMessage.h"

/**
 * The class udpClient is used to asynchronously communicate with a server.
 */
class tcpClient : public boost::noncopyable
{
public:

    /**
     * Static method to get access to the only instance of this class.
     * @return The instance of this class.
     */
    static tcpClient &
    instance ( )
    {
        static tcpClient inst;
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
     * Private constructor.
     */
    tcpClient ( );

    /** The connection used for sending and receiving the messages*/
    tcpConnection::pointer connection_;

    /** The IO Service used for communication*/
    boost::asio::io_service io_service_;

    /** This boolean is used to protect the client from opening new connections while an old one is still active.*/
    bool running = false;

} ;

#endif	/* TCPCLIENT_H */