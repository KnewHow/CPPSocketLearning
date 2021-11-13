#include "socket.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

namespace server_client {

bool TCPWrite(const int sockfd, const char *data, const int dataLength) {
    int length = 0;
    if(dataLength == 0) length = strlen(data);
    else                length = dataLength;

    int net_length = htonl(dataLength);
    char buffer[net_length + 4];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &net_length, 4);
    memcpy(buffer + 4, data, length);
    return write(sockfd, buffer, net_length);
}

bool TCPRead(const int sockfd, char *data, int *dataLength) {
    *dataLength = 0;
    bool ret;
    if(!read(sockfd, (char*)dataLength, 4)) return false;
    (*dataLength) = ntohl(*dataLength);
    return read(sockfd, data, *dataLength);
}

bool read(const int sockfd, char *data, const size_t length) {
    int left = length, idx = 0, ret;
    while(left > 0) {
        ret = recv(sockfd, data + idx, left, 0);
        if(ret == -1) {
            perror("recv");
            return false;
        }
        left -= ret;
        idx += ret;
    }
    return true;
}

bool write(const int sockfd, const char *data, const size_t length) {
    int left = length, idx = 0, ret;
    while(left > 0) {
        ret = send(sockfd, data + idx, left, 0);
        if(ret == -1) {
            perror("send");
            return false;
        }
        left -= ret;
        idx += ret;
    }
    return true;
}


} // namespace server_client

