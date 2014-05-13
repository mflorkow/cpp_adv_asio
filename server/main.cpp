#include "tcpServer.h"
#include "udpServer.h"
#include "chatServer.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <ratio>

/**
 * Returns a input string from the shell.
 * @return A string the user entered.
 */
std::string
getInputString ( std::string message_ )
{
    std::string temp_;
    std::cout << message_;
    std::getline (std::cin , temp_ , std::cin.widen ('\n'));
    return temp_;
}

/**
 * Launches The servers on a specified ports.
 * @param tcp_port_ The port for the tcp-Server.
 * @param udp_port_ The port for the udp-Server.
 */
void
launchServer ( int tcp_port_ , int udp_port_ )
{
    //Launching the TCP Server
    tcpServer::instance ().start (tcp_port_);
    //Launching the UDP Server
    udpServer::instance ().start (udp_port_);

    //starting the Threads for the IO Services
    boost::thread tcpThread (boost::bind (&boost::asio::io_service::run , &tcpServer::instance ().getIOService ()));
    std::cout << "TCP Server successfully launched." << std::endl << "Now listening on port: " << tcp_port_ << std::endl;

    boost::thread udpThread (boost::bind (&boost::asio::io_service::run , &udpServer::instance ().getIOService ()));
    std::cout << "UDP Server successfully launched." << std::endl << "Now listening on port: " << udp_port_ << std::endl;

    //Stopping loop
    while (getInputString ("Enter 'q' to stop: \n") != "q");

    //Stopping the TCP Server
    tcpServer::instance ().getIOService ().stop ();
    tcpServer::instance ().shutdown ();

    //Stopping the UDP Server
    udpServer::instance ().getIOService ().stop ();
    udpServer::instance ().shutdown ();
}

int
main ( int argc , char *argv[] )
{
    if (argc < 3 )
    {
        std::cout << "Usage: tcp_udp_server <tcp_port> <udp_port>" << std::endl;
    } else
    {
        try
        {
            uint16_t tcp_port = std::stoi (argv[1]);
            uint16_t udp_port = std::stoi (argv[2]);
            if (tcp_port == udp_port || tcp_port < 4096 || tcp_port > 32768 || udp_port < 4096 || udp_port > 32768)
            {
                throw std::exception ();
            } else
            {
                try
                {
                    launchServer (tcp_port , udp_port);
                } catch (boost::exception & ec)
                {
                    std::cout << "Ports already in use. Please choose a different one" << std::endl;
                }
            }
            //We do not care what exception has been thrown
        } catch (...)
        {
            std::cout << "Not a valid Port. Please choose a port between 0 and 65535." << std::endl;
        }
    }
    return 0;
}

