#include <iostream>
#include <filesystem>

#include "server/server.h"

using namespace server_client;

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    std::string log_dir = "/tmp/socket_server";
    bool log_dir_is_exsit = std::filesystem::exists(log_dir.c_str());
    if(!log_dir_is_exsit) {
        std::filesystem::create_directory(log_dir.c_str());
    }
    FLAGS_log_dir = log_dir.c_str();
    FLAGS_stderrthreshold = 1; // Warning and above.

    if(argc != 2) {
        printf("Using ./sever prot\n Example: ./sever 5005\n");
        return EXIT_FAILURE;
    }


    Server server(atoi(argv[1]));
    char buffer[1024];
    int bufferLength;
    memset(buffer, 0, sizeof(buffer));
    while(1) {
        bool r = server.Accept();
        if(r) {
            if(server.Read(buffer, &bufferLength)) {
                LOG(INFO) << "Receive: " << std::string(buffer, bufferLength);
                strcpy(buffer, "OK");
                if(server.Write(buffer)) {
                    LOG(INFO) << "Send responese to clinet: " << std::string(buffer);
                } else {
                    LOG(INFO) << "Send responese to client failure";
                }
            } else {
                LOG(INFO) << "Receive client request failure!";
            }
        }
    }
    return EXIT_SUCCESS;
}