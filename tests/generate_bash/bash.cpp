#include "gtest/gtest.h"
#include <glog/logging.h>
#include <stdio.h>
#include <string.h>

char IP[16] = "123.60.46.246";

TEST(Bash, Regist) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    long phone_number = 18326600000;
    for(int i = 0; i < 1000; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client %s 5005 1 '%ld' 'Knewhow' '123456' 1 25  >> /tmp/socket_client/client.log 2>&1 &\n", IP, ++phone_number);
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, Login) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    long phone_number = 18326600000;
    for(int i = 0; i < 500; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client %s 5005 2 '%ld' '123456' >> /tmp/socket_client/client.log 2>&1 &\n", IP, ++phone_number);
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, Recharge) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    long phone_number = 18326600000;
    for(int i = 0; i < 500; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client %s 5005 3 '%ld' '100'>> /tmp/socket_client/client.log 2>&1 &\n", IP, ++phone_number);
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, QueryMoney) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    long phone_number = 18326600000;
    for(int i = 0; i < 500; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client %s 5005 4 '%ld' >> /tmp/socket_client/client.log 2>&1 &\n", IP, ++phone_number);
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, DoTrade) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    long phone_number = 18326600000;
    for(int i = 0; i < 200; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client %s 5005 5 '%ld' '%ld' '20' >> /tmp/socket_client/client.log 2>&1 &\n", IP, phone_number, phone_number+1);
        ++phone_number;
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}



