#include<stdio.h>
#include<pthread.h>

#include<unistd.h>

pthread_mutex_t A;
pthread_mutex_t B;

void *sub(void*argv ) {
    printf("sub process\n");
    pthread_mutex_lock(&B);
    /*kfdlsjakfjdskafjsdkjfa*/
    pthread_mutex_lock(&A);
    /**kfdsjfdskfjlskajf*/


    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);
}

int main() {
    printf("main process\n");
    /*kfdlsjakfjdskafjsdkjfa*/
    pthread_mutex_lock(&A);
    pthread_t id;
    pthread_create(&id,NULL,sub,NULL);
 
    /**kfdsjfdskfjlskajf*/
    pthread_mutex_lock(&B);

    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);    
   sleep(1);
    return 0;
}
