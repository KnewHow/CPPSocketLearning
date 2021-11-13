#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define CLISTEN_QUEUE_LEN 10

int main(int argc, char **argv) {

    int listen_sock_fd = -1, client_sock_fd = -1;
    struct sockaddr_in6 server_addr, client_addr;
    socklen_t client_addr_len;
    char str_addr[INET6_ADDRSTRLEN];
    int flag, ret;
    char ch;

    if(argc != 2) {
        printf("Using ./server port\n Example: ./server 5005\n");
        return EXIT_FAILURE;
    }

    // Create socket to listen
    listen_sock_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if(listen_sock_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set socket to reuse addresss
    flag = 1;
    ret = setsockopt(listen_sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if(ret == -1) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(atoi(argv[1]));

    ret = bind(listen_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1) {
        perror("bind");
        close(listen_sock_fd);
        return EXIT_FAILURE;
    }

    ret = listen(listen_sock_fd, CLISTEN_QUEUE_LEN);
    if(ret == -1) {
        perror("listen");
        close(listen_sock_fd);
        return EXIT_FAILURE;
    }

    client_addr_len = sizeof(client_addr);

    while(1) {
        client_sock_fd = accept(listen_sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if(client_sock_fd == -1) {
            perror("accept");
            close(listen_sock_fd);
            return EXIT_FAILURE;
        }

        inet_ntop(AF_INET6, &client_addr.sin6_addr, str_addr, sizeof(str_addr));
        printf("New connection from: %s:%d\n", str_addr, ntohs(client_addr.sin6_port));

        ret = read(client_sock_fd, &ch, sizeof(ch));
        if(ret == -1) {
            perror("read");
            close(client_sock_fd);
            continue;
        }

        // do something deal to rece data
        printf("Receive %c from client\n", ch);
        ch++;
        
        ret = write(client_sock_fd, &ch, sizeof(ch));
        if(ret == -1) {
            perror("write");
            close(client_sock_fd);
            continue;
        }
        
        ret = close(client_sock_fd);
        if(ret == -1) {
            perror("close");
            client_sock_fd = -1;
        }

        printf("Connection closed\n");

    }

    close(listen_sock_fd);
    return EXIT_SUCCESS;

}