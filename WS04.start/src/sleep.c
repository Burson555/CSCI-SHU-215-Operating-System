#define XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sigint_handler (int sig) {
    printf ("reception de SIGINT\n");
}

unsigned int mysleep(int sec) {

    alarm(sec);
    pause();
    return alarm(0);
}

int main (int argc, char* argv []) {

    // change default behavior of SIGINT
    sigset_t sig_proc;
    struct sigaction action;
    sigemptyset (& sig_proc);
    action.sa_mask = sig_proc;
    action.sa_flags = 0;
    action.sa_handler = sigint_handler;
    sigaction (SIGINT, &action, 0);
    // change default behavior of SIGINT

    if (fork() == 0){
        sleep((int) (timeSet / 2));
        printf("total sleep time is %d seconds\n", timeSet);
        printf("child has sleeped for %d seconds\n", (int) (timeSet / 2));
        kill(getppid(), SIGINT);
        exit(0);
    }
    
    printf("time left is %ds\n", mysleep(timeSet));
    
    return EXIT_SUCCESS;
}