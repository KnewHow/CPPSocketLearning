#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {
    if(argc != 2) 
        printf("Using ./server prot.\n Example: ./server 5005.\n");

    // [1] Creating server socket
    int listenfd;
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("[server] create listen socket failure!\n");
        return -1;
    }

    // [2] Bind ing server IP and port to listen socket
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    if(bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        printf("[server] bind listen failure!\n");
        close(listenfd);
        return -1;
    }

    // [3] Let socket keep listening
    if(listen(listenfd, 3) != 0) {
        printf("[server] socket listen failure!\n");
    }

    // [4] Waiting client connect
    int clientfd;
    int socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    clientfd = accept(listenfd, (struct sockaddr *)&client_addr, (socklen_t *)&socklen);
    printf("Client(%s) has connected\n", inet_ntoa(client_addr.sin_addr));

    // [5] Communication with client.
    char buffer[1024];
    while(1) {
        int iret;
        memset(buffer, 0, sizeof(buffer)); 
        if((iret = recv(clientfd, &buffer, sizeof(buffer), 0)) <= 0) { // Receive message from client
            printf("[server] receive iret=%d\n", iret);
            break;
        }
        printf("Receive Message: %s\n", buffer);

        strcpy(buffer, "OK!");
        if((iret = send(clientfd, &buffer, strlen(buffer), 0)) <= 0) {
            printf("[server] send iret=%d\n", iret);
            break;
        }
        printf("Send: %s\n", buffer);
    }
    
    // [6] Close socket
    close(clientfd);
    close(listenfd);

    return 0;
}