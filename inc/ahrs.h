#ifndef _AHRS_H_
#define _AHRS_H_

#include <stdint.h>
class ahrs{
	public:
		ahrs(float sampletime);
		void  MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
		void  MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay ,float az);
		float invSqrt(float x);
		void toEuler(float *thetax, float *thetay, float *thetaz);
		
		void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az); 
		void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz); 
		float q0, q1, q2, q3;
		float twoKp, twoKi;
		float integralFBX, integralFBY, integralFBZ;
		float sampleTime;
		float beta;
};
#endif
