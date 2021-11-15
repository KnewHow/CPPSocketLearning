#include <iostream>
#include <filesystem>
#include <signal.h>

#include "server/server.h"


using namespace server_client;

Server *parent_process_server = nullptr;
Server *child_process_sever = nullptr;

void parentProcessExit(int sig);
void childProcessExit(int sig);

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    // std::string log_dir = "/tmp/socket_server";
    // bool log_dir_is_exsit = std::filesystem::exists(log_dir.c_str());
    // if(!log_dir_is_exsit) {
    //     std::filesystem::create_directory(log_dir.c_str());
    // }
    // FLAGS_log_dir = log_dir.c_str();
    // FLAGS_stderrthreshold = 1; // Warning and above.
    FLAGS_logtostderr = true;

    if(argc != 2) {
        printf("Using ./sever prot\n Example: ./sever 5005\n");
        return EXIT_FAILURE;
    }
    signal(SIGCHLD, SIG_IGN); // ignore child process exit, avoid defunct process
    
    signal(SIGINT, parentProcessExit); // parent exit signal
    signal(SIGTERM, parentProcessExit);
    
    Server server(atoi(argv[1]));
    parent_process_server = &server;
    char buffer[1024];
    int bufferLength;
    memset(buffer, 0, sizeof(buffer));
    while(1) {
        bool r = server.Accept();
        int pid = fork();
        if(pid > 0) {// let parent process go on listening and child process deal request.
            server.CloseClient();// because parent process don't deal request, so parent process need to close the client socket
            continue;
        }
        
        signal(SIGINT, childProcessExit); // child exit signal
        signal(SIGTERM, childProcessExit);
        server.CloseListen(); // for child process, don't need to listen, close it.
        child_process_sever = &server;
        if(r) {
            LOG(INFO) << "Build connect from: " << std::string(server.GetClientIP());
            while(true) {
                if(server.Read(buffer, &bufferLength)) {
                    LOG(INFO) << "[" << getpid() << "] " << "Receive: " << std::string(buffer, bufferLength);
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, "OK");
                    r = server.Write(buffer);
                    if(r) {
                        LOG(INFO) << "Send responese to clinet: " << std::string(buffer);
                    } else {
                        break;
                    }
                } else {
                    break;
                }
                google::FlushLogFiles(google::INFO);
            }
        }
        server.CloseClient();
        LOG(INFO) << "Close client connect";
        google::FlushLogFiles(google::INFO);
        return EXIT_SUCCESS; // exit child process
    }
    return EXIT_SUCCESS;
}


void parentProcessExit(int sig) {
    if(sig > 0) {
        signal(sig, SIG_IGN);
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, SIG_IGN); // avoid multiply exit
    }
    kill(0, SIGTERM); // notify child process exit
    if(parent_process_server)
        parent_process_server->~Server();
    printf("[%d] Parent process has exited\n", getpid());
    exit(0);
}

void childProcessExit(int sig) {
    if(sig > 0) {
        signal(sig, SIG_IGN);
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, SIG_IGN); // avoid multiply exit
    }
    if(child_process_sever)
        child_process_sever->~Server();
    printf("[%d] Child process has exited\n", getpid());
    exit(0);
}