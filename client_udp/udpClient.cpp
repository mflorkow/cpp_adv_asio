#include "udpClient.h"
#include "Console.h"
#include <boost/bind.hpp>

udpClient::udpClient ( ) : socket_ ( io_service_ ) , message_buffer ( BUFFERSIZE , 0 ) { }

boost::asio::io_service&
udpClient::getIOService ( )
{
    return io_service_;
}

void
udpClient::shutdown ( )
{
    if (running)
    {
        sendMessage ("logout");
        running = false;
        socket_.cancel ();
        io_service_.stop ();
    }
}

void
udpClient::sendMessage ( std::string p_msg_ )
{
    const char * msg_ = p_msg_.c_str ();
    socket_.send (boost::asio::buffer (msg_ , p_msg_.length ()));
}

void
udpClient::openConnection ( std::string p_host_ , uint16_t p_port_ )
{
    if (!running)
    {
        boost::asio::ip::udp::resolver resolver (io_service_);
        //Opening the connection to the given host and on the given port.
        socket_.connect (*resolver.resolve ({boost::asio::ip::udp::v4 () , p_host_ , std::to_string (p_port_)}));

        //We do not need bigger buffer.
        socket_.set_option (boost::asio::socket_base::send_buffer_size (BUFFERSIZE));
        socket_.set_option (boost::asio::socket_base::receive_buffer_size (BUFFERSIZE));
        //We use this to initialize the connection
        sendMessage ("login");
        //Starting the asynchronous operations.
        socket_.async_receive (
                               boost::asio::buffer (message_buffer) ,
                               boost::bind (&udpClient::handle_receive , this , boost::asio::placeholders::error , boost::asio::placeholders::bytes_transferred)
                               );
        running = true;
    }
}

void
udpClient::handle_receive ( const boost::system::error_code& error , std::size_t bytes_transferred )
{
    if (!error && bytes_transferred > 0)
    {

        //Transforming the bytes into a string.
        std::stringstream ss;
        ss.write (message_buffer.data () , bytes_transferred);

        //Sending the message to the console.
        Console::instance ().printMessage (ss.str ());

        socket_.async_receive (
                               boost::asio::buffer (message_buffer) ,
                               boost::bind (&udpClient::handle_receive , this , boost::asio::placeholders::error , boost::asio::placeholders::bytes_transferred)
                               );
    } else
    {
        std::cout << "An error occurred. The program will be closed" << std::endl;
        shutdown ();
    }
}
