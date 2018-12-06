#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSZ 1024


int checkfile(char* fname, char* input) {
    int fd, or = 1;
    char* buffer = malloc(BUFSZ*sizeof(char));
    
    fd = open(fname, O_RDONLY, 0400);
    if (fd == -1) {
        perror("can't open");
        exit(1);
    }
    while(or > 0) {
        or = read(fd, buffer, BUFSZ);
        if (strstr(buffer, input) != NULL) {
            return 1;
        }
        // to avoid the case when a targeted array is half in half out
        if (or == BUFSZ)
            lseek(fd, -strlen(input), SEEK_CUR);
    }
    return 0;
}


int main(int argc, char** argv) {
    DIR *pt_Dir;
    struct dirent* dirEnt;
    struct stat info;
    
    if ((pt_Dir = opendir (argv[2]) ) == NULL) {
        perror ("opendir");
        return EXIT_FAILURE;
    }
    
    while ((dirEnt = readdir(pt_Dir)) !=NULL) {
        printf("checking entry...\n");
        stat(dirEnt->d_name, &info);
        if (S_ISREG(info.st_mode)) {
            printf("find a regular file\n");
            printf ("Checking %s... ", dirEnt->d_name);
            if (checkfile(dirEnt->d_name, argv[1]) == 1)
                printf("Yes!\n");
            else
                printf("Nope\n");
        }
    }
    closedir (pt_Dir);
    
    return EXIT_SUCCESS;
    
}
