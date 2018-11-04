#define XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// ***************** defining variables *****************
// #define nbProcesses 5
// nbProcesses represents total number of descendants to create
pid_t ppid = 0; // used to locate the parent
pid_t pid;
int i;
// ***************** defining variables *****************

// ***************** defining handlers *****************
void usr1Handler (int sig) {
    if (ppid != 0){
        // all descendants but LD: send signal to P
        kill(ppid, SIGUSR1);
    }
    printf("Generation %d: receiving %d\n", i, sig);
}
void usr2Handler (int sig) {
    if (pid != 0){
        // all descendants but LD: send signal to C
        kill(pid, SIGUSR2);
    }
    printf("Generation %d: receiving %d\n", i, sig);
}
void intHandler (int sig) {
    if (ppid != 0){
        // all descendants but LD: send signal to P
        kill(ppid, SIGINT);
    }
    printf("Generation %d: receiving %d\n", i, sig);
}
// ***************** defining handlers *****************

int wave(int N) {

    sigset_t generalMask;
	struct sigaction actionINT;
    struct sigaction actionUSR1;
    struct sigaction actionUSR2;

	sigfillset (& generalMask);
    sigdelset (& generalMask, SIGINT);
    sigdelset (& generalMask, SIGUSR1);
    sigdelset (& generalMask, SIGUSR2);

    sigprocmask (SIG_SETMASK, & generalMask, NULL);

    actionUSR1.sa_mask = generalMask;
    actionUSR1.sa_flags = 0;
    actionUSR1.sa_handler = usr1Handler;
	sigaction (SIGUSR1, & actionUSR1, 0);

    actionUSR2.sa_mask = generalMask;
    actionUSR2.sa_flags = 0;
    actionUSR2.sa_handler = usr2Handler;
    sigaction (SIGUSR2, & actionUSR2, 0);

    actionINT.sa_mask = generalMask;
    actionINT.sa_flags = 0;
    actionINT.sa_handler = intHandler;
    sigaction (SIGINT, & actionINT, 0);

    // create lineage of processes
    for (i = 0; (i < N) && ((pid = fork()) == 0); i++)
        ppid = getppid();

    // ***************** 1st wave *****************
    if (pid == 0) {
        kill(ppid, SIGUSR1);
    } else {
        pause();
    }
    // ***************** 1st wave *****************

    // ***************** 2nd wave *****************
    if (ppid == 0) {
        kill(pid, SIGUSR2);
    } else {
        pause();
    }
    // ***************** 2nd wave *****************

    // ***************** 3rd wave *****************
    // sending SIGINT is necessary
    // because only in this way can the SIGINT be passed on to PP
    // of course we change the handler of SIGINT
    if (pid == 0) {
        kill(ppid, SIGINT);
    } else {
        pause();
    }
    // ***************** 3rd wave *****************

    if (ppid == 0)
        printf("End of program\n");

    return 0;
}

int main (int argc, char* argv []) {
    
    wave(nbProcesses);
    
    return EXIT_SUCCESS;
}