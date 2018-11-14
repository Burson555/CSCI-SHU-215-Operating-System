Bosen Yang by570


####################################################################################################
compiling rules START
####################################################################################################

make: running all sources files, i.e. files for all questions

make build_conversion: transforming all Q1 source files into executables
make build_server: compile server source file to executable
make build_client: compile client source file to executable
make test_conversion: test the code



make clean: remove all files in bin/, src/ and lib/ for a fresh restart

####################################################################################################
compiling rules END
####################################################################################################



####################################################################################################
files description START
####################################################################################################

include/conversion.h: header file for both conversion_client and conversion_server
bin/conversion_client: executable file for client
bin/conversion_server: executable file for server
src/conversion_client: source file for client
src/conversion_server: source file for server


####################################################################################################
files description END
####################################################################################################





####################################################################################################
Q1 COMMENTS START
####################################################################################################

Here's the logic behind my code (the following behaviors should happen in chronological order):
	1.	upon initiating the server
			-1 create a shared memory shm_server
			-2 create N child processes to handle N different currencies
			-3 P the server to wait to conversion request
			-4 set mask in child processes, masking all but SIGUSR1
			-5 pause() the child processes awaiting signals 
	2.	upon initiating the client
			-1 write conversion information into client created shared memory shm_client
			-2 write conversion information into server created shared memory shm_server
			-3 V the server
			-4 P itself waiting for conversion result
	3.	after server receives a request
			-1 send SIGUSR1 to the corresponding child to convert to targeted currency
			-2 this child copies what's in the shm_server to local memory
			-3 this child calculates and write into shm_client
			-4 V the client
			-5 client prints the result
			-6 server goes back to 1-3

Several semaphores are needed for realization of the behaviors described above
Then I'll list the semaphores and corresponding critical resources and where to insert the semaphores
	1.	most obvious semaphores --- consume and produce semaphore:
		just another application of the consumer producer model
		where the server is the producer and the client is the consumer
	2. shm_server --- sem_server_mutex: 
		after a client starts writing the currency info and before a child process stores such info to its local memory
		no other processes should touch the shm_server in this window
		so we need a P at 2-2 and a V immediately after 3-2
	3. child local memory --- sem_child_protect:
		after a child starts calculating and before it finishes the current work
		additional work for a currently working child should wait until the child finishes last work
		because this time the child is unable to read and store the infomation in shm_server
		before the child finished the current work
		other clients can submit request and gets the info in shm_server rewritten
		then a request is lost and a client would be waiting forever
		so we need a P at 3-1 and a V immediately after 2-4

####################################################################################################
Q1 COMMENTS END
####################################################################################################





####################################################################################################
Q2 COMMENTS START
####################################################################################################

I found the LINUX will automatically perform context switch after child processes run kill.
Thus, some processes will receive and deliver the signal (SIGUSR1 or SIGUSR2) before calling pause.
Then since there will no longer be signals arriving, the process will pause forever.
Under this condition, I added some sleep in between to bring about the situation we want.
I also did the same(inserting sleeps) for question 3 and question 4.

####################################################################################################
Q2 COMMENTS END
####################################################################################################


