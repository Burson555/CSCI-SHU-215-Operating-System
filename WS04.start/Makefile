CFLAGS=-Iopf -Wall

all: build run

run: sleep wave barrier

build: clean
	# build file sleep.c
	gcc $(CFLAGS) -c -o obj/sleep.o src/sleep.c
	gcc $(CFLAGS) -o bin/sleep obj/sleep.o
	# build file wave.c
	gcc $(CFLAGS) -c -o obj/wave.o src/wave.c -DnbProcesses=8
	gcc $(CFLAGS) -o bin/wave obj/wave.o
	# build file barrier.c
	gcc $(CFLAGS) -c -o obj/barrier.o src/barrier.c
	gcc $(CFLAGS) -o bin/barrier obj/barrier.o

sleep:
	bin/sleep 1 > opf/sleep.txt
	cat opf/sleep.txt

wave:
	bin/wave 1 > opf/wave.txt
	cat opf/wave.txt

barrier:
	bin/barrier 1 > opf/barrier.txt
	cat opf/barrier.txt

clean:
	rm -rf bin/* obj/* opf/*