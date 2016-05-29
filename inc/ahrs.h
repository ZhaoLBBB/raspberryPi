#ifndef _AHRS_H_
#define _AHRS_H_

#include <stdint.h>
class ahrs{
	public:
		ahrs(float sampletime);
		void  update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
		void  update(float gx, float gy, float gz, float ax, float ay ,float az);
		float invSqrt(float x);
		void toEuler(float *thetax, float *thetay, float *thetaz);
		float q0, q1, q2, q3;
		float twoKp, twoKi;
		float integralFBX, integralFBY, integralFBZ;
		float sampleTime;
};
#endif
