CFLAGS =-Iinclude -Wall -lpthread

all: build run

build: clean build_upper build_upperN build_bounded-buffer build_threads-vs-signals

build_upper: 
	gcc $(CFLAGS) -c -o obj/upper.o src/upper.c
	gcc $(CFLAGS) -o bin/upper obj/upper.o

build_upperN: 
	gcc $(CFLAGS) -c -o obj/upperN.o src/upperN.c
	gcc $(CFLAGS) -o bin/upperN obj/upperN.o

build_bounded-buffer: 
	gcc $(CFLAGS) -c -o obj/bounded-buffer.o src/bounded-buffer.c
	gcc $(CFLAGS) -o bin/bounded-buffer obj/bounded-buffer.o

build_threads-vs-signals: 
	gcc $(CFLAGS) -c -o obj/threads-vs-signals.o src/threads-vs-signals.c
	gcc $(CFLAGS) -o bin/threads-vs-signals obj/threads-vs-signals.o

run: run_Q1 run_Q2 run_Q3 run_Q4

run_Q1:
	bin/upper test1/test1.txt test1/test2.txt test1/test3.txt test1/test4.txt

run_Q2:
	bin/upperN test1/test1.txt test1/test2.txt test1/test3.txt test1/test4.txt

run_Q3:
	bin/bounded-buffer

run_Q4:
	bin/threads-vs-signals

clean:
	rm -rf bin/* obj/* opf/*