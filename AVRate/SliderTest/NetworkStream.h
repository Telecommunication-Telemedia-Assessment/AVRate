//
//  NetworkStream.h
//  iOSAVRateServer
//
//  Created by Pierre on 16/03/13.
//  Copyright (c) 2013 T-Labs. All rights reserved.
//

#ifndef __iOSAVRateServer__NetworkStream__
#define __iOSAVRateServer__NetworkStream__

#include <iostream>



class NetworkStream {
    bool ready;
    int  sock;
    int  csock; // socket used to contact client in server mode.
    
    
public:
    NetworkStream(const std::string &ip, unsigned int port);
    NetworkStream(unsigned int port);
    ~NetworkStream();

    size_t read(char *buffer, size_t size);
    size_t write(const char *buffer, size_t size);
	size_t read(std::string &buffer);
    size_t write(const std::string &buffer);
    
};







#endif /* defined(__iOSAVRateServer__NetworkStream__) */
