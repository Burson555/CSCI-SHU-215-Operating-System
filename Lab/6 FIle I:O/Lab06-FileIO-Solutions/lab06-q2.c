#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUFSZ 1024

int main(int argc, char** argv) {
    
    int fd1, fd2, or = 1;
    struct stat info1, info2;
    void* buffer = malloc(BUFSZ);
    
    fd1 = open(argv[1], O_RDWR, 0600);
    if (fd1 == -1) {
        perror("Pb à l'ouverture du fichier source");
        exit(1);
    }
    fstat(fd1, &info1);
    if (!(S_ISREG(info1.st_mode))) {
        perror("Fichier source n'est pas régulier");
        exit(1);
    }
    
    if (stat(argv[2], &info2) != -1) {
        perror("Fichier cible existe");
        exit(3);
    }
    fd2 = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0600);
    if (fd2 == -1) {
        perror("Pb à l'ouverture du fichier cible");
        exit(3);
    }
    
    while(or > 0) {
        or = read(fd1, buffer, BUFSZ);
        write(fd2, buffer, or);
    }
    
    close(fd1);
    close(fd2);
    
    return 0;
    
}
