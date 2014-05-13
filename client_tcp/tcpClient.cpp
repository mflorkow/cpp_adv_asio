#include "tcpClient.h"
#include "Console.h"

tcpClient::tcpClient ( ) { }

void
tcpClient::openConnection ( std::string p_host_ , uint16_t p_port_ )
{
    if (!running)
    {
        //Opening the connection to the given host and on the given port.
        connection_ = tcpConnection::createConnection (io_service_);

        boost::asio::ip::tcp::resolver resolver (io_service_);
        //Resolving the hostname and connection the sockets.
        connection_->getSocket ().connect (*resolver.resolve ({boost::asio::ip::tcp::v4 () , p_host_ , std::to_string (p_port_)}));
        connection_->start ();
        running = true;
    }
}

void
tcpClient::sendMessage ( std::string p_msg_ )
{
    if (running)
    {
        connection_->sendData (p_msg_);
    }
}

void
tcpClient::shutdown ( )
{
    if (running)
    {
        io_service_.stop ();
        connection_->getSocket ().shutdown (connection_->getSocket ().shutdown_both);
        connection_->getSocket ().close ();
        running = false;
    }
}

boost::asio::io_service&
tcpClient::getIOService ( )
{
    return io_service_;
}