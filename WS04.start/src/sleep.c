#define XOPEN_SOURCE 700

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define timeSet 5

int timeCount;

void sig_hand (int sig) {
	timeCount --;
	printf("Time left: %d\n", timeCount);
}

unsigned int mysleep(int sec) {

	timeCount = sec;
    
    // Now I should set a mask
    // so that receiving signals other than SIGALRM
    // will return the time remaining
    // before doing the defaut behavior

    // since the suspension will only be executed once
    // I can only modify the sigation

    // set mask
    sigset_t sig_proc;
	struct sigaction action;
	sigemptyset (& sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction (SIGALRM, & action, 0);

    while (timeCount != 0) {
        alarm(1);
        pause();
    }

    action.sa_handler = SIG_DFL;
    sigaction (SIGALRM, & action, 0);
    alarm(2);
    sleep(10);

    return 0;
}

int main (int argc, char* argv []) {

    if (fork() == 0){
        sleep((int) (timeSet / 2));
        printf("child sleeped for %d seconds\n", (int) (timeSet / 2));
        kill(getppid(), SIGINT);
        exit(0);
    }
    
    mysleep(timeSet);
    
    return EXIT_SUCCESS;
}