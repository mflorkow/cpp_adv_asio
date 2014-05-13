#ifndef CHATSERVER_H
#    define	CHATSERVER_H

#    include "message_information.h"
#    include "tcpServer.h"
#    include "udpServer.h"

class chatServer : public boost::noncopyable
{
public:

    /**
     * Static method to get access to the only instance of this class.
     * @return The instance of this class.
     */
    static chatServer &
    instance ( )
    {
        static chatServer inst;
        return inst;
    }

    /**
     * Handles new incoming messages from a connection.
     * @param p_request
     */
    void handelRequest ( message_information p_request );

private:
    /**
     * Private constructor
     */
    chatServer ( );

    /**
     * Sends a message to all connected clients.
     * @param p_message_
     */
    void sendToAll ( std::string p_message_ );
} ;

#endif	/* CHATSERVER_H */
