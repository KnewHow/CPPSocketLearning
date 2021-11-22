#include <iostream>
#include <filesystem>
#include <signal.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#include <vector>
#include <unordered_map>

#include "server.h"
#include "socket/socket.h"
#include "xmlParser.h"
#include "common/dbconfig.h"


using namespace server_client;
Server *global_server = nullptr;
void shutdown_func(int sig);






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
    
    signal(SIGINT, shutdown_func); // parent exit signal
    signal(SIGTERM, shutdown_func);
    
}

void shutdown_func(int sig) {
    global_server->CloseListen();
    LOG(INFO) << "[" << pthread_self() << "] main thread exit";
    exit(0);
}
