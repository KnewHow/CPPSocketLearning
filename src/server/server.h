#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#include "socket.h"

namespace server_client {

class Server {
public:
    Server(const int port);
    bool Accept();
    bool Read(char *buffer, int *bufferLength) const ;
    bool Write(const char *buffer, const int bufferLength = 0) const;
    char *GetClientIP() const;
    void CloseClient();
    void Close();
    ~Server();
    const int port;

private:
    int listenfd;
    int clientfd;
    struct sockaddr_in server_addr, client_addr;
    static constexpr int CLISTEN_QUEUE_LEN = 10;
};

} // namespace server_client

#endif // SRC_SERVER_SERVER_H_