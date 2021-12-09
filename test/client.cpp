#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "client/client.h"
#include <chrono>

using namespace server_client;

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
    
    if(argc != 3) {
        printf("Using ./clinet IP port\n Example: ./client 127.0.0.1 5005\n");
        return EXIT_FAILURE;
    }
    //FLAGS_logtostderr = true;
    std::chrono::milliseconds begin =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    Client client(std::string(argv[1]), atoi(argv[2]));
    client.Connect();
    char buffer[1024];
    int bufferLength = 0;
    memset(buffer,0,sizeof(buffer));
    snprintf(buffer,1000,"I am request from client");
    if(client.Write(buffer)) {
        if(client.Read(buffer, &bufferLength)) {
            std::chrono::milliseconds end =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
            LOG(INFO) << "receive server message:  " << std::string(buffer, bufferLength) << ", took: " << (end-begin).count() << " ms";
        } else {
            LOG(INFO) << "Receive server response failure!";
        }
    } else {
        LOG(INFO) << "send message to server failure!";
    }
    
}