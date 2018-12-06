#lab05ex1-thread-creation

#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

void* thread_control(void* arg) {
    int *tmp;
    int local_val = 0;
    
    tmp = (int*)arg;
    local_val += (*tmp)*2;
    printf("tid = %ld -- val %d\n", (long)pthread_self(), local_val);
    pthread_exit((void*)(size_t)local_val);
}


int main(int argc, char** argv) {
    
    int i;
    int *pt_i;
    int return_value;
    pthread_t tid[N];
    
    for (i = 0; i < N; i++){
        pt_i = (int*)malloc(sizeof(i));
        *pt_i = i;
        pthread_create((pthread_t*)&tid[i], NULL, thread_control, (void*)pt_i);
    }
    
    for (i = 0; i < N; i++) {
        printf("MT> tid[%d] = %ld\n", i, (long)tid[i]);
        if(pthread_join(tid[i], (void**)&return_value) != 0) {
            perror("join");
            exit(1);
        }
        printf("MT> val -- %d\n", return_value);
    }

    printf("MT> End of program\n");
    
    return 0;
}
