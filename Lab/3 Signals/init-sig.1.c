#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_hand(int sig){
	printf("hahahaha %d\n", sig);
}
int main () {
	sigset_t sig_proc;
	struct sigaction action;

	sigemptyset (& sig_proc); 

	action.sa_mask = sig_proc; 
  action.sa_flags = 0; 
  action.sa_handler = sig_hand; 

	sigaction(SIGINT, &action, 0);
	pause ();
	printf ("1er signal\n");
	pause ();
	printf ("2e signal\n");
	return 0;
}
