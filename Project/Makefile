# the rules in this Makefile file only compile the programs needed
# which are 3 client requests (deployment, status and result), master server launch and worker server launch
CFLAGS =-Iinclude -Wall 
# CFLAGS =-Iinclude -Wall -lpthread 

all: build 

build: clean build_master build_client build_worker

build_worker: 
	gcc $(CFLAGS) -c -o obj/worker.o src/worker.c
	gcc $(CFLAGS) -o bin/worker obj/worker.o

build_master: 
	gcc $(CFLAGS) -c -o obj/master.o src/master.c
	gcc $(CFLAGS) -o bin/master obj/master.o

build_client: build_deploy build_status build_result

build_deploy:
	gcc $(CFLAGS) -c -o obj/deploy.o src/deploy.c
	gcc $(CFLAGS) -o bin/deploy obj/deploy.o
build_status:
	gcc $(CFLAGS) -c -o obj/status.o src/status.c
	gcc $(CFLAGS) -o bin/status obj/status.o
build_result:
	gcc $(CFLAGS) -c -o obj/result.o src/result.c
	gcc $(CFLAGS) -o bin/result obj/result.o

clean:
	rm -rf bin/* obj/* opf/* clt_file/* mst_file/* wkr_file/*