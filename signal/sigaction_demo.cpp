#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void task(int sig);

int main(int argc, char** argv)  {

    struct sigaction sgaction;
    memset(&sgaction, 0, sizeof(sgaction));

    // use sgaction implement same signal block
    sgaction.sa_handler = task;
    sgaction.sa_flags = SA_RESTART;
    sigaddset(&sgaction.sa_mask, 15);
    sigaction(34, &sgaction, NULL);
    sigaction(15, &sgaction, NULL);
    for(int i = 0; i < 100; ++i) {
        printf("i = %d\n", i);
        sleep(1);
    }
}

void task(int sig) {
    printf("signal=%d\n", sig);
    for(int j = 0; j < 5; ++j) {
        printf("j(%d)=%d\n", sig, j);
        sleep(1);
    }
}