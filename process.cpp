#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    printf("Begin\n");
    int pid;
    if((pid = fork()) != 0) {
        printf("fork pid is: %d\n", pid);
        printf("I am parent process pid is: %d, ppid is: %d\n", getpid(), getppid());
        for(int i = 0; i < 10; ++i) {
            printf("i=%d\n", i);
            sleep(1);
        }
        exit(0);
    } else {
        printf("I am child process pid is: %d, ppid is: %d\n", getpid(), getppid());
        for(int j = 0; j < 10; ++j) {
            printf("j=%d\n", j);
            sleep(1);
        }
        exit(0);
    }
}