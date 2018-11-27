// modified by myself
//  not the code provided by the professor
#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

int sum = 0;
int count = N;
pthread_t tid[N];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* rand_thread(void* arg) {
    int random_val;

    srand((long)pthread_self());
    random_val = rand() % 11;
    printf("ST> tid = %ld -- val %d\n", (long)pthread_self(), random_val);

    pthread_mutex_lock(&mutex);
    sum += random_val;
    count--;
    if (count == 0)
        pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    
    pthread_exit((void*)random_val);
}

void* print_thread(void* arg) {
    int i;
    int return_value;
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
    for (i = 0; i < N; i++) {
        if(pthread_join(tid[i], (void**)&return_value) != 0) {
            perror("join");
            exit(1);
        }
        printf("EST> tid[%d] = %ld >> val = %d\n", i, (long)tid[i], return_value);
    }
    printf("EST> Sum of return values: %d\n", sum);
    pthread_exit((void*)0);
}

int main(int argc, char** argv) {
    
    int i;
    int *pt_i;
    int return_value;
    pthread_t tidp;
    
    pthread_mutex_lock(&mutex);
    pthread_create(&tidp, NULL, print_thread, NULL);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    for (i = 0; i < N; i++){
        pt_i = (int*)malloc(sizeof(i));
        *pt_i = i;
        pthread_create(&tid[i], NULL, rand_thread, NULL);
    }

    if(pthread_join(tidp, (void**)&return_value) != 0) {
        perror("join");
        exit(1);
    }

    printf("MT> End of program\n");
    
    return 0;
}

