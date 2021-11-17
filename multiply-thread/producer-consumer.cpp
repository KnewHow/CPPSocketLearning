#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <vector>

int messageid = 0;

struct Message {
    Message(){
        id = 0;
        memset(message, 0, sizeof(message));
    }
    Message(int id, char* m): id(id) {
        strcpy(message, m);
    }
    int id;
    char message[32];
};


std::vector<Message> messages;

pthread_mutex_t mutex;
pthread_cond_t cond;

void* consumer_func(void *agrv);
void producer_func(int sig);

int main(int argc, char **argv) {
    signal(15, producer_func);
    
    const int size = 3;
    pthread_t pids[size];
    for(int i = 0; i < size; ++i) {
        pthread_create(&pids[i], nullptr, consumer_func, nullptr);
    }

    for(int i = 0; i < size; ++i) {
        pthread_join(pids[i], nullptr);
    }

    return 0;

}


void producer_func(int sig) {
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < 5; ++i) {
        char buffer[32];
        sprintf(buffer, "This %d th messgae", ++messageid);
        messages.push_back({ messageid, buffer});
    }
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond);
}

void* consumer_func(void *agrv) {
    Message msg;
    while(true) {
        pthread_mutex_lock(&mutex);
        while(messages.size() == 0)
            pthread_cond_wait(&cond, &mutex);
        memcpy(&msg, &messages[0], sizeof(msg));
        messages.erase(messages.begin());
        pthread_mutex_unlock(&mutex);
        printf("[%ld] deal message, id: %d\n", pthread_self(), msg.id);
        usleep(1000);
    }
    return nullptr;
}

