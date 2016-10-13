#include <letmecreate/letmecreate.h>
#include <stdio.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#define STEPS 10000
pthread_barrier_t barr;


struct accel{
        float X;
        float Y;
        float Z;
};
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

void* temp(void *args){
    printf("Scheduled Temperature Thread ID: %lu, CPU: %d\n", pthread_self(), sched_getcpu());
    printf("Reading Temperatures\n");
    struct timespec begin,end;
 
    float temperature = 0.f;
    double sum  = 0.0;
    //Synchronization point
    int rc = pthread_barrier_wait(&barr);
    if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD){
        printf("Could not wait on barrier\n");
        exit(-1);
    }
   
    clock_gettime(CLOCK_THREAD_CPUTIME_ID,&begin);
    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    thermo3_click_enable(0);
    for(int i=0;i<STEPS;i++){   
        thermo3_click_get_temperature(&temperature);
        sum += temperature;
    }           
                
    thermo3_click_disable();

    i2c_release();
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);    
    printf("Average Temprature %0.3f\n",(sum/STEPS));
    double seconds = diff(begin, end).tv_sec;
    double ns = diff(begin,end).tv_nsec;
    double time = roundf((seconds + (ns/1000000000))*100)/100;
    printf("Time Taken temprature thread execution(s):  %f\n",time);

    return 0;           
}

void* read_accel(void *args){
    	printf("Scheduled Accelerometer Thread ID: %lu, CPU: %d\n", pthread_self(), sched_getcpu());
	
        printf("Reading accelerometer values\n");
	struct accel *a;
	double roll,pitch;
	struct timespec begin,end;
        a = malloc(sizeof(struct accel)*STEPS);
        //Synchronization point
    	int rc = pthread_barrier_wait(&barr);
    	if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD){
        	printf("Could not wait on barrier\n");
        	exit(-1);
    	}
	clock_gettime(CLOCK_THREAD_CPUTIME_ID,&begin);
        spi_init();
        spi_select_bus(MIKROBUS_2);
        accel_click_enable();
        for(int i=0;i<STEPS;i++){
                accel_click_get_measure(&a[i].X, &a[i].Y, &a[i].Z);
                roll = atan2(a[i].Y, a[i].Z) * 180/M_PI;
                pitch = atan2(-a[i].X, sqrt(a[i].Y*a[i].Y + a[i].Z*a[i].Z)) * 180/M_PI;


        }
        accel_click_disable();
        spi_release();
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
	double seconds = diff(begin, end).tv_sec;
    	double ns = diff(begin,end).tv_nsec;
    	double time = roundf((seconds + (ns/1000000000))*100)/100;
	printf("Current roll and pitch %0.3f and %0.3f\n",roll,pitch); 
    	printf("Time Taken accel thread execution(s):  %f\n",time);
	free(a);
        return 0;        
} 
 
int main(int argc, char **argv){

	//Use cpus 2 & 3 to schedule threads
	int cpu_index[2] = {2,3};
        int no_cpus = 2;
        pthread_t threads[no_cpus];
	pthread_attr_t attr;
    	cpu_set_t cpus;
    	int rc;
    	pthread_attr_init(&attr);
    	// Barrier initialization
    	if(pthread_barrier_init(&barr, NULL, no_cpus)){
        	printf("Could not create a barrier\n");
        	return -1;
    	}

    	//schedule threads on online processors or passed arguments
   	for (int i = 0; i < no_cpus; i++) {
       		CPU_ZERO(&cpus);
       		CPU_SET(cpu_index[i], &cpus);
       		rc = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
       		if(rc!=0){
                	printf("Error spawning threads\n");
       		}
		if(i==0)
       			pthread_create(&threads[i], &attr, temp, NULL);
		else
			pthread_create(&threads[i], &attr, read_accel,NULL);
    	}

    	for (int i = 0; i < no_cpus; i++) {
        	pthread_join(threads[i], NULL);
    	}
	
	return 0;
}

