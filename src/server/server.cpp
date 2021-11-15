#include "server.h"

namespace server_client {

Server::Server(int port): port(port) {
    clientfd = -1;
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenfd == -1) {
        perror("socket");
        LOG(FATAL) << "[server] listen socket failure";
    }
    
    int flag = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if(ret == -1) {
        perror("setsockopt");
        LOG(FATAL) << "[server] setsockopt failure";
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    ret = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        perror("bind");
        listenfd = -1;
        LOG(FATAL) << "[server] bind socket failure";
    }

    ret = listen(listenfd, CLISTEN_QUEUE_LEN);
    if(ret == -1) {
        perror("listen");
        close(listenfd);
        listenfd = -1;
        LOG(FATAL) << "[server] listen socket failure";
    }
}

bool Server::Accept() {
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_addr_len = sizeof(client_addr);
    clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if(clientfd == -1) {
        perror("accept");
        LOG(ERROR) << "[server] Accept client socket failure";
        return false;
    }
    return true;
}

bool Server::Write(const char *buffer, const int bufferLength) const {
    return TCPWrite(clientfd, buffer, bufferLength);
}

bool Server::Read(char *buffer, int *bufferLength) const {
    return TCPRead(clientfd, buffer, bufferLength);
}

char *Server::GetClientIP() const {
    return inet_ntoa(client_addr.sin_addr);
}

void Server::CloseClient() {
    if(clientfd != -1) {
        int ret = close(clientfd);
        if(ret == -1) {
            perror("close");
            LOG(ERROR) << "[server] Close clientfd failure";
        }
    }
    clientfd = -1;
}


void Server::CloseListen() {
    if(listenfd != -1) {
        int ret = close(listenfd);
        if(ret == -1) {
            perror("close");
            LOG(ERROR) << "[server] Close listenfd failure";
        }
    }
    listenfd = -1;
}

Server::~Server() {
    CloseClient();
    CloseListen();
}

} // namespace server_client