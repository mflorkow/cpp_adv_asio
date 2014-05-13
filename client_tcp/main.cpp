#include "Console.h"
#include <boost/thread.hpp>
#include "tcpClient.h"

/**
 * Returns a input string from the shell.
 * @return A string the user entered.
 */
std::string
getInputString ( )
{
    std::string temp;
    // Endline to mark the end of an input
    std::getline (std::cin , temp , std::cin.widen ('\n'));
    return temp;
}

/**
 * Starts the connection to the given host and port.
 * Also starts the IO Service in a separate thread.
 * @param p_host_ The host we want to connect to.
 * @param p_port_ The port on which the server is listening.
 */
void
startConnection ( std::string p_host_ , uint16_t p_port_ )
{
    Console & client_ = Console::instance ();
    client_.start (p_host_ , p_port_);
    boost::thread bt (boost::bind (&boost::asio::io_service::run , &tcpClient::instance ().getIOService ()));

}

/**
 * Main method.
 * 
 * You launch the client with a target IP-Address and Port. 
 * If the connection could be successfully established, you are able to send messages to the server.
 * 
 * @param argc
 * @param argv
 * @return 
 */
int
main ( int argc , char *argv[] )
{
    if (argc < 3)
    {
        std::cout << "Please launch the Client with a destination host and port" << std::endl;
    } else
    {
        try
        {
            std::string adress = argv[1];
            int port = std::stoi (argv[2]);
            if (port < 4096 || port > 32768)
            {
                throw std::exception ();
            } else
            {
                try
                {
                    std::cout << "Trying to connect to " << adress << " on Port " << port << std::endl;
                    startConnection (adress , port);
                    std::cout << "To stop the client just enter 'q', 'quit' , 'exit' or 'logout'." << std::endl;
                    //Loop to keep the program alive and handle input.
                    while (!tcpClient::instance ().getIOService ().stopped ())
                    {
                        Console::instance ().processCommand (getInputString ());
                    }
                } catch (boost::exception & ec)
                {
                    std::cout << "An error occurred. The program will be closed" << std::endl;
                }
            }
        } catch (std::exception)
        {
            std::cout << "Not a valid Port. Please choose a port between 4096 and 32768." << std::endl;
        }
    }
    return EXIT_SUCCESS;
}