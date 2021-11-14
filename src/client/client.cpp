#include "client.h"

namespace server_client {

Client::Client(const std::string &server_IP, const int server_port)
    :server_IP(server_IP), server_port(server_port) {
    sockfd = -1;
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1) {
        perror("socket");
        LOG(FATAL) << "[client] create socket failure";
    }

    struct hostent *h = nullptr;
    char ip[server_IP.length() + 1];
    strcpy(ip, server_IP.c_str());
    h = gethostbyname(ip);
    if(h == nullptr) {
        perror("gethostbyname");
        LOG(FATAL) << "[client] gethostbyname failure";
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port=htons(server_port);
    memcpy(&server_addr.sin_addr, h->h_addr, h->h_length);
}

bool Client::Connect() const {
    int ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        perror("connect");
        LOG(ERROR) << "[client] connect failure";
        return false;
    }
    return true;
}

bool Client::Write(const char* buffer, const int bufferLength) const {
    return TCPWrite(sockfd, buffer, bufferLength);
}

bool Client::Read(char *buffer, int *bufferLength) const {
    return TCPRead(sockfd, buffer, bufferLength);
}

void Client::Close() {
    if(sockfd != -1) {
        int ret = close(sockfd);
        if(ret == -1) {
            perror("close");
            LOG(ERROR) << "[client] close socket error";
        }
    }
    sockfd = -1;
}

Client::~Client() {
    Close();
}

} // namespace server_client