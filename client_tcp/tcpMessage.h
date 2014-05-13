#ifndef TCPMESSAGE_H
#    define	TCPMESSAGE_H

#    include <cstdlib>
#    include <string>

/**
 * The class tcpMessage is used to communicate through the network with other programs. 
 * It consists of an head and body.
 * 
 * There are two ways to use it.
 * The first use is for writing a message.
 * 
 * It wraps a string message into a char(byte) array.
 * The first four bytes of the array are filled with an unsigned integer representing the length of the message itself (the head). 
 * The rest is filled with the message transformed into bytes (the body).
 * The integer is in network-order (big endian) and must be converted into host-order to be useable.
 * 
 * The second use if for receiving messages.
 * 
 * For this you read the first 4 bytes from the network directly into the public variable body_length_in_network_order_. 
 * After this you can decode the header to create an array big enough to store the message.
 * 
 * The maximum length of a message is 2^32-1 Bytes.
 */
class tcpMessage
{
public:
    /**
     * Default constructor
     */
    tcpMessage ( );

    /**
     * Creates a tcpMessage from a string.
     * @param message The String that should be converted.
     */
    tcpMessage ( std::string message );

    /**
     * Copy-constructor.
     * @param orig
     */
    tcpMessage ( const tcpMessage & orig );

    /**
     * Destructor
     */
    virtual ~tcpMessage ( );

    /**
     * Returns a constant pointer to the underlying array.
     * @return Pointer to the data array.
     */
    const char* getData ( ) const;

    /**
     * Returns a non-constant pointer to the underlying array.
     * @return  Pointer to the data array.
     */
    char* getData ( );

    /**
     * Returns the length of the body plus the length of the head.
     * @return Length of the underlying array.
     */
    uint32_t getDataLength ( ) const;

    /**
     * Returns a constant pointer to the underlying array starting after the head space.
     * @return Pointer to the body array.
     */
    const char* getBody ( ) const;

    /**
     * Returns a non-constant pointer to the underlying array starting after the head space.
     * @return Pointer to the body array.
     */
    char* getBody ( );

    /**
     * Return the length of the message in host order.
     * @return message length;
     */
    uint32_t getBodyLength ( ) const;

    /**
     * Decodes the header.
     * @return true if successful, false if an error occurred.
     */
    bool decode_header ( );

    /**
     * Returns the stored message 
     * @return Stored bytes transformed into a string.
     */
    const std::string getMessage ( ) const;

    /** Length of the header */
    static const uint32_t header_length_ = 4;
    /** May Length of the message */
    static const uint32_t max_body_length_ = INT32_MAX;

    /** The length of the message in network order */
    uint32_t body_length_in_network_order_ = 0;

private:
    /** The data contains 4 byte's of the body length in network order and the byte's of the message. The array will be allocated when we know how long the message will be. */
    char * data_;
    /** The length of the message in host-order */
    uint32_t body_length_in_host_order_ = 0;
} ;

#endif	/* TCPMESSAGE_H */