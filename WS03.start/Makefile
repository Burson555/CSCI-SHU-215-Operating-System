CFLAGS=-Iinclude -Wall

FILE=tasks4

all: build run

build: clean
	mkdir obj
	gcc $(CFLAGS) -c -o obj/policies.o src/sched-policies.c	
	gcc $(CFLAGS) -c -o obj/simulator.o src/sched-simulator.c
	mkdir bin
	gcc $(CFLAGS) -o bin/sched-simulator obj/simulator.o obj/policies.o

run:
#	bin/sched-simulator $(FILE) FCFS 1>sched-run-fcfs.txt
#	bin/sched-simulator $(FILE) SJF 1>sched-run-sjf.txt
#	bin/sched-simulator $(FILE) SRTF 1>sched-run-srtf.txt
#	bin/sched-simulator $(FILE) RR 4 1>sched-run-rr.txt
	bin/sched-simulator $(FILE) RR 4
	cat *.txt

clean:
	rm -rf bin obj sched-run-*.txt