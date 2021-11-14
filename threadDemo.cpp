#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t pid1, pid2;
int x = 0, y = 0;

void *thread_fun1(void *arg);
void *thread_fun2(void *arg);

int main(int argc, char **argv) {
    if(pthread_create(&pid1, nullptr, thread_fun1, (void*)0) != 0)
        printf("create thead one failure\n");
    
    if(pthread_create(&pid2, nullptr, thread_fun2, (void*)0) != 0)
        printf("create thead two failure\n");
    
    printf("main function start join\n");
    pthread_join(pid1, nullptr);
    pthread_join(pid2, nullptr);

    printf("main function exit");
    return 0;
}

void *thread_fun1(void *arg) {
    for(x = 0; x < 100; ++x) {
        printf("x is: %d\n", x);
        sleep(1);
    }
    pthread_exit(nullptr);
}

void *thread_fun2(void *arg) {
    for(y = 0; y < 100; ++y) {
        printf("y is: %d\n", y);
        sleep(1);
    }
    pthread_exit(nullptr);
}