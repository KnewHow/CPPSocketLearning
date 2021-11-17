#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void* thread_func(void *agrv);
void* thread_func2(void *agrv);
void notify_func(int sig);

int main(int argc, char** argv) {

    signal(15, notify_func);

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);

    const int size = 2;
    pthread_t pids[size];
    for(int i = 0; i < size; ++i) {
            pthread_create(&pids[i], nullptr, thread_func, nullptr);
    }
    // pthread_t pid3;
    // pthread_create(&pid3, nullptr, thread_func2, nullptr);

    for(int i =0; i < size; ++i) {
        pthread_join(pids[i], nullptr);
    }
    // pthread_join(pid3, nullptr);
    return 0;
}

void* thread_func(void *agrv) {
    while(true) {
        pthread_mutex_lock(&mutex);
        printf("[%ld] wait...\n", pthread_self());
        // when wait: release lock and wait
        // when weak up: get lock(if it can) and run
        pthread_cond_wait(&cond, &mutex);
        printf("[%ld] has been weaken...\n", pthread_self());
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void* thread_func2(void *agrv) {
    pthread_mutex_lock(&mutex);
    printf("thread lock pid=%ld\n", pthread_self());
    return nullptr;
}

void notify_func(int sig) {
    pthread_cond_signal(&cond);
    //pthread_cond_broadcast(&cond);
}