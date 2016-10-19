/*
	Serial-read  two sensor values
	Temprature sensor and accelerometer
*/
#include <letmecreate/letmecreate.h>
#include <stdio.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#define STEPS 10000

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

double temp(){
    printf("Reading Tempratures\n");
    float temperature = 0.f;
    double sum  = 0.0;
    struct timespec begin, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&begin);
    i2c_init();
    i2c_select_bus(MIKROBUS_1);
  
    thermo3_click_enable(0);
    for(int i=0;i<STEPS;i++){	
    	thermo3_click_get_temperature(&temperature);
	sum += temperature;
    }
    
    thermo3_click_disable();

    i2c_release();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end);
    printf("Average Temprature  %.3f°C\n",(sum/STEPS));
    double seconds = diff(begin, end).tv_sec;
    double ns = diff(begin,end).tv_nsec;
    double time = roundf((seconds + (ns/1000000000))*100)/100;	
    return time;

}

double read_accel(){
	printf("Reading accelerometer values\n");
        struct accel *a;
	double roll, pitch;
        a = malloc(sizeof(struct accel)*STEPS);
        struct timespec begin, end;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&begin);
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
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&end);
        double seconds = diff(begin, end).tv_sec;
        double ns = diff(begin,end).tv_nsec;
        double time = roundf((seconds + (ns/1000000000))*100)/100;  
        printf("Current roll and pitch %0.3f and %0.3f\n", roll, pitch);
	free(a);
	return time;
		
}

int main(int argc, char **argv){	
	double time = temp();
	time += read_accel();
	printf("Time Taken serial execution(s):  %f\n",time);
		
	return 0;	
}
