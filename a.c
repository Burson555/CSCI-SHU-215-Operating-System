#define XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 128



int main (int argc, char** argv) {

	char *env[] = { "HOME=/Users/bosen/Desktop/WS03.start/bin", (char *)0 };

	printf("hahahahahah\n");
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	   printf("Current working dir: %s\n", cwd);
	} else {
	   perror("getcwd");
	   exit(2);
	}
	strcat(cwd, "111");
	printf("%s\n", cwd);
    // execlp("/Users/bosen/Library/Mobile Documents/com~apple~CloudDocs/Portal/OS_SF/WS03.start", "make", NULL);
    // execlp("make", "make", "-C", "/Users/bosen/Desktop", NULL);
    // execlp("tar", "tar", "xvzf", "/Users/bosen/Desktop", NULL);
    // execlp("make", "make", "-C", "/Users/bosen/Desktop/WS03.start", NULL);
    // execlp("/Users/bosen/Desktop/WS03.start/bin/sched-simulator", "sched-simulator", "/Users/bosen/Desktop/WS03.start/tsk/tasks1", "RR", "4", NULL);
    // execlp("ls", "ls", "/Users/bosen/Library/", NULL);

    return EXIT_SUCCESS;
}