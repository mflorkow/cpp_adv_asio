#include "chatServer.h"
#include <sstream>

void
chatServer::handelRequest ( message_information p_request )
{
    if (p_request.getConnectionType () >= 1 && p_request.getConnectionType () <= 3)
    {
        std::stringstream ss;
        std::string host = p_request.getHost ().to_string ();
        uint16_t port = p_request.getPort ();
        ss << host << ":" << port;
        switch (p_request.getConnectionType ())
        {
            case message_information::NEW_CONNECTION:
                ss << " has connected";
                break;
            case message_information::MESSAGE_FROM_CLIENT:
                ss << "\t " << p_request.getMessage ();
                break;
            case message_information::DISCONNECT:
                ss << " has disconnected";
                break;
        }
        sendToAll (ss.str ());
    } else
    {
        //There are only three connection states, so this should not happen unless a programmer is doing something wrong.
    }
}

void
chatServer::sendToAll ( std::string p_message_ )
{
    tcpServer::instance ().sendAtAll (p_message_);
    udpServer::instance ().sendAtAll (p_message_);
}

chatServer::chatServer ( ) { }