#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define LEN_ARR 100
#define NB_CONS 6
#define NB_PROD 6
#define PROD_ROUND 10

int count;
int arr[LEN_ARR];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t cons = PTHREAD_COND_INITIALIZER;

void* prod_func(void *argv){
    // int i;
    // for (i = 0; i < PROD_ROUND; ++i) {
    // while(1){
        int msg;
        srand((int) pthread_self () * rand());
        msg =(int) (1 + rand() % 100);
    
        pthread_mutex_lock(&mutex);
        if (count >= LEN_ARR){
            // pthread_cond_signal(&cons);
            pthread_cond_wait(&prod, &mutex);
        }
        arr[count] = msg;
        printf("Produce arr[%d] %d\n", count, msg);
        count++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cons);
    // }

    pthread_exit((void*)0);
}

void* cons_func(void *argv){
    // int i;
    // for (i = 0; i < PROD_ROUND; ++i) {
    // while(1){
        int msg;
        pthread_mutex_lock(&mutex);
        if (count == 0){
            // pthread_cond_signal(&prod);
            pthread_cond_wait(&cons, &mutex);}
        msg = arr[count - 1];
        printf("Consume arr[%d] %d\n", count - 1, msg);
        count--;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&prod);
    // }
    pthread_exit((void*)0);
}


int main (int argc, char ** argv) {

    int i;

    printf("MT> %d producers\n", NB_PROD);
    printf("MT> %d consumers\n", NB_CONS);

    pthread_t tid_prod[NB_PROD];
    pthread_t tid_cons[NB_CONS];

    for (i = 0; i < NB_PROD; i++)
        pthread_create(&tid_prod[i], NULL, prod_func, NULL);

    for (i = 0; i < NB_CONS; i++)
        pthread_create(&tid_cons[i], NULL, cons_func, NULL);

    for (i = 0; i < NB_PROD; i++) 
        pthread_join(tid_prod[i], NULL);
    
    for (i = 0; i < NB_CONS; i++)
        pthread_join(tid_cons[i], NULL);
    

    return EXIT_SUCCESS;
}
