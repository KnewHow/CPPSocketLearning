#include <iostream>
#include <filesystem>
#include <signal.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#include <vector>
#include <unordered_map>

#include "server/server.h"
#include "socket.h"
#include "xmlParser.h"


using namespace server_client;

struct Request {
    Request(){
        clientfd = -1;
    }
    Request(int fd): clientfd(fd){}
    int clientfd;
};

void shutdown_func(int sig);
void* thread_func(void *argc);
void doRequest(const Request &req);

bool service(char *receiveBuffer, char *sendBuffer);
bool login(char *receiveBuffer, char *sendBuffer);

Server *global_server = nullptr;

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    std::string log_dir = "/tmp/socket_server";
    bool log_dir_is_exsit = std::filesystem::exists(log_dir.c_str());
    if(!log_dir_is_exsit) {
        std::filesystem::create_directory(log_dir.c_str());
    }
    FLAGS_log_dir = log_dir.c_str();
    FLAGS_stderrthreshold = 1; // Warning and above.
    //FLAGS_logtostderr = true;


    if(argc != 2) {
        printf("Using ./sever prot\n Example: ./sever 5005\n");
        return EXIT_FAILURE;
    }
    LOG(INFO) << "[" << pthread_self() << "] main thread work";

    
    signal(SIGINT, shutdown_func); // parent exit signal
    signal(SIGTERM, shutdown_func);
    
    Server server(atoi(argv[1]));
    global_server = &server;
    while(true) {
        bool r = server.Accept();
        if(r) {
            LOG(INFO) << "Build connect from: " << std::string(server.GetClientIP());
            pthread_t pid;
            Request *req = new Request(server.GetClientFd());
            pthread_create(&pid, nullptr, thread_func, (void *)req);
        }
    }
    
}

void shutdown_func(int sig) {
    global_server->CloseListen();
    LOG(INFO) << "[" << pthread_self() << "] main thread exit";
    exit(0);
}

void* thread_func(void *argc) {
    Request *req = (Request*)argc;
    doRequest(*req);
    delete req;
    return nullptr;
}

void doRequest(const Request &req) {
    char receiveBuffer[1024], sendBuffer[1024];
    int receiveBufferLength;
    memset(receiveBuffer, 0, sizeof(receiveBuffer));
    memset(sendBuffer, 0, sizeof(sendBuffer));
    if(TCPRead(req.clientfd, receiveBuffer, &receiveBufferLength)) {
        LOG(INFO) << "Request: " << std::string(receiveBuffer, receiveBufferLength);
        sprintf(sendBuffer,"Success");
        TCPWrite(req.clientfd, sendBuffer);
    } else {
        LOG(ERROR) << "Read data from client error";
    }
    if(close(req.clientfd) == -1) {
        perror("close");
        LOG(ERROR) << "Close clientfd: [" << req.clientfd << "] failure";
    }
}

bool service(char *receiveBuffer, char *sendBuffer) {
    int ibizcode=-1;
    GetXMLBuffer(receiveBuffer,"bizcode",&ibizcode);
    switch (ibizcode)
    {
    case 1: // 身份认证
        return login(receiveBuffer, sendBuffer);
    
    default: 
        LOG(INFO) << "Invalid bizcode: " << ibizcode;
        return false;
    }
    
}

bool login(char *receiveBuffer, char *sendBuffer) {
    char username[51],password[51];
    memset(username,0,sizeof(username));
    memset(password,0,sizeof(password));

    GetXMLBuffer(receiveBuffer,"username",username,50);
    GetXMLBuffer(receiveBuffer,"password",password,50);

    if ((strcmp(username,"Knewhow")==0) && (strcmp(password,"123")==0) )
        sprintf(sendBuffer,"<retcode>0</retcode><message>成功。</message>");
    else
        sprintf(sendBuffer,"<retcode>-1</retcode><message>用户名或密码不正确。</message>");

    return true;
}