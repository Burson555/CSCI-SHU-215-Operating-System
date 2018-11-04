CFLAGS=-Iopf -Wall

all: build run

run: sleep wave barrier

build: clean
	# build file sleep.c
	gcc $(CFLAGS) -c -o obj/sleep.o src/sleep.c
	gcc $(CFLAGS) -o bin/sleep obj/sleep.o
	# build file wave.c
	gcc $(CFLAGS) -c -o obj/wave.o src/wave.c -DnbProcesses=4
	gcc $(CFLAGS) -o bin/wave obj/wave.o
	# build file barrier.c
	gcc $(CFLAGS) -c -o obj/barrier.o src/barrier.c
	gcc $(CFLAGS) -o bin/barrier obj/barrier.o
	# build file wait.c
	gcc $(CFLAGS) -c -o obj/wait.o src/wait.c
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