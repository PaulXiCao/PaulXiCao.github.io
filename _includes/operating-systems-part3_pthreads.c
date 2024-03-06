#include <pthread.h>
#include <stdio.h>      // printf

void* doWork(void*) {
    printf("Hello World!\n");
    return NULL;
}

int main(){
    pthread_t t;
    pthread_create(&t, NULL, doWork, NULL);
    pthread_join(t, NULL);
    return 0;
}
