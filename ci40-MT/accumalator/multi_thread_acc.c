/*
multi_thread_acc.c

This program runs a accumulator like workload, that sums up 'x' elements
in a loop.

The program also uses CPU_SET to set thread affinity.

The program accepts logical core ID as arguments

Example:
========
	./multi_thread 0 1

Where 0 and 1 are logical cores

The list of available logical cores can be found by using

	cat /proc/cpuinfo

OUTPUT is reported-per-thread

Time is reported as 

x seconds 


COMPILATION
===========
mips-linux-gnu-gcc multi_thread_acc.c -std=c99 -Wall -O3  -g -EL -DRTELINUX -D_GNU_SOURCE -march=1004kc -DNDEBUG -pthread -static -o ./multi_thread_acc -lpthread -lrt -lm

*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
//global data

#define DATA_SIZE 10000

#define MAX_LOGIC_CORES 4

pthread_mutex_t lock;

pthread_barrier_t barr;

int result=0;
/*
This functions takes start and end times and computes diffrence in seconds and remainging nano seconds granularity
*/
struct timespec diff(struct timespec start,struct timespec end){
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
/*
  This function represents the accumulator workload 
  Prints the CPU it is scheduled on 
  Loops over elements and adds them to the local sum
  Every thread has it's own set of inputs
  A Barrier is set till all values are initialized
*/
void* workload(void* args) {
    printf("Scheduled Thread ID: %lu, CPU: %d\n", pthread_self(), sched_getcpu());
    int sum=0;
    
    int i;
    int *a;
    a = malloc(sizeof(a)*DATA_SIZE);
    for (int i =0;i< DATA_SIZE;i++){
         a[i]= rand()%1000 + 1;
    }
   
     //Synchronization point
    int rc = pthread_barrier_wait(&barr);
    if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD){
        printf("Could not wait on barrier\n");
        exit(-1);
    } 
    struct timespec begin,end;
    //start the time
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &begin);
    for (int j=0; j < 10000; j++) {
     for(i=0 ;i< DATA_SIZE; i++)
     {
        sum += a[i];
     
     }
    }
     
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
    //stop the clock and compute timr
    double time_spent = diff(begin,end).tv_nsec;
    double time_spent_s = diff(begin,end).tv_sec;
    pthread_mutex_lock(&lock);
    result += sum;
    pthread_mutex_unlock(&lock);
    double time = roundf((time_spent_s + (time_spent/1000000000))*100)/100;
    //Print time taken by thread
    printf("Thread ID: %lu, CPU: %d Time taken(s): %f\n",pthread_self(),sched_getcpu(),time);
    free(a);    
    return 0;    
}

/*
 The MAIN function
  Sets thread affinity using arguments or available logical cores
  Then it initiates a pthread barrier and spawns the threads
*/
int main(int argc , char **argv) {   

    int numberOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);
    int *cpu_index, passed_cpus;
    //Display number of processors in system
    
    printf("Number of processors: %d\n", numberOfProcessors);
    if (argc > 1){
         passed_cpus = argc -1;
         cpu_index = malloc(sizeof(int)*passed_cpus);
    }
    else{
        cpu_index = malloc(sizeof(int)*numberOfProcessors);
        passed_cpus = numberOfProcessors;
    }
    pthread_t threads[passed_cpus];

    pthread_attr_t attr;
    cpu_set_t cpus;
    int rc, j = 0;
    pthread_attr_init(&attr);
    //detect CPUs that are online or use CPUS passed in argument list
    if(argc < 2){
    	CPU_ZERO(&cpus);
    	sched_getaffinity(0, sizeof(cpu_set_t), &cpus);

    	for(int i = 0; i < MAX_LOGIC_CORES; i++){
        	if(CPU_ISSET(i,&cpus)){
           		cpu_index[j] = i;
           		j++;
        	}

    	}
    }
    else{
          for(int count =1; count < argc; count++){
              cpu_index[j] = atoi(argv[count]);
              j++;
          }
    }
    

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }
    // Barrier initialization
    if(pthread_barrier_init(&barr, NULL, passed_cpus)){
        printf("Could not create a barrier\n");
        return -1;
    }

    //schedule threads on online processors or passed arguments
    for (int i = 0; i < passed_cpus; i++) {
       CPU_ZERO(&cpus);
       CPU_SET(cpu_index[i], &cpus);
       rc = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
       if(rc!=0){
		printf("Error spawning threads\n");
       }
       pthread_create(&threads[i], &attr, workload, NULL);
    }
    
    for (int i = 0; i < passed_cpus; i++) {
        pthread_join(threads[i], NULL);
    }
	
    free(cpu_index);
    return 0;
}
