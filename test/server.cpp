#include <iostream>
#include <filesystem>
#include <signal.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#include <vector>

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

bool isShutdown = false;
std::vector<pthread_t> thread_ids;
std::vector<Request> requests;
pthread_mutex_t mutex;
pthread_cond_t cond;

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
    // FLAGS_logtostderr = true;


    if(argc != 2) {
        printf("Using ./sever prot\n Example: ./sever 5005\n");
        return EXIT_FAILURE;
    }
    LOG(INFO) << "[" << pthread_self() << "] main thread work";
    
    isShutdown = false;
    if(pthread_mutex_init(&mutex, nullptr) != 0) {
        perror("pthread_mutex_init");
        LOG(FATAL) << "init mutex failure";
    }
    if(pthread_cond_init(&cond, nullptr)!= 0) {
        perror("pthread_cond_init");
        LOG(FATAL) << "init cond failure";
    }

    int avaiable_cores = get_nprocs();
    thread_ids.clear();
    for(int i = 0; i < avaiable_cores - 1; ++i) { // we use avaiable_cores -1 to create child thread, remain one core to main thread
        pthread_t pid;
        pthread_create(&pid, nullptr, thread_func, nullptr);
        thread_ids.push_back(pid);
    }

    
    signal(SIGINT, shutdown_func); // parent exit signal
    signal(SIGTERM, shutdown_func);
    
    Server server(atoi(argv[1]));
    global_server = &server;
    while(true) {
        bool r = server.Accept();
        if(r) {
            LOG(INFO) << "Build connect from: " << std::string(server.GetClientIP());
            pthread_mutex_lock(&mutex);
            requests.push_back({server.GetClientFd()});
            pthread_mutex_unlock(&mutex);  
            pthread_cond_signal(&cond);
        }
    }
    
}


void shutdown_func(int sig) {
    isShutdown = true;
    pthread_cond_broadcast(&cond);
    for(int i = 0; i < thread_ids.size(); ++i) {
        pthread_join(thread_ids[i], nullptr);
    }
    if(global_server)
        global_server->CloseListen();
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    LOG(INFO) << "[" << pthread_self() << "] main thread exit";
    exit(0);
}

void* thread_func(void *argc) {
    LOG(INFO) << "[" << pthread_self() << "] thread work";
    Request req;
    pthread_mutex_lock(&mutex);
    while(!isShutdown) {
        if(requests.empty()) {
            pthread_cond_wait(&cond, &mutex);
        } else {
            memcpy(&req, &requests[0], sizeof(req));
            requests.erase(requests.begin());
            pthread_mutex_unlock(&mutex);
            doRequest(req);
            pthread_mutex_lock(&mutex);
        }
    }
    pthread_mutex_unlock(&mutex);
    LOG(INFO) << "[" << pthread_self() << "] thread exit";
    return nullptr;
}

void doRequest(const Request &req) {
    char receiveBuffer[1024], sendBuffer[1024];
    int receiveBufferLength;
    LOG(INFO) <<  "[" << pthread_self() <<"] thread deal request begin...";
    while(true) {
        memset(receiveBuffer, 0, sizeof(receiveBuffer));
        memset(sendBuffer, 0, sizeof(sendBuffer));
        if(TCPRead(req.clientfd, receiveBuffer, &receiveBufferLength)) {
            LOG(INFO) << "Request: " << std::string(receiveBuffer, receiveBufferLength);
            if(service(receiveBuffer, sendBuffer)) { // success deal service
                if(TCPWrite(req.clientfd, sendBuffer)) { // send response to client
                    LOG(INFO) << "Response: " << std::string(sendBuffer);
                } else {
                    break;
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }
    if(close(req.clientfd) == -1) {
        perror("close");
        LOG(ERROR) << "Close clientfd: [" << req.clientfd << "] failure";
    }
    LOG(INFO) <<  "[" << pthread_self() <<"] thread deal request end...";
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