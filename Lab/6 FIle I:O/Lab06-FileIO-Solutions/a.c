#define _XOPEN_SOURCE 700
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

// about Dir
DIR* pt_Dir;
struct dirent *dent;
int main (int argc, char * argv []) {
	if ((pt_Dir = opendir (argv [1])) == NULL) { 
		perror ( "opendir");
		return EXIT_FAILURE;
	}
	while ((dent = readdir(pt_Dir)) != NULL)
		printf ( "%s \n", dent-> d_name);
	closedir (pt_Dir);
	return EXIT_SUCCESS;
}


// about fctnl
int main (int argc, char * argv []) {
    int fd1, mode_op;
    if ((fd1 = open (argv[1], O_RDWR)) == -1) {
    	perror("open"); 
   		return EXIT_FAILURE; 
   	}
    if (write(fd1, "abc", 3) == -1) { 
    	perror("write"); 
    	return EXIT_FAILURE; }
    mode_op = fcntl(fd1, F_GETFL);
    mode_op |= O_APPEND;
    fcntl (fd1, F_SETFL, mode_op);
    if (write(fd1, "xyz", 3) == -1) { 
    	perror("write"); 
    	return EXIT_FAILURE; 
    }
	close(fd1);
    return EXIT_SUCCESS ;
 }