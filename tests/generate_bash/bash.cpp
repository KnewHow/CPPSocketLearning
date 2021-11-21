#include "gtest/gtest.h"
#include <glog/logging.h>
#include <stdio.h>
#include <string.h>

TEST(Bash, Regist) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    for(int i = 0; i < 1; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client 127.0.0.1 5005 1 '18326600932' 'Knewhow' '123456' 1 25 \n");
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, Login) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    for(int i = 0; i < 1; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client 127.0.0.1 5005 2 '18326600932' '123456'\n");
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, Recharge) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    for(int i = 0; i < 1; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client 127.0.0.1 5005 3 '18326600932' '100'\n");
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, QueryMoney) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    for(int i = 0; i < 1; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client 127.0.0.1 5005 4 '18326600931'\n");
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}

TEST(Bash, DoTrade) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    for(int i = 0; i < 1; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client 127.0.0.1 5005 5 '18326600931' '18326600932' '20'\n");
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}



