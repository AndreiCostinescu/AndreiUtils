//
// Created by ga78cat on 11.03.2021.
//

#ifndef ANDREIUTILS_NETWORKINCLUDES_H
#define ANDREIUTILS_NETWORKINCLUDES_H

#include <cerrno>
#include <unistd.h>

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>

typedef int SocketAddressLength;

#define SOCKET_TIMEOUT WSAETIMEDOUT
#define SOCKET_CONNRESET WSAECONNRESET
#define SOCKET_AGAIN WSATRY_AGAIN
#define SOCKET_WOULDBLOCK WSAEWOULDBLOCK
#define SOCKET_SHUTDOWN WSAESHUTDOWN
#define SOCKET_PIPE WSAESHUTDOWN

#define SIGKILL 9
#define SIGPIPE 13

#else

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string.h>

/*
#define	EADDRINUSE      98	// Address already in use
#define	EADDRNOTAVAIL   99	// Cannot assign requested address
#define	ENETDOWN       100	// Network is down
#define	ENETUNREACH	   101	// Network is unreachable
#define	ENETRESET	   102	// Network dropped connection because of reset
#define	ECONNABORTED   103	// Software caused connection abort
#define	ECONNRESET	   104	// Connection reset by peer
#define	ENOBUFS		   105	// No buffer space available
#define	EISCONN		   106	// Transport endpoint is already connected
#define	ENOTCONN	   107	// Transport endpoint is not connected
#define	ESHUTDOWN	   108	// Cannot send after transport endpoint shutdown
#define	ETOOMANYREFS   109	// Too many references: cannot splice
#define	ETIMEDOUT      110	// Connection timed out
#define	ECONNREFUSED   111	// Connection refused
#define	EHOSTDOWN      112	// Host is down
#define	EHOSTUNREACH   113	// No route to host
*/

typedef int SOCKET;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (SOCKET)(-1)
#endif
#define SOCKET_ERROR -1
#define SOCKET_TIMEOUT ETIMEDOUT
#define SOCKET_CONNRESET ECONNRESET
#define SOCKET_AGAIN EAGAIN
#define SOCKET_WOULDBLOCK EWOULDBLOCK
#define SOCKET_SHUTDOWN ESHUTDOWN
#define SOCKET_PIPE EPIPE

typedef socklen_t SocketAddressLength;

#endif

typedef struct sockaddr_in SocketAddress;

#include <iostream>

extern bool socketsStarted;

#endif //ANDREIUTILS_NETWORKINCLUDES_H
