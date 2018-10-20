compiler:
	gcc -o concurrent-compilation/conc-compiler concurrent-compiler.c -Wall
	cd concurrent-compilation/;./conc-compiler main.c toto.c titi.c tata.c
	concurrent-compilation/my_exec

clean:
	rm -f concurrent-compilation/*.o concurrent-compilation/my_exec concurrent-compilation/conc-compiler 

