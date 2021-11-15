#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int a = 0;

int main(int argc, char **argv) {
    int b = 3;
    int pid = fork();
    if(pid != 0) { // parent process
        sleep(5);
        printf("This is parent process, pid is: %d, ppid is: %d\n", getpid(), getppid());
        ++a;
        ++b;
        printf("[%d] a=%d, b=%d\n",getpid(), a, b);
    } else { // child process
        sleep(10);
        printf("This is child process, pid is: %d, ppid is: %d\n", getpid(), getppid());
        printf("[%d] a=%d, b=%d\n", getpid(), a, b);
    }

}