#include "message_information.h"

message_information::message_information ( bool p_isTcp_ , boost::asio::ip::address p_host_ , uint16_t p_port_ , std::string p_message_ , int p_connection_type ) :
isTcp_ ( p_isTcp_ ) , host_ ( p_host_ ) , port_ ( p_port_ ) , message_ ( p_message_ ) , connectionType_ ( p_connection_type ) {
}

const boost::asio::ip::address
message_information::getHost ( ) const
{
    return host_;
}

const std::string
message_information::getMessage ( ) const
{
    return message_;
}

const uint16_t
message_information::getPort ( ) const
{
    return port_;
}

const bool
message_information::isTcp ( ) const
{
    return isTcp_;
}

const int
message_information::getConnectionType ( ) const
{
    return connectionType_;
}
