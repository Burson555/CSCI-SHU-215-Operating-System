#define _XOPEN_SOURCE 700
/* includes <stdio.h>, <pthread.h>, <stdlib.h>, and <unistd.h> */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// gcc -Wall -lpthread


// page 23 example INCOMPLETE
void *test (void *arg) { 
	int i;
	printf ( "%s Argument received, tid:%d \n", (char *) arg, (int) pthread_self ()); 
	for (i = 0; i <1E6; i ++);
	printf ( "end of secondary thread %d\n", (int) pthread_self ()); 
	return NULL;
}
// int main (int argc, char ** argv) {
// 	pthread_t tid;
// 	if (pthread_create(&tid, NULL, test, "HELLO") != 0) { 
// 		perror("pthread_create\n");
// 		exit (1); 
// 	} 
// 	sleep (3);
// 	printf ( "end of main thread\n");
// 	return EXIT_SUCCESS;
// }



// page 26 example INCOMPLETE
// #define NUM_THREADS 4
// void * func_thread (void * arg) {
// 	printf ("Argument received:%s, thread_id:%d\n", (char*)arg, (int) pthread_self ());
// 	pthread_exit ((void *) 0); 
// }
// int main (int argc, char ** argv) { 
// 	int i; pthread_t tid[NUM_THREADS]; 
// 	for (i = 0; i < NUM_THREADS; i ++) {
// 		if (pthread_create(&(tid[i]), NULL, func_thread, argv[i+1]) != 0) {
// 			printf ( "pthread_create \n"); exit (1);}
// 	}
// 	sleep (3);
// 	for (i = 0; i < NUM_THREADS; i ++) {
// 		printf ( "tid:%d \n", (int) tid[i]); 
// 	}
// 	return EXIT_SUCCESS;
// }



// page 28 example INCOMPLETE
// #define NUM_THREADS 2
// void * func_thread (void * arg) {
// 	printf("tid:%d> %s\n", (int)pthread_self (), (char*)arg); 
// 	pthread_exit ((void *) 0);
// }
// int main (int argc, char ** argv) {
// 	int i, status; pthread_t tid[NUM_THREADS];
// 	for (i = 0; i <NUM_THREADS; i ++)
// 		if (pthread_create (&(tid[i]), NULL, func_thread, argv[i+1]) != 0) { 
// 			printf ( "pthread_create \n"); 
// 			exit (1); } 
// 	for (i = 0; i <NUM_THREADS; i ++) {
// 		if (pthread_join (tid [i], (void* *) & status) != 0) { 
// 			printf ( "pthread_join"); 
// 			exit (1); 
// 		} else
// 			printf ( "%d Thread finished with status:%d \n", i, status);
// 	}
// 	return EXIT_SUCCESS; 
// }





// page 33 example INCOMPLETE
// #define NUM_THREADS 3
// void* test (void * arg) {
// for (int j = 0; j < NUM_THREADS; j ++) {
// for (int i = 0; i <1000; i ++);
// printf ( "thread %d \n", (int) pthread_self ()); sched_yield();
// }
// return NULL; }
// int main (int argc, char ** argv) { pthread_t tid[NUM_THREADS]; int i; for (i = 0; i <NUM_THREADS; i ++)
// if (pthread_create (&tid[i], NULL, test, NULL) != 0) { perror ( "pthread_create \n"); exit (1); }
// sleep (3);
// printf ( "end of thread main \n");  return EXIT_SUCCESS;
// }





// page 38 example INCOMPLETE
// int cont = 0; 
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// void * sum_thread (void * arg) { 
// 	pthread_mutex_lock(&mutex); cont ++; 
// 	pthread_mutex_unlock(&mutex); 
// 	pthread_exit ((void *) 0);
// }
// int main (int argc, char ** argv) {
// 	pthread_t tid;
// 	if (pthread_create (& tid, NULL, sum_thread, NULL) != 0)
// 		{ printf ( "pthread_create"); 
// 		exit (1); 
// 	} 
// 	pthread_mutex_lock(&mutex);
// 	cont ++;
// 	pthread_mutex_unlock (&mutex);
// 	pthread_join (tid, NULL);
// 	printf ("cont:%d \n", cont);
// 	return EXIT_SUCCESS;
// }





// page 43 example INCOMPLETE
pthread_mutex_t mutex_end = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond_end = PTHREAD_COND_INITIALIZER;
void * func_thread (void * arg) {
	printf ( "tid:%ld \n", (long) pthread_self ()); 
	pthread_mutex_lock(&mutex_end); 
	pthread_cond_signal (&cond_end); 
	pthread_mutex_unlock (&mutex_end); 
	pthread_exit ((void *) 0);
}
int main (int argc, char ** argv) {
	pthread_t tid;
	pthread_mutex_lock (&mutex_end);
	if (pthread_create (&tid, NULL, func_thread, NULL) != 0) { 
		printf ( "pthread_create error \n"); 
		exit (1);
	} 
	if (pthread_detach (tid) != 0) { 
		printf ( "pthread_detach error"); 
		exit (1); 
	}
	pthread_cond_wait (&cond_end, &mutex_end); 
	pthread_mutex_unlock (&mutex_end);
	printf ( "End thread \n");
	return EXIT_SUCCESS;
}





// page 44 example INCOMPLETE
// #define _XOPEN_SOURCE 700
// int flag = 0;
// pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// void * func_thread (void * arg) {
// 	pthread_mutex_lock(&​​m); 
// 	while (! flag)
// 		pthread_cond_wait (& cond, & m); 
// 	pthread_mutex_unlock(&​​m); 
// 	pthread_exit ((void *) 0);
// }
// int main (int argc, char ** argv) {
//     pthread_t tid1, tid2;
//     if ((pthread_create (&tid1, NULL, func_thread, NULL) != 0)
//           || (pthread_create (& tid2, NULL, func_thread, NULL)! = 0))
//           { printf ( "pthread_create error \n"); 
//       	exit (1); 
//     }
// 	sleep (1);
// 	pthread_mutex_lock (& ​​m);
// 	flag = 1;
// 	pthread_cond_broadcast (& cond, & m); pthread_mutex_unlock(&​​m);
//     pthread_join (tid1, NULL);
//     pthread_join (tid2, NULL);
//     return EXIT_SUCCESS;
// }









