#include <iostream>
#include <filesystem>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

#include <sys/epoll.h>

#include "glog/logging.h"

static constexpr int CLISTEN_QUEUE_LEN = 10;
static constexpr int MAX_EVENTS = 100;
void shutdown_func(int sig);

int port = -1;

int initListenSocket();


int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    
    FLAGS_logtostderr = true;
    // std::string log_dir = "/tmp/socket_server";
    // bool log_dir_is_exsit = std::filesystem::exists(log_dir.c_str());
    // if(!log_dir_is_exsit) {
    //     std::filesystem::create_directory(log_dir.c_str());
    // }
    // FLAGS_log_dir = log_dir.c_str();
    // FLAGS_stderrthreshold = 1; // Warning and above.


    if(argc != 2) {
        printf("Using ./sever prot\n Example: ./sever 5005\n");
        return EXIT_FAILURE;
    }
    port = atoi(argv[1]);
    signal(SIGINT, shutdown_func); // parent exit signal
    signal(SIGTERM, shutdown_func);

    int listenfd = initListenSocket();
    int epollfd = epoll_create(1);
    if(epollfd == -1) {
        perror("epoll_create");
        LOG(FATAL) << "create epoll failure";
    }
    struct epoll_event ev;
    ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
    while(1) {
        struct epoll_event events[MAX_EVENTS];
        int infds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if(infds < 0) {
            perror("epoll_wait");
            LOG(ERROR) << "epoll wait failure;";
            break;
        } else if(infds == 0) {
            LOG(INFO) << "epoll wait timeout";
        } else {
            for(int i = 0; i < infds; ++i) {
                if(events[i].data.fd == listenfd && (events[i].events & EPOLLIN)) { // new client connect
                    struct sockaddr_in client_addr;
                    socklen_t client_addr_len = sizeof(client_addr);
                    int client_fd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
                    if(client_fd < 0) {
                        LOG(INFO) << "Accept failure";
                    } else {
                        //LOG(INFO) << "Accept new client, socket=" << client_fd;
                        memset(&ev, 0, sizeof(ev));
                        ev.data.fd = client_fd;
                        ev.events = EPOLLIN;
                        epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev);
                    }
                } else if(events[i].events & EPOLLIN) { // client send data or disconnect
                    char buffer[1024];
                    memset(buffer, 0,sizeof(buffer));
                    ssize_t size = read(events[i].data.fd, buffer, sizeof(buffer));
                    if(size <= 0) { // read finish or client disconnect
                        //LOG(INFO) << "Client has disconnected: " << events[i].data.fd;
                        memset(&ev, 0, sizeof(ev));
                        ev.data.fd = events[i].data.fd;
                        ev.events = EPOLLIN;
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, ev.data.fd, &ev);
                        close(ev.data.fd);
                    } else { // deal read data
                        //LOG(INFO) << "Receive message: " << std::string(buffer, size);
                        memset(buffer, 0, sizeof(buffer));
                        sprintf(buffer, "Success");
                        write(events[i].data.fd, buffer, strlen(buffer));
                    }
                }
            }
        }

    }
    
}

int initListenSocket() {
    int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    ret = setsockopt(listenfd, SOL_SOCKET,SO_KEEPALIVE, &flag, sizeof(flag));
    if(ret == -1) {
        perror("setsockopt");
        LOG(FATAL) << "[server] setsockopt failure";
    }
    
    struct sockaddr_in server_addr;
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
    return listenfd;
}

void shutdown_func(int sig) {
    LOG(INFO) << "[" << pthread_self() << "] main thread exit";
    exit(0);
}
