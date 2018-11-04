#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h> 



// Page 11 example
// int main (int arg, char ** argv) {
//     printf ( "begin program\n");
//     kill (getpid (), SIGINT);
//     printf ( "end program\n");
//     return 0;
// }

// Page 15 example
// void sig_hand(int sig) {
//    printf ( "received signal% d \n", sig);
// }
//
//  int main (int argc, char ** argv) {
//          sigset_t sig_proc;
//          struct sigaction action;
//
//          printf ( "start program \n");
//          sigemptyset(&sig_proc);
//          action.sa_mask = sig_proc;
//          action.sa_flags = 0;
//          action.sa_handler = sig_hand;
//          sigaction (SIGINT, &action, 0);
//          kill (getpid(), SIGINT);
//          printf ( "end program \n");
//  }

// Page 24 example
// int main(int argc, char const *argv[]) {
//   sigset_t sig_proc;
//   printf("start program\n");
//
//   sigemptyset(&sig_proc);
//   sigaddset(&sig_proc, SIGINT);
//   sigprocmask(SIG_BLOCK, &sig_proc, 0);
//
//   sleep(5);
//   printf("after sleep\n");
//   sigprocmask(SIG_UNBLOCK, &sig_proc, 0);
//
//   printf("end program\n");
//   sleep(6);
//   return 0;
// }

int main (int argc, char * argv []) { 
  sigset_t sig_set;
  // / * List of blocked signals * / 
  sigemptyset (& sig_set); 
  sigaddset (& sig_set, SIGINT); 
  // / * Hide * SIGINT / 
  sigprocmask (SIG_SETMASK, & sig_set, NULL); 
  printf(" start killing\n");
  kill(getpid (), SIGINT); 
  printf(" end killing\n");
  // / * Get the list of pending signals * /
  printf("print pending #1\n");
  igpending (& sig_set); 
  printf("print pending #2\n");
  if (sigismember (& sig_set, SIGINT)) 
    printf ( "SIGINT is pending\n"); 
  return EXIT_SUCCESS; 
}

// void sig_hand (int sig) {
// printf ( "\nreceived signal% d \n", sig);
// }
//
// int main (int argc, char ** argv) {
//   sigset_t sig_proc;
//   struct sigaction action;
//
//   // / * Mask SIGINT * / 
//   sigemptyset (& sig_proc); 
//   sigaddset (& sig_proc, SIGINT); 
//   sigprocmask (SIG_SETMASK, & sig_proc, NULL); 
//
//   // / * Redefine handler for SIGINT * / 
//   action.sa_mask = sig_proc; 
//   action.sa_flags = 0; 
//   action.sa_handler = sig_hand; 
//   sigaction (SIGINT, & action, NULL); 
//
//   // / * Wait for SIGINT * / 
//   sigfillset (& sig_proc);
//   sigdelset (& sig_proc, SIGINT); 
//   sigsuspend (& sig_proc); 
//
//   printf("adfasdfsdfa\n");
//
//   return EXIT_SUCCESS;
// }


// pid_t pid_child;

// int cont;
// void sig_hand (int sig) {
//   if (sig == SIGUSR1){
//     cont ++;
//     printf ( "cont++ \n");
//   }
//   else {
//     printf ( "number of SIGUSR1 received: %d \n", cont);
//     exit (0);
//   }
// }

// int main (int argc, char ** argv) {
//     sigset_t sig_proc; int i;  struct sigaction action;
//     sigemptyset (& sig_proc);
//     action.sa_mask = sig_proc;
//     action.sa_flags = 0;
//     action.sa_handler = sig_hand;
//     sigaction (SIGUSR1, & action, 0);
//     sigaction (SIGINT, & action, 0);
//     if ((pid_child = fork ()) == 0) {
//       while (1)
//         pause ();
//     } else {
//       for (i = 0; i <20; i ++)
//         kill (pid_child, SIGUSR1);
//       kill (pid_child, SIGINT);
//       wait (NULL);
//       return EXIT_SUCCESS;
//  }
// }

// sigset_t sig;
// struct sigaction sigact_int;
// struct sigaction sigact_alrm;
// int n_int = 0;
// int n_alrm = 0;

// void sigalrm_handler (int sig) {
//   printf ("reception de SIGALRM\n");
//   alarm (7);
// }

// void sigint_handler (int sig) {
//   printf ("reception de SIGINT\n");
// }


// int main () {
//   sigfillset (&sig);
//   sigdelset (&sig, SIGINT);
//   sigdelset (&sig, SIGALRM);

//   sigprocmask (SIG_SETMASK, &sig, NULL);

//   sigact_int.sa_handler = sigint_handler;
//   sigaction (SIGINT, &sigact_int, NULL);

//   sigact_alrm.sa_handler = sigalrm_handler;
//   sigaction (SIGALRM, &sigact_alrm, NULL);

//   alarm (1);

//   while (1) {
//     sigsuspend (&sig);
//   }
//   return 0;
// }
