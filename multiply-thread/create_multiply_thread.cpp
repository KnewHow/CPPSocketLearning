#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>


int sum = 0;
void *thread_func(void *argv);

int main(int argc, char** argv) {
    pthread_t pid1, pid2;
    if(pthread_create(&pid1, nullptr, thread_func, nullptr) != 0) {
        printf("create thread failure\n");
    }
    if(pthread_create(&pid2, nullptr, thread_func, nullptr) != 0) {
        printf("create thread failure\n");
    }
    printf("pid1=%ld, pid2=%lu\n", pid1, pid2);
    pthread_join(pid1, nullptr);
    pthread_join(pid2, nullptr);
    printf("sum=%d\n", sum);
    return 0;
}

void *thread_func(void *argv)
{
    pid_t tid = gettid();
    for(int i = 0; i < 200; ++i) {
        ++sum;
        //printf("[%d] sum=%d\n", tid, sum);
        usleep(100);
    }
    return nullptr;
}
