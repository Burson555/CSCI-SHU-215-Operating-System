CFLAGS =-Iinclude -Wall -lpthread -lrt

all: build run

run: sleep wave barrier wait

build_conversion: clean build_server build_client

build_server: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/conversion_server.o src/conversion_server.c
	gcc $(CFLAGS) -o bin/conversion_server obj/conversion_server.o

build_client: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/conversion_client.o src/conversion_client.c
	gcc $(CFLAGS) -o bin/conversion_client obj/conversion_client.o

clean:
	rm -rf bin/* obj/* opf/*