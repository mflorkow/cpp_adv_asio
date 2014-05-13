#include "tcpConnection.h"
#include "Console.h"
#include <boost/bind.hpp>

tcpConnection::pointer
tcpConnection::createConnection ( boost::asio::io_service& io_service )
{
    return pointer (new tcpConnection (io_service));
}

boost::asio::ip::tcp::socket&
tcpConnection::getSocket ( )
{
    return socket_;
}

void
tcpConnection::start ( )
{
    boost::asio::async_read (socket_ ,
                             boost::asio::buffer (&readMessage_.body_length_in_network_order_ , tcpMessage::header_length_) ,
                             boost::bind (&tcpConnection::handle_read_head , shared_from_this () , boost::asio::placeholders::error));
}

tcpConnection::tcpConnection ( boost::asio::io_service& io_service ) : socket_ ( io_service ) { }

void
tcpConnection::handle_read_head ( const boost::system::error_code& ec )
{
    if (!ec && readMessage_.decode_header ())
    {
        boost::asio::async_read (socket_ ,
                                 boost::asio::buffer (readMessage_.getBody () , readMessage_.getBodyLength ()) ,
                                 boost::bind (&tcpConnection::handle_read_body , shared_from_this () ,
                                              boost::asio::placeholders::error));
        Console::instance ().printMessage (readMessage_.getMessage ());
    } else
    {
        std::cout << "An error occurred. The program will be closed." << std::endl;
        Console::instance ().shutdown ();
    }
}

void
tcpConnection::handle_read_body ( const boost::system::error_code& ec )
{
    if (!ec)
    {
        boost::asio::async_read (socket_ ,
                                 boost::asio::buffer (&readMessage_.body_length_in_network_order_ , tcpMessage::header_length_) ,
                                 boost::bind (&tcpConnection::handle_read_head , shared_from_this () ,
                                              boost::asio::placeholders::error));
    } else
    {
        std::cout << "An error occurred. The program will be closed." << std::endl;
        Console::instance ().shutdown ();
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
        std::cout << "An error occurred. The program will be closed." << std::endl;
        Console::instance ().shutdown ();
    }
}

void
tcpConnection::sendData ( const std::string & message )
{
    bool write_in_progress = !writeMessage_.empty ();
    writeMessage_.push_back (tcpMessage (message));
    if (!write_in_progress)
    {
        boost::asio::async_write (socket_ ,
                                  boost::asio::buffer (writeMessage_.front ().getData () ,
                                                       writeMessage_.front ().getDataLength ()) ,
                                  boost::bind (&tcpConnection::handle_write , this ,
                                               boost::asio::placeholders::error));
    }
}
