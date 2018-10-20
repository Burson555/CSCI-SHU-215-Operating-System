/**
 This is a mash up solution for Questions 4 and 5.
 SIGQUIT signals are ignored
 SIGINT signals are summed (best effort)
 SIGALRM signals end the program
 **/

#define XOPEN_SOURCE 700


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NMAX 10

int nb_int = 0;

int exit_flg;

void sig_handler(int sig){
    switch (sig) {
        case SIGALRM:
            printf ("SIGALRM received \n");
            exit_flg=1;
            break;
        case SIGINT:
            nb_int++;
            if (nb_int == NMAX) {
                printf ("NMAX SIGINT received \n");
                exit_flg=1;
            }
            break;
    }
}

int main(int argc, char **argv) {
    struct sigaction action, action2;
    sigset_t mask;
    
    sigfillset(&mask);
    sigprocmask (SIG_SETMASK, &mask, NULL);
    
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = sig_handler;
    action2.sa_handler = SIG_IGN;
    
    sigaction(SIGALRM, &action,0);
    sigaction(SIGINT, &action,0);
    sigaction(SIGQUIT, &action2,0);
    
    printf ("Start program \n");
    
    sigdelset(&mask, SIGINT);
    sigdelset(&mask, SIGQUIT);
    sigdelset(&mask, SIGALRM);
    alarm(10);
    while (!exit_flg)
        sigsuspend(&mask);
    
    return 0;
}


