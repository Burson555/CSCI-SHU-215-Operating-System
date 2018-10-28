// 1
// Write a program called pending-signals that displays whether it has received signals but has yet to deliver them. This program works as follows: it masks SIGINT and SIGQUIT, then sleeps for 5 seconds using the sleep function. Upon awakening, the program should display whether any or both of these signals are pending.

// What happens if the program unmasks SIGINT and SIGQUIT before displaying whether they are pending? Explain why.

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	sigset_t mask_set, wait_set;
    struct sigaction action;

	sigemptyset(&mask_set);
	sigemptyset(&wait_set);

	sigaddset(&mask_set, SIGINT);
	sigaddset(&mask_set, SIGQUIT);

	sigprocmask(SIG_BLOCK, &mask_set, NULL);

	printf("%s\n", "Finish setting mask");
	printf("%s\n", "start sleep");
	kill(getpid(), SIGQUIT);
	kill(getpid(), SIGINT);
	sleep(3);
	printf("%s\n", "end sleep");

	sigpending(&wait_set);
    
    // if I unclock the mask before displaying the pending
    // the pending information won't show
    // because as soon as I unblock, the signals get dilivered
    // and the program ends acordingly
//    sigprocmask(SIG_UNBLOCK, &mask_set, NULL);
    
	if (sigismember(&wait_set, SIGINT))
		printf("%s\n", "SIGINT is a member");
	if (sigismember(&wait_set, SIGQUIT))
		printf("%s\n", "SIGQUIT is a member");

	return 0;
}
