#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "client.h"
#include "xmlParser.h"
#include "common/timeUtil.h"
#include "socket/socket.h"

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
    int number = atoi(argv[3]);

    int sockfd;
    struct sockaddr_in servaddr;
    char buf[1024];
    
    if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) { printf("socket() failed.\n"); return -1; }
	
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)
    {
        printf("connect(%s:%s) failed.\n",argv[1],argv[2]); close(sockfd);  return -1;
    }

    std::chrono::milliseconds begin = getCurrentMillseconds();
    int messageSize = 1;
    for (int i = 0; i < messageSize; ++i)
    {
        memset(buf,0,sizeof(buf));
        // 从命令行输入内容。
        //printf("please input:"); scanf("%s",buf);
        sprintf(buf,"This is %d th message", i);

        if (write(sockfd,buf,strlen(buf)) <=0)
        { 
        printf("write() failed.\n");  
        close(sockfd);  
        return -1;
        }
            
        memset(buf,0,sizeof(buf));
        int ret = read(sockfd,buf,sizeof(buf));
        if (ret == 0) 
        { 
        close(sockfd);  
        return 0;
        } else if(ret < 0) {
            printf("read() failed.\n");  
            return -1;
        }
        //printf("recv:%s\n",buf);
    }
    std::chrono::milliseconds end = getCurrentMillseconds();
    LOG(INFO) << "[" << number << "]  took: " << (end - begin).count() << " ms";
}