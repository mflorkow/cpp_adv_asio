#include "Console.h"
#include "udpClient.h"

Console::Console ( ) { }

void
Console::start ( std::string p_ipAdress_ , int p_port_ )
{
    udpClient::instance ().openConnection (p_ipAdress_ , p_port_);
}

void
Console::processCommand ( std::string cmd )
{
    if (cmd == "login")
    {
        //Do nothing
    } else if (cmd == "q" || cmd == "exit" || cmd == "logout" || cmd == "quit")
    {
        shutdown ();
    } else if (cmd != "")
    {
        udpClient::instance ().sendMessage (cmd);
    }
}

void
Console::shutdown ( )
{
    udpClient::instance ().shutdown ();
}

void
Console::printMessage ( std::string p_message_ )
{
    std::cout << p_message_ << std::endl;
}
