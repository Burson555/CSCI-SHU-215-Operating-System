// modified by myself
//  not the code provided by the professor
#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* rand_thread(void* arg) {
    int random_val;

    srand((long)pthread_self());
    random_val = rand() % 11;
    printf("tid = %ld -- val %d\n", (long)pthread_self(), random_val);

    pthread_mutex_lock(&mutex);
    sum += random_val;
    pthread_mutex_unlock(&mutex);
    
    pthread_exit((void*)random_val);
}


int main(int argc, char** argv) {
    
    int i;
    int *pt_i;
    int return_value;
    pthread_t tid[N];
    
    for (i = 0; i < N; i++){
        pt_i = (int*)malloc(sizeof(i));
        *pt_i = i;
        pthread_create(&tid[i], NULL, rand_thread, NULL);
    }
    
    for (i = 0; i < N; i++) {
        printf("MT> tid[%d] = %ld\n", i, (long)tid[i]);
        if(pthread_join(tid[i], (void**)&return_value) != 0) {
            perror("join");
            exit(1);
        }
        printf("MT> val -- %d\n", return_value);
    }

    printf("MT> Sum of return values: %d\n", sum);
    printf("MT> End of program\n");
    
    return 0;
}

