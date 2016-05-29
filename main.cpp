#include "MPU6050.h"
#include "AKM8975.h"
#include "I2C.h"
#include "ahrs.h"
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
MPU6050 accelgyro;
AKM8975 mag;
ahrs AHRS(0.001f);
struct timespec ts;
float ax, ay, az, gx, gy, gz;
//int16_t mx, my, mz;
float yaw, pitch, roll;
float axf, ayf, azf, gxf, gyf, gzf;
int main(int argc, char **argv)
{
	ts.tv_sec = 0;
	ts.tv_nsec = 1* (1e-3)/(1e-9);
	if(accelgyro.testConnection()){
		printf("Connection OK\n");
	}
	accelgyro.initialize();
//	if(mag.testConnection())
//		printf("AKM8975 connect OK\n");
	while(1){
		nanosleep(&ts, NULL);
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//		mag.getMagnetic(&mx, &my, &mz);
//		printf("ax:%f,	ay:%f,	az:%f,	gx:%f,	gy:%f, gz:%f\n", ax, ay, az, gx, gy, gz);
		//AHRS.MahonyAHRSupdate(gx, gy, gz, ax, ay, az);
		AHRS.MadgwickAHRSupdate(gx, gy, gz, ax, ay, az);
		AHRS.toEuler(&yaw, &pitch, &roll);
		printf("YAW:%f,\tPITCH:%f,\tROLL:%f\n", yaw*57.32f, pitch * 57.32f, roll * 57.32f);
	}
	return 0;
}
