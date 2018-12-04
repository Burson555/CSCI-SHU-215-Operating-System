#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

// Page 10
// int main (int argc, char * argv []) { 
// 	struct stat stat_info;
// 	if (stat (argv [1], & stat_info) == -1) {
// 		perror ( "stat error"); 
// 		return EXIT_FAILURE; } 
// 	if (!S_ISDIR (stat_info.st_mode))
// 		printf ( "NOT file directory \n");
// 	printf ( "File size:% ld \n", (long) stat_info.st_size); 
// 	if (stat_info.st_mode & S_IRGRP)
// 		printf ( "users of the same goupe can read the file \n"); 
// 	return EXIT_SUCCESS;
// }


// Page 18
// #define BUFSZ 100
// char buffer [BUFSZ];
// int main (int argc, char * argv []) {
// 	int fd1, fd2; int n, i;
// 	fd1 = open (argv [1], O_WRONLY | O_CREAT | O_SYNC, 0600); 
// 	fd2 = open (argv [1], O_RDWR);
// 	if ((fd1 == -1) || (fd2 == -1))
// 		{printf ( "%s open", argv [1]); return EXIT_FAILURE;} 
// 	if (write (fd1, "abcdef", strlen ( "abcdef")) == -1)
// 		{perror("write"); return EXIT_FAILURE; }
// 	if (write (fd2, "123", strlen ( "123")) == -1)
// 		{perror ( "write"); return EXIT_FAILURE; }
// 	if ((n = read (fd2, buffer, BUFSZ)) <= 0)
// 		{perror ( "end file \n"); return EXIT_FAILURE;}
// 	for (i = 0; i < n; i ++)
// 		printf ( "%c", buffer [i]);
// 	printf("\n");
// 	return EXIT_SUCCESS;
// }


// Page 23
#define _XOPEN_SOURCE 700
int fd1;
int main (int argc, char * argv []) {
	if ((fd1 = open (argv [1], O_WRONLY | O_CREAT, 0600)) == -1) 
		{ perror ( "open \n"); return EXIT_FAILURE;}
	printf ( "before dup2: descriptor %d \n", fd1);
	dup2 (fd1, STDOUT_FILENO);
	printf ( "after dup2: descriptor %d \n", fd1);
	return EXIT_SUCCESS;
}
















