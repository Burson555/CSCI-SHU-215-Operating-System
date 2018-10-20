#define XOPEN_SOURCE 700

#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define BUFSZ 1024
#define WDSZ 24
#define NBUF 4


int cut_down(char *buf, char **cmd) {
    char* s = strtok(buf, " \n");
    int i = 0;
    while (s != NULL) {
        cmd[i] = (char*) malloc(sizeof(char) * WDSZ);
        strcpy(cmd[i], s);
        s = strtok(NULL, " \n");
        i++;
    }
    cmd[i] = NULL;
    return i;
}

void sig_handler(int sig) {
}


int main(int argc, char **argv)
{
    
    int or = 1, pid, command_size, i;
    int nb_cmds = 0; /* nb of commands in current salvo */
    pid_t children[NBUF]; /* list of child PIDs */
    char buf[BUFSZ];
    char *command[WDSZ];
    struct sigaction action;
    sigset_t mask;
    
    sigfillset(&mask);
    sigdelset(&mask, SIGINT);
    sigprocmask (SIG_SETMASK, &mask, NULL);
    
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = sig_handler;
    
    
    while (or > 0)
    {
        for (i = 0; i < BUFSZ ; i++)
            buf[i] = '\0';
        if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1)
            perror("read");
        else if (or > 0) {
            command_size = cut_down(buf, (char**)command);
            if ((pid = fork()) == 0) {
                sigaction(SIGUSR1, &action, 0);
                sigdelset(&mask, SIGUSR1);
                sigsuspend(&mask);
                execvp(command[0], command);
                exit(-1); /* only called if exec goes wrong */
            } else {
                children[nb_cmds] = pid;
                nb_cmds++;
                if (nb_cmds == NBUF) {
                    for (i = 0; i < NBUF; i++)
                        kill(children[i],SIGUSR1);
                    for (i = 0; i < NBUF; i++)
                        wait(0);
                    nb_cmds = 0;
                }
            }
        }
    }
    
    return 0;
}
