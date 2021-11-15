#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "client/client.h"

using namespace server_client;

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    std::string log_dir = "/tmp/socket_client";
    bool log_dir_is_exsit = std::filesystem::exists(log_dir.c_str());
    if(!log_dir_is_exsit) {
        std::filesystem::create_directory(log_dir.c_str());
    }
    FLAGS_log_dir = log_dir.c_str();
    FLAGS_stderrthreshold = 1; // Warning and above.
    
    if(argc != 3) {
        printf("Using ./clinet IP port\n Example: ./client 127.0.0.1 5005\n");
        return EXIT_FAILURE;
    }
    //FLAGS_logtostderr = true;

    Client client(std::string(argv[1]), atoi(argv[2]));
    client.Connect();
    char buffer[1024];
    int bufferLength = 0;
    for(int i = 0; i < 30; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "[%d] This is %dth request, ID is: %d", getpid(), i + 1, i + 1);
        if(client.Write(buffer)) {
            LOG(INFO) << "Send message to server success: " << std::string(buffer);
            if(client.Read(buffer, &bufferLength)) {
                LOG(INFO) << "Receive server responese success: " << std::string(buffer, bufferLength);
            } else {
                LOG(INFO) << "Receive server response failure!";
            }
        } else {
            LOG(INFO) << "send message to server failure!";
        }
        sleep(1);
    }
    client.Close();
}