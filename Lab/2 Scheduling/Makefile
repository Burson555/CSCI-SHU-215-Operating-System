CFLAGS=-Iinclude -Wall

all: build run

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

run:
	bin/sched-simulator "tasks" 1>sched-run.txt
	cat sched-run.txt

run2:
	bin/sched-simulator "tasks2" 1>sched-run.txt
	cat sched-run.txt

clean:
	rm -rf bin obj sched-run.txt