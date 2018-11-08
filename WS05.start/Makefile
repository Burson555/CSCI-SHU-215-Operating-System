CFLAGS1 =-Wall -lrt
CFLAGS2 =-Wall -lpthred
CFLAGS3 =-Wall -lpthred -lrt


all: build run

run: sleep wave barrier wait

build: clean B1 B2 B3 B4

B1: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/sleep.o src/sleep.c -DtimeSet=5
	gcc $(CFLAGS) -o bin/sleep obj/sleep.o

B2: # build file wave.c
	gcc $(CFLAGS) -c -o obj/wave.o src/wave.c -DnbProcesses=4
	gcc $(CFLAGS) -o bin/wave obj/wave.o

B3: # build file barrier.c
	gcc $(CFLAGS) -c -o obj/barrier.o src/barrier.c
	gcc $(CFLAGS) -o bin/barrier obj/barrier.o

B4: # build file wait.c
	gcc $(CFLAGS) -c -o obj/wait.o src/wait.c -DN=4
	gcc $(CFLAGS) -o bin/wait obj/wait.o

sleep:
	bin/sleep

wave:
	bin/wave

barrier:
	bin/barrier

wait:
	bin/wait

clean:
	rm -rf bin/* obj/* opf/*