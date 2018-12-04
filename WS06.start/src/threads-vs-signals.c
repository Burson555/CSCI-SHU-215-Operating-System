#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>

#define NB_CHILD 18

int nb_child = 0; //number of secondary threads

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t child_block = PTHREAD_COND_INITIALIZER;
pthread_cond_t child_create = PTHREAD_COND_INITIALIZER;

void *thread(void *arg)
{
    nb_child ++;
    pthread_t tid;

    if (nb_child == NB_CHILD) {
        pthread_mutex_lock(&mutex);
        printf("ST> %d last descendant\n", (int)pthread_self());
        pthread_cond_signal(&child_create);
        pthread_cond_wait(&child_block, &mutex);
        pthread_mutex_unlock(&mutex);
        printf("ST> %d end\n", (int)pthread_self());
        pthread_exit(NULL);
    }
    pthread_create(&tid, NULL, thread, NULL);
    pthread_mutex_lock(&mutex);
    printf("ST> %d created %d\n", (int)pthread_self(), (int) tid);
    pthread_cond_wait(&child_block, &mutex);
    pthread_mutex_unlock(&mutex);
    pthread_join(tid, NULL);
    printf("ST> %d end\n", (int)pthread_self());
    pthread_exit(NULL);
}

void sig_hand(int sig){
    pthread_cond_broadcast(&child_block);
}

int main(int argc, char **argv)
{
    sigset_t sig;
    sigfillset(&sig);
    struct sigaction action;
    action.sa_flags = 0;
    action.sa_mask = sig;
    action.sa_handler = sig_hand;
    sigaction(SIGINT, &action, 0);
    sigprocmask(SIG_SETMASK, &sig, NULL);

    pthread_t tid;
    pthread_mutex_lock(&mutex);
    pthread_create(&tid, NULL, thread, NULL);
    pthread_cond_wait(&child_create, &mutex);
    pthread_mutex_unlock(&mutex);
    printf("All my descendants are created.\n");

    sigdelset(&sig, SIGINT);
    sigsuspend(&sig);

    pthread_join(tid, NULL);
    printf("All my descendants have ended.\n");

    return EXIT_SUCCESS;
}