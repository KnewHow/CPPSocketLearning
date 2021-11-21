#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "client.h"
#include "xmlParser.h"

using namespace server_client;

std::string IP = "";
int port = 0;

void client_send(const char* sendBuffer, char *resultBuffer, int *resultBufferLen);
void service(char **argv);
void regist(const std::string &phone_number, const std::string &username, const std::string &pass, int sex, int age);
void login(const std::string &phone_number, const std::string &pass);
void recharge(const std::string &phone_number, long money);
void queryMoney(const std::string &phone_number);
void doTrade(const std::string &source_phone, const std::string &dest_phone, long money);

int main(int argc, char **argv) {
    // google::InitGoogleLogging(argv[0]);
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

    service(argv);
    
}

void service(char **argv) {
    int bizcode = atoi(argv[3]);
    switch (bizcode)
    {
    case 1:
        regist(std::string(argv[4]), std::string(argv[5]),std::string(argv[6]), atoi(argv[7]), atoi(argv[8]));
        break;
    case 2:
        login(std::string(argv[4]), std::string(argv[5]));
        break;
    case 3:
        recharge(std::string(argv[4]), atol(argv[5]));
        break;
    case 4:
        queryMoney(std::string(argv[4]));
        break;
    case 5:
        doTrade(std::string(argv[4]), std::string(argv[5]), atol(argv[6]));
        break;
    default:
        LOG(ERROR) << "Invalid bizcode: " << bizcode;
        break;
    }
}

void client_send(const char* sendBuffer, char *resultBuffer, int *resultBufferLen) {
    if(IP == "" || port == 0) {
        LOG(ERROR) << "Not init IP or port!";
        return;
    }
    Client client(IP, port);
    client.Connect();
    if(client.Write(sendBuffer)) {
        LOG(INFO) << "Send message to server success: " << std::string(sendBuffer);
        if(client.Read(resultBuffer, resultBufferLen)) {
            LOG(INFO) << "Receive server responese success: " << std::string(resultBuffer, *resultBufferLen);
        } else {
            LOG(INFO) << "Receive server response failure!";
        }
    } else {
        LOG(INFO) << "send message to server failure!";
    }
}

void regist(const std::string &phone_number, const std::string &username, const std::string &pass, int sex, int age) {
    char buffer[4096];
    char result[4096];
    int resultLen;
    memset(buffer,0,sizeof(buffer));
    memset(result, 0, sizeof(result));
    snprintf(buffer,1000,"<bizcode>1</bizcode>\n \
                          <phone_number>%s</phone_number>\n \
                          <password>%s</password>\n \
                          <username>%s</username>\n \
                          <sex>%d</sex>\n \
                          <age>%d</age>\n", phone_number.c_str(), pass.c_str(), username.c_str(), sex, age);

    client_send(buffer, result, &resultLen);

}
void login(const std::string &phone_number, const std::string &pass) {
    char buffer[4096];
    char result[4096];
    int resultLen;
    memset(buffer,0,sizeof(buffer));
    memset(result, 0, sizeof(result));
    snprintf(buffer,1000,"<bizcode>2</bizcode>\n\
                          <phone_number>%s</phone_number>\n \
                          <password>%s</password>\n", phone_number.c_str(), pass.c_str());

    client_send(buffer, result, &resultLen);
}
void recharge(const std::string &phone_number, long money) {
    char buffer[4096];
    char result[4096];
    int resultLen;
    memset(buffer,0,sizeof(buffer));
    memset(result, 0, sizeof(result));
    snprintf(buffer,1000,"<bizcode>3</bizcode>\n\
                          <phone_number>%s</phone_number>\n \
                          <money>%ld</money>\n", phone_number.c_str(), money);

    client_send(buffer, result, &resultLen);
}
void queryMoney(const std::string &phone_number) {
    char buffer[4096];
    char result[4096];
    int resultLen;
    memset(buffer,0,sizeof(buffer));
    memset(result, 0, sizeof(result));
    snprintf(buffer,1000,"<bizcode>4</bizcode>\n\
                          <phone_number>%s</phone_number>\n", phone_number.c_str());

    client_send(buffer, result, &resultLen);
}

void doTrade(const std::string &source_phone, const std::string &dest_phone, long money) {
    char buffer[4096];
    char result[4096];
    int resultLen;
    memset(buffer,0,sizeof(buffer));
    memset(result, 0, sizeof(result));
    snprintf(buffer,1000,"<bizcode>5</bizcode>\n\
                          <source_phone_number>%s</source_phone_number>\n \
                          <dest_phone_number>%s</dest_phone_number>\n \
                          <money>%ld</money>", source_phone.c_str(), dest_phone.c_str(), money);

    client_send(buffer, result, &resultLen);
}
