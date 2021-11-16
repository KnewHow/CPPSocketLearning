#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int sum = 0;

pthread_mutex_t mutex;

void *increase(void *argv);

int main(int argc, char **argv) {

    if(pthread_mutex_init(&mutex, nullptr)) {
        perror("pthread_mutex_init");
        printf("init mutex failure\n");
    }

    pthread_t pids[3];
    for(int i = 0; i < 3; ++i) {
        pthread_create(&pids[i], nullptr, increase, nullptr);
    }

    for(int i = 0; i < 3; ++i) {
        pthread_join(pids[i], nullptr);
    }

    pthread_mutex_destroy(&mutex);
    printf("sum=%d\n", sum);
    return 0;
}

void *increase(void *argv) {
    for(int i = 0; i< 200; ++i) {
        pthread_mutex_lock(&mutex);
        ++sum;
        pthread_mutex_unlock(&mutex);
        usleep(1000);
    }
    return nullptr;
}