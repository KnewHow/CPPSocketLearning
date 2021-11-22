#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "client.h"
#include "xmlParser.h"
#include "common/timeUtil.h"

using namespace server_client;

std::string IP = "";
int port = 0;


int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    // std::string log_dir = "/tmp/socket_client";
    // bool log_dir_is_exsit = std::filesystem::exists(log_dir.c_str());
    // if(!log_dir_is_exsit) {
    //     std::filesystem::create_directory(log_dir.c_str());
    // }
    // FLAGS_log_dir = log_dir.c_str();
    // FLAGS_stderrthreshold = 1; // Warning and above.
    FLAGS_logtostderr = true;
    
    if(argc < 3) {
        printf("Using ./clinet IP port\n Example: ./client 127.0.0.1 5005\n");
        return EXIT_FAILURE;
    }
    IP = std::string(argv[1]);
    port = atoi(argv[2]);

    
}