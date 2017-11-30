#include <pthread.h>

void* thread1(); //thread 1 function prototype
void* thread2(); //thread 2 function prototype

int main(int argc,char *argv[])
{
    pthread_t i1;
    pthread_t i2;

    pthread_create(&i1, NULL, (void*)&thread1, NULL); //create thread 1
    pthread_create(&i2, NULL, (void*)&thread2, NULL); //create thread 2

    pthread_join(i1, NULL); //calls thread 1 function and the main function suspends in backgrouds
    pthread_join(i2, NULL); //calls thread 2 function and the main function suspends in backgrouds

    printf("Hello, I am main process\n");
}

void* thread1(){
    sleep(5); //sleep for 5 seconds
    printf("Hello, I am thread 1\n");
}

void* thread2(){
    sleep(5); //sleep for 5 seconds
    printf("Hello, I am thread 2\n");
}