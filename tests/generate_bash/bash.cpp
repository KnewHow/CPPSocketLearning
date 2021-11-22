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
