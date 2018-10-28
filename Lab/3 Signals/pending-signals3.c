// 3
// Write a modification of the previous program called pending-signals-3, where the behavior is redefined so as to increment a counter and display the value of this counter when receiving a SIGINT.

// What is the maximum value that the counter can reach? Will it be reached upon every execution? Justify your answer.

// What happens now if the program unmasks SIGINT and SIGQUIT before displaying whether they are pending? Explain why.

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int counter;
void sig_hand(int sig) {
    counter++;
    printf ( "counter: %d\n", counter);
}

int main(int argc, char const *argv[])
{
    sigset_t mask_set, wait_set;
    struct sigaction action;
    
    // prosedure
    sigemptyset(&mask_set);
    sigemptyset(&wait_set);
    
    // create set without the two
    sigaddset(&mask_set, SIGINT);
    sigaddset(&mask_set, SIGQUIT);
    
    // mask
    sigprocmask(SIG_SETMASK, &mask_set, NULL);
    
    // redefine
    action.sa_mask = wait_set;
    action.sa_flags = 0;
    action.sa_handler = sig_hand;
    sigaction (SIGINT, &action, 0);
    sigaction (SIGQUIT, &action, 0);
    
    printf("%s\n", "Finish setting mask");
    printf("%s\n", "start sleep");
    kill(getpid(), SIGINT);
    kill(getpid(), SIGQUIT);
    sleep(3);
    printf("%s\n", "end sleep");
    
    sigprocmask(SIG_UNBLOCK, &mask_set, NULL);
    
    sigpending(&wait_set);
    
    if (sigismember(&wait_set, SIGINT))
        printf("%s\n", "SIGINT is a member");
    else
        printf("%s\n", "SIGINT is NOT a member");
    if (sigismember(&wait_set, SIGQUIT))
        printf("%s\n", "SIGQUIT is a member");
    else
        printf("%s\n", "SIGQUIT is NOT a member");
    
    printf("\nelngth of sigset_t: %d\n", sizeof(wait_set));
    return 0;
}

