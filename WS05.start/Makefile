CFLAGS =-Iinclude -Wall -lpthread -lrt

all: build run

run: sleep wave barrier wait

build: clean B1 B2 B3 B4

B1: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/sleep.o src/sleep.c -DtimeSet=5
	gcc $(CFLAGS) -o bin/sleep obj/sleep.o

B2: # build file wave.c
	gcc $(CFLAGS) -c -o obj/wave.o src/wave.c -DnbProcesses=4
	gcc $(CFLAGS) -o bin/wave obj/wave.o

build_server: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/conversion_server.o src/conversion_server.c
	gcc $(CFLAGS) -o bin/conversion_server obj/conversion_server.o

build_client: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/conversion_client.o src/conversion_client.c
	gcc $(CFLAGS) -o bin/conversion_client obj/conversion_client.o

clean:
	rm -rf bin/* obj/* opf/*