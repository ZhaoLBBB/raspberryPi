#include "ahrs.h"
#include <math.h>
ahrs::ahrs(float samplerate)
{
	q0 = 1.0f; q1 = 0.0f; q2 = 0.0f; q3 = 0.0f;
	sampleTime = samplerate;
	integralFBX = 0.0f; integralFBY = 0.0f; integralFBZ = 0.0f;
	twoKp = 2.0f; twoKi = 0.0f;
}

void ahrs::update(float gx, float gy, float gz, float ax, float ay, float az)
{
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	recipNorm = invSqrt(ax*ax + ay*ay + az*az);
	ax *= recipNorm; 
	ay *= recipNorm;
	az *= recipNorm;

	halfvx = q1*q3 - q0 * q2;
	halfvy = q2*q3 + q0 * q1;
	halfvz = 0.5f * (q0*q0 - q1*q1 - q2*q2 + q3*q3);
	
	halfex = (ay * halfvz - az * halfvy);
	halfey = (az * halfvx - ax * halfvz);
	halfez = (ax * halfvy - ay * halfvx);
	
	if(twoKi > 0.0f){
		integralFBX += twoKi * halfex * (sampleTime);
		integralFBY += twoKi * halfey * (sampleTime);
		integralFBZ += twoKi * halfez * (sampleTime);
		gx += integralFBX;
		gy += integralFBY;
		gz += integralFBZ;
	
	}else{
		integralFBX = 0.0f;
		integralFBY = 0.0f;
		integralFBZ = 0.0f;
	}	
	gx += twoKp * halfex;
	gy += twoKp * halfey;
	gz += twoKp * halfez;

	gx *= (0.5f * sampleTime);
	gy *= (0.5f * sampleTime);
	gz *= (0.5f * sampleTime);
	
	qa = q0; qb = q1; qc = q2;
	q0 += (-qb * gx - qc * gy - q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx);

	recipNorm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

void ahrs::update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
	float recipNorm;
	float hx, hy, bz, bx;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;
	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;
	
	recipNorm = invSqrt(ax*ax + ay*ay + ax*az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;
	recipNorm = invSqrt(mx*mx + my*my + mx*mz);
	mx *= recipNorm;
	my *= recipNorm;
	mz *= recipNorm;

	hx = 2.0f * (mx * 0.5f * (q0q0 + q1q1 - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 - q0q2));
	hy = 2.0f * (mx * (q1q2 + q0q3) + 0.5f * my * (q0q0 - q1q1 + q2q2 - q3q3) + mz * (q2q3 - q0q1));
	bz = 2.0f * (mx * (q1q3 - q0q2) + mz * (q2q3 + q0q3) + mz * 0.5f * (q0q0 - q1q1 - q2q2 + q3q3));
	bx = sqrt(hx * hx + hy * hy);
	
	halfvx = q1q3 - q0q2;
	halfvy = q2q3 + q0q1;
	halfvz = 0.5f * (q0q0 - q1q1 - q2q2 + q3q3);
	
	halfwx = bx * 0.5f *(q0q0 + q1q1 - q2q2 -q3q3) + bz * (q1q3 - q0q2);
	halfwy = bx * (q1q2 - q0q3) + bz * (q2q3 + q0q1);
	halfwz = bx * (q1q3 + q0q2) + bx * 0.5f * (q0q0 - q1q1 - q2q2 + q3q3);
	
	halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
	halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
	halfez = (ax * halfvy - ay * halfvx) + (mx * halfwz - my * halfwx);
	if(twoKi > 0.0f){
		integralFBX += twoKi * halfex * (sampleTime);
		integralFBY += twoKi * halfey * (sampleTime);
		integralFBZ += twoKi * halfez * (sampleTime);
		gx += integralFBX;
		gy += integralFBY;
		gz += integralFBZ;
	
	}else{
		integralFBX = 0.0f;
		integralFBY = 0.0f;
		integralFBZ = 0.0f;
	}	
	gx += twoKp * halfex;
	gy += twoKp * halfey;
	gz += twoKp * halfez;

	gx *= (0.5f * sampleTime);
	gy *= (0.5f * sampleTime);
	gz *= (0.5f * sampleTime);
	
	qa = q0; qb = q1; qc = q2;
	q0 += (-qb * gx - qc * gy -q3 * gz);
	q1 += (qa * gx + qc * gz - q3 * gy);
	q2 += (qa * gy - qb * gz + q3 * gx);
	q3 += (qa * gz + qb * gy - qc * gx);

	recipNorm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}
	
void ahrs::toEuler(float *thetax, float *thetay, float *thetaz)
{
	*thetax = atan2(2.0f * (q0*q3 - q1*q2), (q0*q0 + q1*q1 - q2*q2 - q3*q3));
	*thetay = asin(2.0f *(q1*q3 + q0*q2));
	*thetaz = atan2(2.0f * (q0*q1 - q2*q3), (q0*q0 - q1*q1 - q2*q2 + q3*q3));
}
float ahrs::invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

