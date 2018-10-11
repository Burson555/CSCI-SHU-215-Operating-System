#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <unistd.h>

int main () {
	pause ();
	printf ("1er signal\n");
	pause ();
	printf ("2e signal\n");
	return 0;
}
