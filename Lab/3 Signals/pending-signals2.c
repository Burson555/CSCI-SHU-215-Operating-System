// 2
// Write a modification of the previous program called pending-signals-2, where the behavior is redefined so as to ignore a SIGINT or a SIGQUIT.

// What happens now if the program unmasks SIGINT and SIGQUIT before displaying whether they are pending? Explain why.

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

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
    action.sa_handler = SIG_IGN;
    sigaction (SIGINT, &action, 0);
    sigaction (SIGQUIT, &action, 0);
    
    printf("%s\n", "Finish setting mask");
    printf("%s\n", "start sleep");
    kill(getpid(), SIGQUIT);
    kill(getpid(), SIGINT);
    sleep(3);
    printf("%s\n", "end sleep");
    
    sigpending(&wait_set);
    
    sigprocmask(SIG_UNBLOCK, &mask_set, NULL);
    // now whether or not we mask SIGINT or SIGQUIT
    // they won't get printed
    // because they're ignored
    
    if (sigismember(&wait_set, SIGINT))
        printf("%s\n", "SIGINT is a member");
    if (sigismember(&wait_set, SIGQUIT))
        printf("%s\n", "SIGQUIT is a member");
    
    return 0;
}

