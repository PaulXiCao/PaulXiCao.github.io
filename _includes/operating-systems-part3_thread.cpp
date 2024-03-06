#include <thread>
#include <stdio.h>      // printf

void doWork() {
    printf("Hello World!\n");
}

int main(){
    std::thread t(doWork);
    t.join();
    return 0;
}
