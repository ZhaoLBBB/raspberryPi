#include "MPU6050.h"
#include "AKM8975.h"
#include "I2C.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
MPU6050 accelgyro;
AKM8975 mag;
int16_t ax, ay, az, gx, gy, gz;
int16_t mx, my, mz;
float axf, ayf, azf, gxf, gyf, gzf;
int main(int argc, char **argv)
{
	if(accelgyro.testConnection()){
		printf("Connection OK\n");
	}
	accelgyro.initialize();
	if(mag.testConnection())
		printf("AKM8975 connect OK\n");
	while(1){
		usleep(100000);
		//accelgyro.getAcceleration(&ax, &ay, &az);
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		axf = (float)ax/32768.0f * 2.0f * 9.8f;
		ayf = (float)ay/32768.0f * 2.0f * 9.8f;
		azf = (float)az/32768.0f * 2.0f * 9.8f;
	//	gxf = (float)gx/32768.0f * 250.0f;
	//	gyf = (float)gy/32768.0f * 250.0f;
	//	gzf = (float)gz/32768.0f * 250.0f;
		mag.getMagnetic(&mx, &my, &mz);
		printf("ax:%f,	ay:%f,	az:%f,	mx:%d,	my:%d, mz:%d\n", axf, ayf, azf, mx, my, mz);
	}
	return 0;
}
