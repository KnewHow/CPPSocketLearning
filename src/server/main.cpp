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

#include <sys/select.h>


#include "glog/logging.h"

static constexpr int CLISTEN_QUEUE_LEN = 10;
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

    fd_set read_fd_set;
    struct timeval tv;
    int max_fd = 0;
    tv.tv_sec = 5; // timeout is 5 seconds
    tv.tv_usec = 0;
    FD_ZERO(&read_fd_set);
    FD_SET(listenfd, &read_fd_set);
    max_fd = listenfd;
    while(1) {
        fd_set tmp_fd_set = read_fd_set;
        int in_fds = select(max_fd + 1, &tmp_fd_set, nullptr, nullptr, nullptr);
        if(in_fds < 0) {
            perror("select");
            LOG(FATAL) << "select method error!";
        } else if(in_fds == 0) {
            LOG(INFO) << "select timeout!";
        } else {
            for(int i = 0; i <= max_fd; ++i) {
                if(FD_ISSET(i, &tmp_fd_set) != 0) {
                    if(i == listenfd) { // new client connect
                        struct sockaddr_in client_addr;
                        socklen_t client_addr_len = sizeof(client_addr);
                        int client_fd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
                        if(client_fd < 0) {
                            LOG(INFO) << "Accept failure";
                        } else {
                            LOG(INFO) << "Accept new client, socket=" << client_fd;
                            FD_SET(client_fd, &read_fd_set); // add new client socket to listen socket set
                            if(max_fd < client_fd) 
                                max_fd = client_fd; // update max_fd   
                        }
                    } else { // client disconnect or send message
                        char buffer[1024];
                        memset(buffer, 0,sizeof(buffer));
                        ssize_t size = read(i, buffer, sizeof(buffer));
                        if(size <= 0) { // read finish or client disconnect
                            LOG(INFO) << "Client has disconnected";
                            close(i);
                            FD_CLR(i, &read_fd_set); // remove client_fd from read listen set
                            if(i == max_fd) { // if max_fd equal with removed i, recalculate max_fd
                                for(int j = max_fd; j > 0; --j) {
                                    if(FD_ISSET(j, &read_fd_set)) {
                                        max_fd = j;
                                        break;
                                    }
                                }
                            }
                        } else { // deal read data
                            //LOG(INFO) << "Receive message: " << std::string(buffer, size);
                            memset(buffer, 0, sizeof(buffer));
                            sprintf(buffer, "Success");
                            write(i, buffer, strlen(buffer));
                        }
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
