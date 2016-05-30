#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "MPU6050.h"

MPU6050::MPU6050()
{
  devAddr = MPU6050_DEFAULT_ADDRESS;
}
MPU6050::MPU6050(uint8_t address)
{
  devAddr = address;
}

bool MPU6050::testConnection() {
    return getDeviceID() == 0x34;
}

uint8_t MPU6050::getDeviceID()
{
  I2C::readBits(devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer);
  return buffer[0];
}

void MPU6050::initialize()
{
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);
    setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    setSleepEnabled(false);
	setI2CBypassEnabled(true);
	getGyroBias();
}

uint8_t MPU6050::getFullScaleGyroRange()
{
  I2C::readBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, buffer);
  return buffer[0];
}

void MPU6050::setFullScaleGyroRange(uint8_t range)
{
    I2C::writeBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

void MPU6050::setFullScaleAccelRange(uint8_t range)
{
    I2C::writeBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

uint8_t MPU6050::getFullScaleAccelRange()
{
    I2C::readBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, buffer);
    return buffer[0];
}

void MPU6050::setClockSource(uint8_t source)
{
  I2C::writeBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}
bool MPU6050::getI2CBypassEnabled()
{
  I2C::readBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, buffer);
  return buffer[0];
}
void MPU6050::setI2CBypassEnabled(bool enable)
{
  I2C::writeBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enable);
}
uint8_t MPU6050::getClockSource()
{
  I2C::readBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, buffer);
  return buffer[0];
}
bool MPU6050::getSleepEnabled()
{
  I2C::readBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, buffer);
  return buffer[0];
}
void MPU6050::setSleepEnabled(bool enable)
{
  I2C::writeBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enable);
}

void MPU6050::getGyroscope(int16_t* x, int16_t* y, int16_t* z)
{
  I2C::readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 6, buffer);
  *x = (((int16_t)buffer[0])<<8) | (buffer[1]);
  *y = (((int16_t)buffer[2])<<8) | (buffer[3]);
  *z = (((int16_t)buffer[4])<<8) | (buffer[5]);
}
int16_t MPU6050::getGyroscopeX()
{
  I2C::readBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 2, buffer);
  return (((int16_t)buffer[0]) <<8)|(buffer[1]);
}
int16_t MPU6050::getGyroscopeY()
{
  I2C::readBytes(devAddr, MPU6050_RA_GYRO_YOUT_H, 6, buffer);
  return (((int16_t)buffer[0]) <<8)|(buffer[1]);
}
int16_t MPU6050::getGyroscopeZ()
{
  I2C::readBytes(devAddr, MPU6050_RA_GYRO_ZOUT_H, 6, buffer);
  return (((int16_t)buffer[0]) <<8)|(buffer[1]);
}

void MPU6050::getAcceleration(int16_t* x, int16_t* y, int16_t* z)
{
  I2C::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 6, buffer);
  *x = (((int16_t)buffer[0])<<8) | (buffer[1]);
  *y = (((int16_t)buffer[2])<<8) | (buffer[3]);
  *z = (((int16_t)buffer[4])<<8) | (buffer[5]);
}
int16_t MPU6050::getAccelerationX()
{
  I2C::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 2, buffer);
  return (((int16_t)buffer[0]) <<8)|(buffer[1]);
}
int16_t MPU6050::getAccelerationY()
{
  I2C::readBytes(devAddr, MPU6050_RA_ACCEL_YOUT_H, 2, buffer);
  return (((int16_t)buffer[0]) <<8)|(buffer[1]);
}
int16_t MPU6050::getAccelerationZ()
{
  I2C::readBytes(devAddr, MPU6050_RA_ACCEL_ZOUT_H, 2, buffer);
  return (((int16_t)buffer[0]) <<8)|(buffer[1]);
}

void MPU6050::getGyroBias()
{
	int16_t i;
	int32_t gyrox = 0, gyroy = 0, gyroz = 0;
	for(i = 0; i < 100; i++){		
		getGyroscope(&gyroBiasX, &gyroBiasY, &gyroBiasZ);
		gyrox += gyroBiasX;
		gyroy += gyroBiasY;
		gyroz += gyroBiasZ;
		usleep(1000);
	}	
	gyroBiasX = gyrox / 100;
	gyroBiasY = gyroy / 100;
	gyroBiasZ = gyroz / 100;
	
}
void MPU6050::getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{
  I2C::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
  *ax = (((int16_t)buffer[0])<<8) | (buffer[1]);
  *ay = (((int16_t)buffer[2])<<8) | (buffer[3]);
  *az = (((int16_t)buffer[4])<<8) | (buffer[5]);

  *gx = (((int16_t)buffer[8])<<8) | (buffer[9]);
  *gy = (((int16_t)buffer[10])<<8) | (buffer[11]);
  *gz = (((int16_t)buffer[12])<<8) | (buffer[13]);
}

void MPU6050::getMotion6(float *ax, float *ay, float *az, float *gx, float *gy, float *gz)
{
  int16_t rawx, rawy, rawz;	
  I2C::readBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, buffer);
  rawx = (((int16_t)buffer[0])<<8) | (buffer[1]);
  rawy = (((int16_t)buffer[2])<<8) | (buffer[3]);
  rawz = (((int16_t)buffer[4])<<8) | (buffer[5]);
  *ax = ((float)rawx)/16384.0f * 9.801f;
  *ay = ((float)rawy)/16384.0f * 9.801f;
  *az = ((float)rawz)/16385.0f * 9.801f;
  rawx = (((int16_t)buffer[8])<<8) | (buffer[9]);
  rawy = (((int16_t)buffer[10])<<8) | (buffer[11]);
  rawz = (((int16_t)buffer[12])<<8) | (buffer[13]);
  *gx = (((float)(rawx - gyroBiasX))/131.0f)/57.2958f;
  *gy = (((float)(rawy - gyroBiasY))/131.0f)/57.2958f;
  *gz = (((float)(rawz - gyroBiasZ))/131.0f)/57.2958f;

}

void MPU6050::getGyroBias(float *BiasX, float *BiasY, float *BiasZ)
{
	
  *BiasX = (((float)gyroBiasX)/131.0f);
  *BiasY = (((float)gyroBiasX)/131.0f);
  *BiasZ = (((float)gyroBiasX)/131.0f);
}
