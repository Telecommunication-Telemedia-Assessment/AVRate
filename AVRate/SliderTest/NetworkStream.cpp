//
//  NetworkStream.cpp
//  iOSAVRateServer
//
//  Created by Pierre on 16/03/13.
//  Copyright (c) 2013 T-Labs. All rights reserved.
//

#include "NetworkStream.h"

#if defined (WIN32) || defined (_WIN32)

    #include <winsock2.h>
	#include <Ws2tcpip.h>

#elif defined (linux) || defined (_POSIX_VERSION) || defined (_POSIX2_C_VERSION)\
|| defined (_XOPEN_VERSION)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define closesocket(s) close (s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#else
#error not defined for this platform
#endif



#include <stdio.h>
#include <stdlib.h>



// ------------------------------------------------------------------------------------------


NetworkStream::NetworkStream(const std::string &name, unsigned int port) {
    int ret;
    
#if defined (WIN32) || defined (_WIN32)
    WSADATA wsa_data;
    int err = WSAStartup (MAKEWORD (2, 2), &wsa_data);
    
    if (err) {
        std::cerr << "Could not initialize network";
    }
#endif
    
    sock = INVALID_SOCKET;
    csock = INVALID_SOCKET;
    ready = false;
    
    
    csock = socket (AF_INET, SOCK_STREAM, 0);


    if (csock != INVALID_SOCKET) {
        
        /* assign the listening port */
        SOCKADDR_IN sin = {0};
            
        /* protocol family (IP) */
        sin.sin_family = AF_INET;
            
        /* listening port */
        sin.sin_port = htons (port);

        if(inet_pton(AF_INET, name.c_str(), &sin.sin_addr.s_addr) <= 0) {
            std::cerr << "Could not get IP\n";
        }
            
        /* bind */
        //sock_err = bind (sock, (SOCKADDR *) &sin, sizeof sin);

        /* connect */
        ret = connect(csock, (SOCKADDR *) &sin, sizeof sin);

        if(ret == 0) {
            ready = true;
            std::cout << "connection established\n";
        } else 
            std::cerr << "Could not connect \n";
    }   else {
        std::cerr << "INVALID_SOCKET\n";
    }
}




NetworkStream::NetworkStream(unsigned int port) {
#if defined (WIN32) || defined (_WIN32)
    WSADATA wsa_data;
    int err = WSAStartup (MAKEWORD (2, 2), &wsa_data);
    
    if (err) {
        std::cerr << "Could not initialize network";
        ready = false;
    }
#endif
    
    sock = INVALID_SOCKET;
    csock = INVALID_SOCKET;
    ready = false;
    
    
    sock = socket (AF_INET, SOCK_STREAM, 0);
    
    if (sock != INVALID_SOCKET) {
        int sock_err;
        
        /* assign the listening port */
        SOCKADDR_IN sin = {0};
            
        /* automatic IP address */
        sin.sin_addr.s_addr = htonl (INADDR_ANY);
            
        /* protocol family (IP) */
        sin.sin_family = AF_INET;
            
        /* listening port */
        sin.sin_port = htons (port);
            
        /* bind */
        sock_err = bind (sock, (SOCKADDR *) &sin, sizeof sin);
            
        if (sock_err != SOCKET_ERROR) {
            /* start listening (server mode) */
            sock_err = listen (sock, 5);
            
            if (sock_err != SOCKET_ERROR) {
                SOCKADDR_IN csin = {0};
                int recsize = (int) sizeof csin;
                csock = accept (sock, (SOCKADDR *) & csin, reinterpret_cast<socklen_t*>(&recsize));
                
                if (csock != INVALID_SOCKET) {
                    std::cout << "connection established\n";
                    ready = true;
 
                }
            } else {
                std::cerr << "listening mode failed\n";
            }
        } else {
            std::cerr << "Bind failed\n";
        }
    } else {
        std::cerr << "INVALID_SOCKET\n";
    }
}


size_t NetworkStream::read(char *buffer, size_t size) {
    if(!ready)
        return 0;

    return recv(csock, buffer, size, 0);
}


size_t NetworkStream::write(const char *buffer, size_t size) {
    if(!ready)
        return 0;

    return send(csock, buffer, size, 0);

}

size_t NetworkStream::read(std::string &buffer) {
    if(!ready)
        return 0;

    std::string line = "                                     ";

    bool loop = true;
    int prevSize = 0;
    while(loop) {
        int ret = recv(csock, &line[0], line.size(), 0);

        if(ret != line.size())
            loop = false;

        if(ret > 0) {

            buffer.resize(ret+prevSize);        
            for(int i = 0 ; i < ret ; ++i) {
                buffer[i+prevSize] = line[i];
            }

            prevSize = buffer.size();
        }
    }

    return buffer.size();
}


size_t NetworkStream::write(const std::string &buffer) {
    if(!ready)
        return 0;

    return send(csock, &buffer[0], buffer.size(), 0);
}



NetworkStream::~NetworkStream() {
    
    /* close the socket. */
    if(csock != INVALID_SOCKET) {
        shutdown (csock, 2);
        closesocket (csock), csock = INVALID_SOCKET;
    }
    if(sock != INVALID_SOCKET) {
        shutdown (csock, 2);
        closesocket (sock), sock = INVALID_SOCKET;
    }
    
    
#if defined (WIN32) || defined (_WIN32)
    WSACleanup ();
#endif
    
}


