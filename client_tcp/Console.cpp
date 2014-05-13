#include <iostream>
#include "Console.h"
#include "tcpClient.h"

Console::Console ( ) { }

void
Console::start ( std::string p_ipAdress_ , int p_port_ )
{
    tcpClient::instance ().openConnection (p_ipAdress_ , p_port_);
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
        tcpClient::instance ().sendMessage (cmd);
    }
}

void
Console::shutdown ( )
{
    tcpClient::instance ().shutdown ();
}

void
Console::printMessage ( std::string p_message_ )
{
    std::cout << p_message_ << std::endl;
}