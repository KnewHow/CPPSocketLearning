#include "gtest/gtest.h"
#include <glog/logging.h>
#include <stdio.h>
#include <string.h>

// char IP[16] = "123.60.46.246";
char IP[16] = "127.0.0.1";

TEST(Bash, Client) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    for(int i = 0; i < 10000; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client %s 5005 %d >> /tmp/socket_client/client.log 2>&1 &\n", IP, i);
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}
