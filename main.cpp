#include "MPU6050.h"
#include "AKM8975.h"
#include "I2C.h"
#include "ahrs.h"
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
//#define MPU6050_DEBUG
MPU6050 accelgyro;
AKM8975 mag;
ahrs AHRS(0.001f);
struct timespec ts;
float ax, ay, az, gx, gy, gz;
//int16_t mx, my, mz;
float yaw, pitch, roll;
float axf, ayf, azf, gxf, gyf, gzf;
float gyroBiasX, gyroBiasY, gyroBiasZ;
int main(int argc, char **argv)
{
	ts.tv_sec = 0;
	ts.tv_nsec = 1 * (1e-3)/(1e-9);
	if(accelgyro.testConnection()){
		printf("Connection OK\n");
	}
	accelgyro.initialize();
//	accelgyro.getGyroBias(&gyroBiasX, &gyroBiasY, &gyroBiasZ);
//	printf("GYRO BIAS:%f\n", gyroBiasX*gyroBiasX + gyroBiasY*gyroBiasY + gyroBiasZ*gyroBiasZ);
//	if(mag.testConnection())
//		printf("AKM8975 connect OK\n");
	while(1){
		nanosleep(&ts, NULL);
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//		mag.getMagnetic(&mx, &my, &mz);
		#ifdef MPU6050_DEBUG
		printf("ax:%f,	ay:%f,	az:%f,	gx:%f,	gy:%f, gz:%f\n", ax, ay, az, gx * 57.32f, gy * 57.32f, gz*57.32f);
		#endif 
//		AHRS.MahonyAHRSupdate(gx, gy, gz, ax, ay, az);
		AHRS.MadgwickAHRSupdate(gx, gy, gz, ax, ay, az);
		AHRS.toEuler(&yaw, &pitch, &roll);
		printf("YAW:%f,\tPITCH:%f,\tROLL:%f\n", yaw*57.32f, pitch * 57.32f, roll * 57.32f);
	}
	return 0;
}
