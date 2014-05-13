#include "udpServer.h"
#include <sstream>

udpServer::udpServer ( ) : socket_ ( io_service_ ) , message_buffer_ ( BUFFERSIZE , 0 ) { }

void
udpServer::addClient ( boost::asio::ip::address p_adress_ , uint16_t p_port_ )
{
    boost::lock_guard<boost::mutex> lock (connectionsLock_);
    endpoints_.insert (std::make_pair (p_adress_ , p_port_));
}

void
udpServer::removeClient ( boost::asio::ip::address p_adress_ , uint16_t p_port_ )
{
    boost::lock_guard<boost::mutex> lock (connectionsLock_);
    endpoints_.erase (std::make_pair (p_adress_ , p_port_));
}

void
udpServer::start ( const uint16_t port )
{
    if (!running_)
    {
        boost::asio::ip::udp::endpoint localEndpoint (boost::asio::ip::udp::v4 () , port);
        socket_ = boost::asio::ip::udp::socket (io_service_ , localEndpoint);
        running_ = true;
        start_receive ();
    } else
    {
        throw std::string ("ERROR: Server already running");
    }
}

void
udpServer::start_receive ( )
{
    socket_.async_receive_from (
                                boost::asio::buffer (message_buffer_) ,
                                sending_endpoint_ ,
                                boost::bind (&udpServer::handle_receive , this , boost::asio::placeholders::error , boost::asio::placeholders::bytes_transferred)
                                );
}

void
udpServer::handle_receive ( const boost::system::error_code& error , std::size_t bytes_transferred )
{
    if (!error && bytes_transferred > 0)
    {
        std::stringstream ss;
        ss.write (message_buffer_.data () , bytes_transferred);
        handle_message (ss.str () , sending_endpoint_.address () , sending_endpoint_.port ());

        socket_.async_receive_from (
                                    boost::asio::buffer (message_buffer_) ,
                                    sending_endpoint_ ,
                                    boost::bind (&udpServer::handle_receive , this , boost::asio::placeholders::error , boost::asio::placeholders::bytes_transferred)
                                    );
    } else
    {
        std::cout << "Error in handle receive" << std::endl;
    }
    start_receive ();
}

void
udpServer::sendToOne ( std::string p_message_ , boost::asio::ip::udp::endpoint p_endpoint_ )
{
    try
    {
        const char * msg = p_message_.c_str ();
        socket_.send_to (boost::asio::buffer (msg , p_message_.length ()) , p_endpoint_);
    } catch (...)
    {
        std::cout << "Error while sending" << std::endl;
    }
}

void
udpServer::handle_message ( std::string p_message_ , boost::asio::ip::address p_host , uint16_t p_port )
{
    int status = 0;
    if (p_message_ == "login")
    {
        addClient (p_host , p_port);
        status = message_information::NEW_CONNECTION;
        p_message_ = "";
    } else if (p_message_ == "logout")
    {
        removeClient (p_host , p_port);
        status = message_information::DISCONNECT;
        p_message_ = "";
    } else
    {
        status = message_information::MESSAGE_FROM_CLIENT;
    }
    chatServer::instance ().handelRequest (
                                           message_information{
                                           false , //This is a udp connection
                                           p_host ,
                                           p_port ,
                                           p_message_ ,
                                           status
    });
}

boost::asio::io_service&
udpServer::getIOService ( )
{
    return io_service_;
}

void
udpServer::shutdown ( )
{
    if (running_)
    {
        sendAtAll ("Server is going down");
        running_ = false;
        socket_.cancel ();
        io_service_.stop ();
    }
}

void
udpServer::sendAtAll ( std::string p_message_ )
{
    boost::lock_guard<boost::mutex> lock (connectionsLock_);
    for (auto host : endpoints_)
    {
        boost::asio::ip::udp::endpoint remote_endpoint (host.first , host.second);
        sendToOne (p_message_ , remote_endpoint);
    }
}