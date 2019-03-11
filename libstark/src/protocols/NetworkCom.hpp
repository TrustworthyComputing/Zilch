#ifndef NETWORK_COM_HPP__
#define NETWORK_COM_HPP__

#include <iostream>
#include <string>            // For string
#include <cstring>

using namespace std;

#define RCVBUFSIZE 2048    // Size of receive buffer

/**
*   Base class representing basic communication endpoint
*/
class Socket {
public:
    /**
    *   Close and deallocate this socket
    */
    ~Socket();
    
    /**
    *   Get the local address
    *   @return local address of socket
    */
    string getLocalAddress();
    
    /**
    *   Get the local port
    *   @return local port of socket
    */
    unsigned short getLocalPort();
    
    /**
    *   Set the local port to the specified port and the local address to any interface
    *   @param localPort local port
    */
    void setLocalPort(unsigned short localPort);
    
    /**
    *   Set the local port to the specified port and the local address to the specified address.  If you omit the port, a random port  will be selected.
    *   @param localAddress local address
    *   @param localPort local port
    */
    void setLocalAddressAndPort(const string &localAddress, unsigned short localPort = 0);
    
    /**
    *   Resolve the specified service for the specified protocol to the corresponding port number in host byte order
    *   @param service service to resolve (e.g., "http")
    *   @param protocol protocol of service to resolve.  Default is "tcp".
    */
    static unsigned short resolveService(const string &service, const string &protocol = "tcp");
    
private:
    // Prevent the user from trying to use value semantics on this object
    Socket(const Socket &sock);
    void operator=(const Socket &sock);
    
protected:
    int sockDesc;              // Socket descriptor
    Socket(int type, int protocol);
    Socket(int sockDesc);
};

/**
*   Socket which is able to connect, send, and receive
*/
class CommunicatingSocket : public Socket {
public:
    /**
    *   Establish a socket connection with the given foreign address and port
    *   @param foreignAddress foreign address (IP address or name)
    *   @param foreignPort foreign port
    */
    void connect(const string &foreignAddress, unsigned short foreignPort);
    
    /**
    *   Write the given buffer to this socket.  Call connect() before calling send()
    *   @param buffer buffer to be written
    *   @param bufferLen number of bytes from buffer to be written
    */
    void send(const void *buffer, int bufferLen);
    
    /**
    *   Read into the given buffer up to bufferLen bytes data from this socket.  Call connect() before calling recv()
    *   @param buffer buffer to receive the data
    *   @param bufferLen maximum number of bytes to read into buffer
    *   @return number of bytes read, 0 for EOF, and -1 for error
    */
    int recv(void *buffer, int bufferLen);
    
    /**
    *   Get the foreign address.  Call connect() before calling recv()
    *   @return foreign address
    */
    string getForeignAddress();
    
    /**
    *   Get the foreign port.  Call connect() before calling recv()
    *   @return foreign port
    */
    unsigned short getForeignPort();
    
protected:
    CommunicatingSocket(int type, int protocol);
    CommunicatingSocket(int newConnSD);
};

/**
*   TCP socket for communication with other TCP sockets
*/
class TCPSocket : public CommunicatingSocket {
public:
    /**
    *   Construct a TCP socket with no connection
    */
    TCPSocket();
    
    /**
    *   Construct a TCP socket with a connection to the given foreign address and port
    *   @param foreignAddress foreign address (IP address or name)
    *   @param foreignPort foreign port
    */
    TCPSocket(const string &foreignAddress, unsigned short foreignPort);
    
private:
    // Access for TCPServerSocket::accept() connection creation
    friend class TCPServerSocket;
    TCPSocket(int newConnSD);
};

/**
*   TCP socket class for servers
*/
class TCPServerSocket : public Socket {
public:
    /**
    *   Construct a TCP socket for use with a server, accepting connections on the specified port on any interface
    *   @param localPort local port of server socket, a value of zero will give a system-assigned unused port
    *   @param queueLen maximum queue length for outstanding connection requests (default 5)
    */
    TCPServerSocket(unsigned short localPort, int queueLen = 5);
    
    /**
    *   Construct a TCP socket for use with a server, accepting connections on the specified port on the interface specified by the given address
    *   @param localAddress local interface (address) of server socket
    *   @param localPort local port of server socket
    *   @param queueLen maximum queue length for outstanding 
    *                   connection requests (default 5)
    */
    TCPServerSocket(const string &localAddress, unsigned short localPort, int queueLen = 5);
    
    /**
    *   Blocks until a new connection is established on this socket or error
    *   @return new connection socket
    */
    TCPSocket *accept();
    
private:
    void setListen(int queueLen);
};

#endif //ifndef NETWORK_COM_HPP__
