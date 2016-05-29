#include "MPU6050.h"
#include "AKM8975.h"
#include "I2C.h"
#include "ahrs.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
MPU6050 accelgyro;
AKM8975 mag;
ahrs MahonyAHRS(0.05f);
float ax, ay, az, gx, gy, gz;
//int16_t mx, my, mz;
float yaw, pitch, roll;
float axf, ayf, azf, gxf, gyf, gzf;
int main(int argc, char **argv)
{
	if(accelgyro.testConnection()){
		printf("Connection OK\n");
	}
	accelgyro.initialize();
//	if(mag.testConnection())
//		printf("AKM8975 connect OK\n");
	while(1){
		usleep(50000);
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//		mag.getMagnetic(&mx, &my, &mz);
		printf("ax:%f,	ay:%f,	az:%f,	gx:%f,	gy:%f, gz:%f\n", ax, ay, az, gx, gy, gz);
		MahonyAHRS.update(gx, gy, gz, ax, ay, az);
		MahonyAHRS.toEuler(&yaw, &pitch, &roll);
		printf("YAW:%f,\tPITCH:%f,\tROLL:%f\n", yaw*57.32f, pitch * 57.32f, roll * 57.32f);
	}
	return 0;
}
