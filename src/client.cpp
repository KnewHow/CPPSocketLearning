#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    int sock_fd = -1;
    struct sockaddr_in6 server_addr;
    int ret;
    char ch = 'a';

    if(argc != 3) {
        printf("Using ./server IP port\n Example: ./client 127.0.0.1 5005");
        return EXIT_FAILURE;
    }

    sock_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if(sock_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &server_addr.sin6_addr);
    server_addr.sin6_port = htons(atoi(argv[2]));

    ret = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        perror("connect");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    ret = write(sock_fd, &ch, sizeof(ch));
    if(ret == -1) {
        perror("write");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    ret = read(sock_fd, &ch, sizeof(ch));
    if(ret == -1) {
        perror("read");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    printf("Receive: %c from client\n", ch); 

    ret = close(sock_fd);
    if(ret == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}