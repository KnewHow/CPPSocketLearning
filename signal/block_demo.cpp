#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void task1(int sig);
void task2(int sig);

int main(int argc, char ** argv) {

    signal(15, task1);
    signal(34, task2);

    for(int i = 0; i < 100; ++i) {
        printf("i=%d\n", i);
        sleep(1);
    }
}

void task1(int sig) {

    sigset_t set
    memset;
    sigemptyset(&set);
    sigaddset(&set, 34);
    sigprocmask(SIG_BLOCK, &set, NULL); // let 34 signal block

    for(int j = 0; j < 5; ++j) {
        printf("signal = %d, j = %d\n",sig, j);
        sleep(1);
    }
    sigprocmask(SIG_UNBLOCK, &set, NULL); // let 34 signal unblock
}

void task2(int sig) {
    for(int j = 0; j < 5; ++j) {
        printf("signal = %d, j = %d\n",sig, j);
        sleep(1);
    }
}