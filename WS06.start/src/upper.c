#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>




int main (int argc, char ** argv) {
    FILE *fp1, *fp2;
    char dest_fname[128];
    int c = 1;
    
    fp1 = fopen (argv[1], "r");
    strcpy(dest_fname, argv[1]);
    strcat(dest_fname, ".UPPER.txt");
    printf("%s\n", dest_fname);
    fp2 = fopen (dest_fname, "w");
    if ((fp1 == NULL) || (fp2 == NULL)) {
        perror ( "fopen");
        exit (1);
    }
    
    while (c != EOF) {
        c = fgetc(fp1);
        if (c != EOF)
            fputc(toupper(c), fp2);
//            fputc(toupper(c), stdout);
    }
    
    fclose (fp1);
    fclose (fp2);
    
    return EXIT_SUCCESS;
}
