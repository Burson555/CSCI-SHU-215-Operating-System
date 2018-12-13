Bosen Yang by570


####################################################################################################
compiling rules START
####################################################################################################

make clean: remove all files in bin/, src/ and lib/ for a fresh restart

Question 1
make build_conversion: transforming all Q1 source files into executables
make build_server: compile server source file to executable
make build_client: compile client source file to executable
make test_conversion: test the code

Question 2
make build_philo: compile source file to executable


####################################################################################################
compiling rules END
####################################################################################################



####################################################################################################
files description START
####################################################################################################

include/conversion.h: header file for both conversion_client and conversion_server
bin/conversion_client: executable file for client
bin/conversion_server: executable file for server
src/conversion_client.c: source file for client
src/conversion_server.c: source file for server

bin/philo: executable file for server
src/philo.c: source file for client

####################################################################################################
files description END
####################################################################################################



####################################################################################################
test START
####################################################################################################

Question 1
you should build first
then run server program in one bash and run client in another
sample code:
	bin/conversion_server aa
	bin/conversion_client aa bb GBP 100
	bin/conversion_client aa bb USD 100
	bin/conversion_client aa bb EUR 100

Question 2
the program requires 2 arguments
the first argument indicates how many philosophers are there
while the second argument indicates how many rounds the philosophers thinks
(in our case, for convenience of testing, we don't want the philosophers' behaviors to loop forever)
for example, "bin/philo 9 4" will present a condition where 9 philosophers each think for 4 rounds


####################################################################################################
test END
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
			-1 send SIGUSR1 to every child to convert to the corresponding currency
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
		the parent waits for all its children to finish their work and then tells the client to consume
		so we need a P at 3-1 and a V immediately after 2-4

####################################################################################################
Q1 COMMENTS END
####################################################################################################





####################################################################################################
Q2 COMMENTS START
####################################################################################################

We have 2 kinds of semaphores.

sem_chop
The first kind, regards each chopstick as a critical resource.
A process/philosopher will P one semaphore when it takes a chopstick.
By default, a philosopher first takes the chopstick on the left and then on the right.
Under this condition, a deadlock may happen when all philosophers takes the chopstick on the loft at the same time.

sem_philo
Then we introduce the second kind of semaphore to prevent such deadlock.
This semaphore functions as a mutex for two neighboring philosophers, so that only on philosopher is allowed to take the chopstick between them and the other one keeps waiting.

####################################################################################################
Q2 COMMENTS END
####################################################################################################


