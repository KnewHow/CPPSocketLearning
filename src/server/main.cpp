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
#include "service/globalService.h"
#include "common/dbconfig.h"


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
bool registe(char *receiveBuffer, char *sendBuffer);
bool recharge(char *receiveBuffer, char *sendBuffer);
bool queryMoney(char *receiveBuffer, char *sendBuffer);
bool doTrade(char *receiveBuffer, char *sendBuffer);

Server *global_server = nullptr;

int main(int argc, char **argv) {
    // google::InitGoogleLogging(argv[0]);
    
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
    LOG(INFO) << "[" << pthread_self() << "] main thread work";
    DBConfig dbconfig("../config/db.properties");
    
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
    LOG(INFO) << "[" << pthread_self() << "] thread work";
    Request *req = (Request*)argc;
    doRequest(*req);
    delete req;
    LOG(INFO) << "[" << pthread_self() << "] thread exit";
    return nullptr;
}

void doRequest(const Request &req) {
    char receiveBuffer[4096], sendBuffer[4096];
    int receiveBufferLength;
    LOG(INFO) <<  "[" << pthread_self() <<"] thread deal request begin...";
    while(true) {
        memset(receiveBuffer, 0, sizeof(receiveBuffer));
        memset(sendBuffer, 0, sizeof(sendBuffer));
        if(TCPRead(req.clientfd, receiveBuffer, &receiveBufferLength)) {
            LOG(INFO) << "Request: " << std::string(receiveBuffer, receiveBufferLength);
            service(receiveBuffer, sendBuffer);
            if(TCPWrite(req.clientfd, sendBuffer)) { // send response to client
                    LOG(INFO) << "Response: " << std::string(sendBuffer);
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
    case 1: 
        return registe(receiveBuffer, sendBuffer);
    case 2:
        return login(receiveBuffer, sendBuffer);
    case 3: 
        return recharge(receiveBuffer, sendBuffer);
    case 4:
        return queryMoney(receiveBuffer, sendBuffer);
    case 5:
        return doTrade(receiveBuffer, sendBuffer);
    
    default: 
        LOG(INFO) << "Invalid bizcode: " << ibizcode;
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Invalid bizcode:</message>");
        return false;
    }
    
}

bool registe(char *receiveBuffer, char *sendBuffer) {
    char phone_number[51],password[51], username[51];
    int sex, age;
    memset(phone_number,0,sizeof(phone_number));
    memset(password,0,sizeof(password));
    memset(username,0,sizeof(username));
    GetXMLBuffer(receiveBuffer,"phone_number",phone_number,50);
    GetXMLBuffer(receiveBuffer,"password",password,50);
    GetXMLBuffer(receiveBuffer,"username",username,50);
    GetXMLBuffer(receiveBuffer,"sex",&sex);
    GetXMLBuffer(receiveBuffer,"age",&age);
    
    Customer customer(std::string(phone_number), std::string(password), sex, age, std::string(username));
    int r = globalService::customervice.insert(customer);
    if(r > 0) {
        sprintf(sendBuffer,"<retcode>0</retcode>\n<message>Register success</message>\n");
        return true;;
    } else {
        sprintf(sendBuffer,"<retcode>-1</retcode>\n<message>Register failure: System error!</message>\n");
        return false;
    }
}

bool recharge(char *receiveBuffer, char *sendBuffer) {
    char phone_number[51];
    long money;
    memset(phone_number,0,sizeof(phone_number));
    GetXMLBuffer(receiveBuffer,"phone_number",phone_number,50);
    GetXMLBuffer(receiveBuffer,"money",&money);
     std::optional<Customer> customer = globalService::customervice.selectByPhoneNumber(std::string(phone_number));
    if(!customer.has_value()) {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Phone number not exist!</message>");
        return false;
    }
    int r = globalService::accountServive.recharge(customer.value().id, money);
    if(r > 0) {
        sprintf(sendBuffer,"<retcode>0</retcode><message>Recharge success!</message>");
        return true;
    } else {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Recharge faliure,system error.</message>");
        return false;
    }
}
bool queryMoney(char *receiveBuffer, char *sendBuffer) {
    char phone_number[51];
    memset(phone_number,0,sizeof(phone_number));
    GetXMLBuffer(receiveBuffer,"phone_number",phone_number,50);
    std::optional<Customer> customer = globalService::customervice.selectByPhoneNumber(std::string(phone_number));
    if(!customer.has_value()) {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Phone number not exist!</message>");
        return false;
    }
    auto account = globalService::accountServive.selectByCustomerId(customer.value().id);
    if(!account.has_value()) {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Not create account!</message>");
        return false;
    } else {
        sprintf(sendBuffer,"<retcode>0</retcode>\
                            <message>Success</message>\
                            <money>%ld</money>", account.value().money);
        return true;
    }

}
bool doTrade(char *receiveBuffer, char *sendBuffer){
    char source_phone_number[51], dest_phone_number[51];
    long money;
    memset(source_phone_number,0,sizeof(source_phone_number));
    memset(dest_phone_number,0,sizeof(dest_phone_number));
    GetXMLBuffer(receiveBuffer,"source_phone_number",source_phone_number,50);
    GetXMLBuffer(receiveBuffer,"dest_phone_number",dest_phone_number,50);
    GetXMLBuffer(receiveBuffer,"money",&money);
    std::optional<Customer> source_customer = globalService::customervice.selectByPhoneNumber(std::string(source_phone_number));
    if(!source_customer.has_value()) {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Source phone_number not exist!</message>");
        return false;
    }

    std::optional<Customer> dest_customer = globalService::customervice.selectByPhoneNumber(std::string(dest_phone_number));
    if(!dest_customer.has_value()) {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Dest phone_number not exist!</message>");
        return false;
    }

    std::optional<Account> source_acc = globalService::accountServive.selectByCustomerId(source_customer.value().id);
    if(!source_acc.has_value()) {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Source account not exist</message>");
        return false;
    }

    std::optional<Account> dest_acc = globalService::accountServive.selectByCustomerId(dest_customer.value().id);
    if(!dest_acc.has_value()) {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>Dest account not exist</message>");
        return false;
    }
    bool r = globalService::accountServive.doTrade(source_customer.value().id, dest_customer.value().id, money);
    if(r) {
        sprintf(sendBuffer,"<retcode>0</retcode><message>Trade success</message>");
        return true;
    } else {
        sprintf(sendBuffer,"<retcode>-1</retcode><message>No enouth money to trade</message>");
        return false;
    }
}

bool login(char *receiveBuffer, char *sendBuffer) {
    char phone_number[51],password[51];
    memset(phone_number,0,sizeof(phone_number));
    memset(password,0,sizeof(password));

    GetXMLBuffer(receiveBuffer,"phone_number",phone_number,50);
    GetXMLBuffer(receiveBuffer,"password",password,50);

    std::optional<Customer> customer = globalService::customervice.login(std::string(phone_number), std::string(password));
    if(customer.has_value()) {
        Customer c = customer.value();
        sprintf(sendBuffer,"<retcode>0</retcode>\n\
                            <message>Success</message>\n\
                            <id>%ld</id>\n\
                            <phone_number>%s</phone_number>\n\
                            <username>%s</username>\n", c.id, c.phone_number, c.username);
        return true;
    } else {
        sprintf(sendBuffer,"<retcode>-1</retcode>\n<message>Username or password invalid</message>\n");
        return false;
    }
}