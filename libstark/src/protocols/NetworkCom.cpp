#include "NetworkCom.hpp"

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
typedef void raw_type;       // Type used for raw data on this platform
#include <errno.h>             // For errno

using namespace std;

static void fillAddr(const string &address, unsigned short port, sockaddr_in &addr) {
    memset(&addr, 0, sizeof(addr));  // Zero out address structure
    addr.sin_family = AF_INET;       // Internet address
    
    hostent *host;  // Resolve name
    if ((host = gethostbyname(address.c_str())) == NULL) {
        perror("Failed to resolve name (gethostbyname())");
    }
    addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
    addr.sin_port = htons(port);     // Assign port in network byte order
}

Socket::Socket(int type, int protocol) {
    if ((sockDesc = socket(PF_INET, type, protocol)) < 0) {
        perror("Socket creation failed (socket())");
    }
}

Socket::Socket(int sockDesc) {
    this->sockDesc = sockDesc;
}

Socket::~Socket() {
    ::close(sockDesc);
    sockDesc = -1;
}

string Socket::getLocalAddress() {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);
    if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
        perror("Fetch of local address failed (getsockname())");
    }
    return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort() {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);
    if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
        perror("Fetch of local port failed (getsockname())");
    }
    return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort) {
    // Bind the socket to its port
    sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(localPort);
    
    if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
        perror("Set of local port failed (bind())");
    }
}

void Socket::setLocalAddressAndPort(const string &localAddress, unsigned short localPort) {
    // Get the address of the requested host
    sockaddr_in localAddr;
    fillAddr(localAddress, localPort, localAddr);
    
    if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
        perror("Set of local address and port failed (bind())");
    }
}

unsigned short Socket::resolveService(const string &service, const string &protocol) {
    struct servent *serv;        /* Structure containing service information */
    
    if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL)
    return atoi(service.c_str());  /* Service is port number */
    else 
    return ntohs(serv->s_port);    /* Found port (network byte order) by name */
}

// CommunicatingSocket Code

CommunicatingSocket::CommunicatingSocket(int type, int protocol) : Socket(type, protocol) {}

CommunicatingSocket::CommunicatingSocket(int newConnSD) : Socket(newConnSD) {}

void CommunicatingSocket::connect(const string &foreignAddress, unsigned short foreignPort) {
    // Get the address of the requested host
    sockaddr_in destAddr;
    fillAddr(foreignAddress, foreignPort, destAddr);
    
    // Try to connect to the given port
    if (::connect(sockDesc, (sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
        perror("Connect failed (connect())");
    }
}

void CommunicatingSocket::send(const void *buffer, int bufferLen) {
    if (::send(sockDesc, (raw_type *) buffer, bufferLen, 0) < 0) {
        perror("Send failed (send())");
    }
}

int CommunicatingSocket::recv(void *buffer, int bufferLen)  {
    int rtn;
    if ((rtn = ::recv(sockDesc, (raw_type *) buffer, bufferLen, 0)) < 0) {
        perror("Received failed (recv())");
    }
    return rtn;
}

string CommunicatingSocket::getForeignAddress() {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);
    
    if (getpeername(sockDesc, (sockaddr *) &addr,(socklen_t *) &addr_len) < 0) {
        perror("Fetch of foreign address failed (getpeername())");
    }
    return inet_ntoa(addr.sin_addr);
}

unsigned short CommunicatingSocket::getForeignPort() {
    sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);
    
    if (getpeername(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
        perror("Fetch of foreign port failed (getpeername())");
    }
    return ntohs(addr.sin_port);
}

// TCPSocket Code

TCPSocket::TCPSocket() : CommunicatingSocket(SOCK_STREAM, IPPROTO_TCP) {}

TCPSocket::TCPSocket(const string &foreignAddress, unsigned short foreignPort) : CommunicatingSocket(SOCK_STREAM, IPPROTO_TCP) {
    connect(foreignAddress, foreignPort);
}

TCPSocket::TCPSocket(int newConnSD) : CommunicatingSocket(newConnSD) {
}

// TCPServerSocket Code

TCPServerSocket::TCPServerSocket(unsigned short localPort, int queueLen) : Socket(SOCK_STREAM, IPPROTO_TCP) {
    setLocalPort(localPort);
    setListen(queueLen);
}

TCPServerSocket::TCPServerSocket(const string &localAddress, unsigned short localPort, int queueLen) : Socket(SOCK_STREAM, IPPROTO_TCP) {
    setLocalAddressAndPort(localAddress, localPort);
    setListen(queueLen);
}

TCPSocket *TCPServerSocket::accept() {
    int newConnSD;
    if ((newConnSD = ::accept(sockDesc, NULL, 0)) < 0) {
        perror("Accept failed (accept())");
    }
    
    return new TCPSocket(newConnSD);
}

void TCPServerSocket::setListen(int queueLen) {
    if (listen(sockDesc, queueLen) < 0) {
        perror("Set listening socket failed (listen())");
    }
}
