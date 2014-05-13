#include "tcpMessage.h"
#include <sstream>
//for ntohl and htonl
#include <netinet/in.h>

tcpMessage::tcpMessage ( ) { }

tcpMessage::tcpMessage ( const tcpMessage& orig ) : body_length_in_host_order_ ( orig.body_length_in_host_order_ ) , body_length_in_network_order_ ( orig.body_length_in_network_order_ )
{
    data_ = new char[body_length_in_host_order_ + header_length_];
    std::copy (orig.data_ , orig.data_ + body_length_in_host_order_ + header_length_ , data_);
}

tcpMessage::tcpMessage ( std::string message )
{
    const char * messageAsCharArray = message.c_str ();
    body_length_in_host_order_ = message.length ();
    body_length_in_network_order_ = htonl (body_length_in_host_order_);
    data_ = new char[body_length_in_host_order_ + header_length_];
    std::copy (messageAsCharArray , messageAsCharArray + body_length_in_host_order_ , data_ + header_length_);
    char * ptrToLength = (char*) &body_length_in_network_order_;
    for (uint8_t i = 0; i < 4; ++i , ++ptrToLength)
    {
        data_[i] = *ptrToLength;
    }
}

tcpMessage::~tcpMessage ( )
{
    if (body_length_in_host_order_ != 0)
    {
        delete[] data_;
    }
}

const char*
tcpMessage::getData ( ) const
{
    return data_;
}

char*
tcpMessage::getData ( )
{
    return data_;
}

uint32_t
tcpMessage::getDataLength ( ) const
{
    return body_length_in_host_order_ + header_length_;
}

const char*
tcpMessage::getBody ( ) const
{
    return data_ + header_length_;
}

char*
tcpMessage::getBody ( )
{
    return data_ + header_length_;
}

uint32_t
tcpMessage::getBodyLength ( ) const
{
    return body_length_in_host_order_;
}

bool
tcpMessage::decode_header ( )
{
    body_length_in_host_order_ = ntohl (body_length_in_network_order_);
    if (body_length_in_host_order_ > max_body_length_)
    {
        body_length_in_host_order_ = 0;
        body_length_in_network_order_ = 0;
        return false;
    } else
    {
        data_ = new char[body_length_in_host_order_ + header_length_];
        char * ptrToLength = (char*) &body_length_in_network_order_;
        for (uint8_t i = 0; i < 4; ++i , ++ptrToLength)
        {
            data_[i] = *ptrToLength;
        }
    }
    return true;
}

const std::string
tcpMessage::getMessage ( ) const
{
    if (body_length_in_host_order_ != 0)
    {
        std::stringstream ss;
        ss.write (data_ + header_length_ , body_length_in_host_order_);
        return ss.str ();
    } else
    {
        throw std::string ("Error. Kein MessageBody vorhanden!");
    }
}
