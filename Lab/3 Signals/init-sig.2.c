#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;

void sig_hand(){
	printf("\nhahahaha\n");
}

int main () {

	// change SIGINT handler
	sigset_t sig_proc;
 	struct sigaction action;
	
 	sigemptyset (& sig_proc); 
 	action.sa_mask = sig_proc; 
  action.sa_flags = 0; 
  action.sa_handler = sig_hand; 
	sigaction(SIGINT, &action, 0);

	// mask all but SIGINT
	sigfillset (&sig);
	sigdelset (&sig, SIGINT);
	sigprocmask (SIG_SETMASK, &sig, NULL);

	sigsuspend (&sig);
	printf ("1er signal\n");
	sigsuspend (&sig);
	printf ("2e signal\n");
	return 0;
}
