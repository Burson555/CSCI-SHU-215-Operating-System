CFLAGS =-Iinclude -Wall -lpthread

all: build run

build_conversion: clean build_server build_client

build_server: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/conversion_server.o src/conversion_server.c
	gcc $(CFLAGS) -o bin/conversion_server obj/conversion_server.o

build_client: # build file sleep.c
	gcc $(CFLAGS) -c -o obj/conversion_client.o src/conversion_client.c
	gcc $(CFLAGS) -o bin/conversion_client obj/conversion_client.o

build_philo: # build file sleep.c
	gcc $(CFLAGS) -lm -c -o obj/philo.o src/philo.c
	gcc $(CFLAGS) -lm -o bin/philo obj/philo.o

clean:
	rm -rf bin/* obj/* opf/*