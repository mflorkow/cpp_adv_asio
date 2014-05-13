#include "tcpConnection.h"
#include "tcpServer.h"
#include "chatServer.h"
#include <boost/bind.hpp>
#include <boost/array.hpp>

#include <boost/asio/read.hpp>
#include <boost/asio/read_at.hpp>

tcpConnection::tcp_pointer
tcpConnection::createConnection ( boost::asio::io_service & io_service_ , uint32_t sessionID )
{
    return tcp_pointer (new tcpConnection (io_service_ , sessionID));
}

boost::asio::ip::tcp::socket&
tcpConnection::getSocket ( )
{
    return socket_;
}

void
tcpConnection::start ( )
{
    if (!started_)
    {
        partnerAdress_ = socket_.remote_endpoint ().address ();
        partnerPort_ = socket_.remote_endpoint ().port ();
        boost::asio::async_read (socket_ ,
                                 boost::asio::buffer (&readMessage_.body_length_in_network_order_ , message::header_length_) ,
                                 boost::bind (&tcpConnection::handle_read_head , shared_from_this () , boost::asio::placeholders::error));
        started_ = true;
    }
}

tcpConnection::tcpConnection ( boost::asio::io_service & io_service_ , uint32_t sessionID ) : socket_ ( io_service_ ) , sessionID_ ( sessionID ) { }

void
tcpConnection::connectionInterrupted ( )
{
    tcpServer::instance ().reportDisconnect (sessionID_);
    chatServer::instance ().handelRequest (
                                           message_information (
                                                                   true ,
                                                                   partnerAdress_ ,
                                                                   partnerPort_ ,
                                                                   "" ,
                                                                   message_information::DISCONNECT));
}

void
tcpConnection::handle_read_head ( const boost::system::error_code& ec )
{
    if (!ec && readMessage_.decode_header ())
    {
        boost::asio::async_read (socket_ ,
                                 boost::asio::buffer (readMessage_.getBody () , readMessage_.getBodyLength ()) ,
                                 boost::bind (&tcpConnection::handle_read_body , shared_from_this () ,
                                              boost::asio::placeholders::error));

        chatServer::instance ().handelRequest (
                                               message_information (
                                                                       true ,
                                                                       socket_.remote_endpoint ().address () ,
                                                                       socket_.remote_endpoint ().port () ,
                                                                       readMessage_.getMessage () ,
                                                                       message_information::MESSAGE_FROM_CLIENT));
    } else
    {
        connectionInterrupted ();
    }
}

void
tcpConnection::handle_read_body ( const boost::system::error_code& ec )
{
    if (!ec)
    {
        boost::asio::async_read (socket_ ,
                                 boost::asio::buffer (&readMessage_.body_length_in_network_order_ , message::header_length_) ,
                                 boost::bind (&tcpConnection::handle_read_head , shared_from_this () ,
                                              boost::asio::placeholders::error));
    } else
    {
        connectionInterrupted ();
    }
}

void
tcpConnection::handle_write ( const boost::system::error_code& ec )
{
    if (!ec)
    {
        writeMessage_.pop_front ();
        if (!writeMessage_.empty ())
        {
            boost::asio::async_write (socket_ ,
                                      boost::asio::buffer (writeMessage_.front ().getData () ,
                                                           writeMessage_.front ().getDataLength ()) ,
                                      boost::asio::transfer_all () ,
                                      boost::bind (&tcpConnection::handle_write , this ,
                                                   boost::asio::placeholders::error));
        }
    } else
    {
        connectionInterrupted ();
    }
}

void
tcpConnection::sendData ( const std::string & p_message_ )
{
    if (started_)
    {
        bool write_in_progress = !writeMessage_.empty ();
        writeMessage_.push_back (message (p_message_));
        if (!write_in_progress)
        {
            boost::asio::async_write (socket_ ,
                                      boost::asio::buffer (writeMessage_.front ().getData () ,
                                                           writeMessage_.front ().getDataLength ()) ,
                                      boost::asio::transfer_all () ,
                                      boost::bind (&tcpConnection::handle_write , this ,
                                                   boost::asio::placeholders::error));
        }
    }
}

uint32_t
tcpConnection::getSessionID ( )
{
    return sessionID_;
}