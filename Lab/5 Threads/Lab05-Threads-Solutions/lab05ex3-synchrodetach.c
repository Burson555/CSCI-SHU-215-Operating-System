#lab05ex3-synchrodetach

#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int cpt;
int val;

void* thread_rand(void* arg) {
  int *tmp = (int*)arg;
  pthread_mutex_lock(&mutex);
  cpt++;
  val += (*tmp)*2;
  if(cpt == N)
    pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  pthread_exit((void*)0);
}

void* thread_print(void* arg) {
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cond);
  pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);
  printf("val -- %d\n", val);
  pthread_exit((void*)0);
}

int main(int argc, char** argv) {

  int i;
  pthread_t tid[N+1];
  int *pt_i;

  /*create thread_print then wait for its signal */
  pthread_mutex_lock(&mutex);
  pthread_create((pthread_t*)&tid[0], NULL, thread_print, NULL);
  pthread_cond_wait(&cond, &mutex);
  cpt = 0;
  val = 0;
  pthread_mutex_unlock(&mutex);

  /*thread_print is waiting for a signal from a thread_rand ;
    all thread_rand threads can now be created.*/
  for (i = 1; i <= N; i++){
    pt_i = (int*)malloc(sizeof(i));
    *pt_i = i;
    pthread_create((pthread_t*)&tid[i], NULL, thread_rand, (void*)pt_i);
    pthread_detach(tid[i]);
  }

  pthread_join(tid[0], NULL);

  return 0;
}
