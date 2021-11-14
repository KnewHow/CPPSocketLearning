#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

FILE *fp = nullptr;

void EXIT(int sig) {
    printf("Receive signal: %d, exit program\n", sig);
    if(fp) fclose(fp);
    exit(0);
}

int main(int argc, char **argv) {

    signal(SIGINT, EXIT); // Ctrl + C 
    signal(SIGTERM, EXIT);

    int i = 0;
    char buffer[5];
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        ++i;
        fp = fopen("/tmp/test.txt", "a+");
        sprintf(buffer, "%d\n", i);
        fwrite(buffer, 1, strlen(buffer), fp);
        fflush(fp);
        sleep(1);
    }
}