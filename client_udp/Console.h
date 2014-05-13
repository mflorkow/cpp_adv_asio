#ifndef CONSOLE_H
#    define	CONSOLE_H

#    include <boost/noncopyable.hpp>
#    include <string>

/**
 * The console is a noncopyable class used to handle IO actions.
 */
class Console : public boost::noncopyable
{
public:

    /**
     * Static method to get access to the only instance of this class.
     * @return The instance of this class.
     */
    static Console &
    instance ( )
    {
        static Console inst;
        return inst;
    }

    /**
     * Opens a connection to the given host.
     * @param p_host_ The host we want to connect to.
     * @param p_port_ The port where the server is listening.
     */
    void start ( std::string p_host_ , int p_port_ );

    /**
     * Filters the message for commands and sends the non command messages to the server.
     * @param cmd The command to process.
     */
    void processCommand ( std::string p_cmd_ );

    /**
     * Closes the connection and finish all asynchronous actions.
     */
    void shutdown ( );

    /**
     * Prints the given message to the std output and appends a new line.
     * @param p_message_ The message to print.
     */
    void printMessage ( std::string p_message_ );

private:
    /**
     * Default constructor
     */
    Console ( );

} ;

#endif	/* CONSOLE_H */