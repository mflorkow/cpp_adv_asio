#ifndef CONNECTION_INFORMATION_H
#    define	CONNECTION_INFORMATION_H

#    include <string>
#    include <boost/asio/ip/address.hpp>

/**
 * Stores information about a message.
 * Was send by either tcp or udp.
 * Has a target host.
 * Has a target port.
 * Has a message.
 * Is of a specific type(see INFORMATION_TYPE)
 */
class message_information
{
public:

    enum INFORMATION_TYPE
    {
        NEW_CONNECTION = 1 ,
        MESSAGE_FROM_CLIENT = 2 ,
        DISCONNECT = 3
    } ;

    message_information ( bool p_isTcp_ , boost::asio::ip::address p_host_ , uint16_t p_port_ , std::string p_message_ , int p_connection_type );

    const bool isTcp ( ) const;
    const boost::asio::ip::address getHost ( ) const;
    const uint16_t getPort ( ) const;
    const std::string getMessage ( ) const;
    const int getConnectionType ( ) const;

private:
    const bool isTcp_;
    const boost::asio::ip::address host_;
    const uint16_t port_;
    const std::string message_;
    const int connectionType_;
} ;

#endif	/* CONNECTION_INFORMATION_H */
