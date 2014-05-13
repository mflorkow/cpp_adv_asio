#include "tcpServer.h"
#include "chatServer.h"

tcpServer::tcpServer ( ) : acceptor_ ( io_service_ ) { }

void
tcpServer::addConnection ( tcpConnection::tcp_pointer p_newConnection_ )
{
    //While we add a new connection to the map, no other thread may change the map.
    boost::lock_guard<boost::mutex> lock (connectionsLock_);
    connections_.insert (std::pair<int , tcpConnection::tcp_pointer>(p_newConnection_->getSessionID () , p_newConnection_));
}

void
tcpServer::start_accept ( )
{
    tcpConnection::tcp_pointer new_connection = tcpConnection::createConnection (io_service_ , ++nextSessionID_);
    acceptor_.async_accept (new_connection->getSocket () ,
                            boost::bind (&tcpServer::handle_accept , this , new_connection ,
                                         boost::asio::placeholders::error));
    addConnection (new_connection);
}

void
tcpServer::handle_accept ( tcpConnection::tcp_pointer new_connection , const boost::system::error_code& error )
{
    if (!error)
    {
        new_connection->start ();
        new_connection->sendData ("Connection Established");
        chatServer::instance ().handelRequest (
                                               message_information (
                                                                       true , //This is a tcp connection
                                                                       new_connection->getSocket ().remote_endpoint ().address () ,
                                                                       new_connection->getSocket ().remote_endpoint ().port () ,
                                                                       "" , //we don't need a message
                                                                       message_information::NEW_CONNECTION));
    } else
    {
        std::cout << "Error in handle accept" << std::endl;
        std::cout << error.message () << std::endl;
    }
    start_accept ();
}

void
tcpServer::sendAtAll ( std::string message )
{
    boost::lock_guard<boost::mutex> lock (connectionsLock_);
    for (auto a : connections_)
    {
        a.second->sendData (message);
    }
}

boost::asio::io_service&
tcpServer::getIOService ( )
{
    return io_service_;
}

void
tcpServer::start ( const uint32_t port )
{
    if (!running_)
    {
        boost::asio::ip::tcp::endpoint endpoint_ (boost::asio::ip::tcp::v4 () , port);
        acceptor_.open (endpoint_.protocol ());
        acceptor_.set_option (boost::asio::ip::tcp::acceptor::reuse_address (true));
        acceptor_.bind (endpoint_);
        acceptor_.listen ();
        start_accept ();
        running_ = true;
    } else
    {
        throw std::string ("ERROR: Server already running");
    }
}

void
tcpServer::shutdown ( )
{
    if (running_)
    {
        sendAtAll ("Server is going down.");
        acceptor_.close ();
        connections_.clear ();
        running_ = false;
    }
}

void
tcpServer::reportDisconnect ( uint32_t sessionID )
{
    //While we remove a connection from the map, no other thread may change the map.
    boost::lock_guard<boost::mutex> lock (connectionsLock_);
    try
    {
        connections_.at (sessionID)->getSocket ().cancel ();
        connections_.erase (sessionID);
    } catch (...)
    {
        //ERROR
        //Do nothing
    }
}