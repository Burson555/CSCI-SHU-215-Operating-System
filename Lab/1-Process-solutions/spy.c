/**** spy.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define _XOPEN_SOURCE 700

#define BUFSZ 1024
#define WDSZ 24


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


int main(int argc, char **argv)
{

    int or = 1, pid, command_size, i;
	char buf[BUFSZ];
	char *command[WDSZ];

    while (or > 0) 
    {
		for (i = 0; i < BUFSZ ; i++)
			buf[i] = '\0';
        if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1)
			perror("read");
		else if (or > 0) {
			command_size = cut_down(buf, (char**)command);
            /** The following block displays the command line entered by the user. **/
            /*
            for (i = 0; i < command_size; i++) {
                printf("%s \n", command[i]);
            }
            printf("\n");
            */
			/* TO DO :
					-> EXECUTION OF THE COMMAND BY A CHILD
					-> PARENT WAITS UNTIL COMPLETION AND RELEASES MEMORY */
            if (fork() == 0) {
                if (command_size == 2) {
                    execlp(command[0], command[0], command[1], NULL);
                } else {
                    execvp(command[0], &command[0]);
                }
            }
		}
    }
	
	return 0;
}
