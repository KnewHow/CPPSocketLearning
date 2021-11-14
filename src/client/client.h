#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_CLIENT_CLIENT_H_
#define SRC_CLIENT_CLIENT_H_

#include <string>

#include "socket.h"


namespace server_client {
class Client {
public:
    Client(const std::string &server_IP, const int server_port);
    ~Client();
    bool Connect() const;
    bool Write(const char *buffer, const int bufferLength = 0) const;
    bool Read(char *buffer, int *bufferLength) const;
    void Close();
    
    const std::string server_IP;
    const int server_port;

private:
    int sockfd;
    struct sockaddr_in server_addr;

};

} // namespace server_client

#endif // SRC_CLIENT_CLIENT_H_