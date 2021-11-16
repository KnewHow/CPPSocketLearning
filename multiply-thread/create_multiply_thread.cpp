#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <memory>
#include <string.h>

struct ThreadInfo {
    ThreadInfo(){
        number = 0;
        memset(info, 0, sizeof(info));
    }
    ThreadInfo(int number, char *in)
        :number(number) {
        strcpy(info, in);
    }
    int number;
    char info[128];
};

int sum = 0;
void *thread_func(void *argv);

int main(int argc, char** argv) {

    printf("sizeof int:%ld, sizeof(void *): %ld, sizeof(std::shared<int>): %ld\n", sizeof(int), sizeof(char *), sizeof(std::shared_ptr<double>));
    pthread_t threads[5];
    for(int i = 0; i < 5; ++i) {
        char buffer[128];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "This is %d th thread, number is: %d", i+1, i+1);
        ThreadInfo *info = new ThreadInfo(i + 1, buffer);
        if(pthread_create(&threads[i], nullptr, thread_func, (void *)info) != 0) { // because pointer type with 8 bytes, we let int to long, then become the address of the pointer, so the parameter to the function parameter
            printf("create thread failure\n");
        }
    }
    
    for(int i = 0; i < 5; ++i) {
        pthread_join(threads[i], nullptr);
    }
    printf("sum=%d\n", sum);
    return 0;
}

void *thread_func(void *argv)
{
    ThreadInfo *info = (ThreadInfo*)argv;
    printf("Current thread number is:%d, message is:%s\n", info->number, info->info);
    for(int i = 0; i < 200; ++i) {
        ++sum;
        usleep(100);
    }
    delete info;
    return nullptr;
}
