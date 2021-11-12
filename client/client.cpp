#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char** argv) {
    if(argc != 3) {
        printf("Using ./client ip port\n Example: ./client 127.0.0.1 5005\n");
        return -1;
    }

    // [1] Create Socket
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("[client] create socket failure!\n");
        return -1;
    }

    // [2] Send request to server
    struct hostent *h;
    if((h = gethostbyname(argv[1])) == 0) {
        printf("[client] get hostname failure\n");
        return -1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port=htons(atoi(argv[2]));
    memcpy(&server_addr.sin_addr, h->h_addr, h->h_length);
    if(connect(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        printf("[client] connect server failure\n");
        return -1;
    }

    // [3] Communication with server
    char buffer[1024];
    for(int i = 0; i < 3; ++i) {
        int iret;
        memset(&buffer, 0, sizeof(buffer));
        sprintf(buffer, "This is %ith request data", i);
        if((iret = send(sockfd, buffer, strlen(buffer), 0)) <= 0) {
            printf("iret=%d\n", iret);
            break;
        }
        printf("Send: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
        if((iret = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0) {
            printf("iret=%d\n", iret);
            break;    
        }
        printf("Receive: %s\n", buffer);
    }


    // [4] close socket
    close(sockfd);
    return 0;
}