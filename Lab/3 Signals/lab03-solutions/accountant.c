#define XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NMAX 10

int nb_sigs[NSIG];
int exit_flg = 0;

void sig_handler(int sig){
    nb_sigs[sig]++;
    if (nb_sigs[SIGINT] == NMAX) {
        printf ("NMAX SIGINT received \n");
        exit_flg = 1;
    }
    if (sig == SIGALRM) {
        exit_flg = 1;
    }
}

int main(int argc, char **argv) {
    struct sigaction action;
    sigset_t mask;
    int i;
    
    sigfillset(&mask);
    sigprocmask (SIG_SETMASK, &mask, NULL);
    
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = sig_handler;
    for (i = 1; i < NSIG; i++) {
        nb_sigs[i] = 0;
        sigaction(i, &action,0);
    }
    
    printf ("Start program \n");
    
    sigemptyset(&mask);
    alarm(10);
    while (!exit_flg)
        sigsuspend(&mask);
    
    for (i=1; i <NSIG; i++)
        printf("sig %d - # %d\n", i, nb_sigs[i]);

    
    return 0;
}
