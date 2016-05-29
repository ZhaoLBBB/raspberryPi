#include "ahrs.h"
#include <math.h>
ahrs::ahrs(float sampletime)
{
	q0 = 1.0f; q1 = 0.0f; q2 = 0.0f; q3 = 0.0f;
	sampleTime = sampletime;
	integralFBX = 0.0f; integralFBY = 0.0f; integralFBZ = 0.0f;
	twoKp = 2.0f; twoKi = 0.01f;
	beta = 1.1f;	
}

void ahrs::MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az)
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

void ahrs::MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
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
void ahrs::MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;


	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * ((-q1 * gx) - (q2 * gy) - (q3 * gz));
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)

		// Normalise accelerometer measurement
	recipNorm = invSqrt(ax * ax + ay * ay + az * az);
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;

		// Normalise magnetometer measurement
	recipNorm = invSqrt(mx * mx + my * my + mz * mz);
	mx *= recipNorm;
	my *= recipNorm;
	mz *= recipNorm;

	// Auxiliary variables to avoid repeated arithmetic
	_2q0mx = 2.0f * q0 * mx;
	_2q0my = 2.0f * q0 * my;
	_2q0mz = 2.0f * q0 * mz;
	_2q1mx = 2.0f * q1 * mx;
	_2q0 = 2.0f * q0;
	_2q1 = 2.0f * q1;
	_2q2 = 2.0f * q2;
	_2q3 = 2.0f * q3;
	_2q0q2 = 2.0f * q0 * q2;
	_2q2q3 = 2.0f * q2 * q3;
	q0q0 = q0 * q0;
	q0q1 = q0 * q1;
	q0q2 = q0 * q2;
	q0q3 = q0 * q3;
	q1q1 = q1 * q1;
	q1q2 = q1 * q2;
	q1q3 = q1 * q3;
	q2q2 = q2 * q2;
	q2q3 = q2 * q3;
	q3q3 = q3 * q3;

	// Reference direction of Earth's magnetic field
	hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
	hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
	_2bx = sqrt(hx * hx + hy * hy);
	_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
	_4bx = 2.0f * _2bx;
	_4bz = 2.0f * _2bz;

	// Gradient decent algorithm corrective step
	s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
	s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
	s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
	s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
	recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
	s0 *= recipNorm;
	s1 *= recipNorm;
	s2 *= recipNorm;
	s3 *= recipNorm;

	// Apply feedback step
	qDot1 -= beta * s0;
	qDot2 -= beta * s1;
	qDot3 -= beta * s2;
	qDot4 -= beta * s3;


	q0 += qDot1 * ( sampleTime);
	q1 += qDot2 * ( sampleTime);
	q2 += qDot3 * ( sampleTime);
	q3 += qDot4 * ( sampleTime);

	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

// IMU algorithm update

void ahrs::MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	qDot1 = 0.5f * ((-q1 * gx) - (q2 * gy) - (q3 * gz));
	qDot2 = 0.5f * ((q0 * gx) + (q2 * gz) - (q3 * gy));
	qDot3 = 0.5f * ((q0 * gy) - (q1 * gz) + (q3 * gx));
	qDot4 = 0.5f * ((q0 * gz) + (q1 * gy) - (q2 * gx));


	recipNorm = invSqrt((ax * ax) + (ay * ay) + (az * az));
	ax *= recipNorm;
	ay *= recipNorm;
	az *= recipNorm;

	_2q0 = 2.0f * q0;
	_2q1 = 2.0f * q1;
	_2q2 = 2.0f * q2;
	_2q3 = 2.0f * q3;
	_4q0 = 4.0f * q0;
	_4q1 = 4.0f * q1;
	_4q2 = 4.0f * q2;
	_8q1 = 8.0f * q1;
	_8q2 = 8.0f * q2;
	q0q0 = q0 * q0;
	q1q1 = q1 * q1;
	q2q2 = q2 * q2;
	q3q3 = q3 * q3;

	s0 = (_4q0 * q2q2) + (_2q2 * ax) + (_4q0 * q1q1) - (_2q1 * ay);
	s1 = (_4q1 * q3q3) - (_2q3 * ax) + (4.0f * q0q0 * q1) - (_2q0 * ay) - _4q1 + (_8q1 * q1q1) + (_8q1 * q2q2) + (_4q1 * az);
	s2 = (4.0f * q0q0 * q2) + (_2q0 * ax) + (_4q2 * q3q3) - (_2q3 * ay) - _4q2 + (_8q2 * q1q1) + (_8q2 * q2q2) + (_4q2 * az);
	s3 = (4.0f * q1q1 * q3) - (_2q1 * ax) + (4.0f * q2q2 * q3) - (_2q2 * ay);
	recipNorm = invSqrt((s0 * s0) + (s1 * s1) + (s2 * s2) + (s3 * s3)); // normalise step magnitude
	s0 *= recipNorm;
	s1 *= recipNorm;
	s2 *= recipNorm;
	s3 *= recipNorm;

	qDot1 -= (beta * s0);
	qDot2 -= (beta * s1);
	qDot3 -= (beta * s2);
	qDot4 -= (beta * s3);

	q0 += qDot1 * ( sampleTime);
	q1 += qDot2 * ( sampleTime);
	q2 += qDot3 * ( sampleTime);
	q3 += qDot4 * ( sampleTime);

	// Normalise quaternion
	recipNorm = invSqrt((q0 * q0) + (q1 * q1) + (q2 * q2) + (q3 * q3));
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}
