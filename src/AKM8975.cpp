#include "AKM8975.h"
#include "I2C.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

AKM8975::AKM8975()
{
  devAddr = AKM8975_DEFAULT_ADDR;
}

AKM8975::AKM8975(uint8_t addr)
{
  devAddr = addr;
}

uint8_t AKM8975::getDeviceID()
{
  I2C::readByte(devAddr, AKM8975_RA_WHO_AM_I, buffer);
  return buffer[0];
}
void AKM8975::initialize()
{
  setMode(AKM8975_OP_MODE_SINGLE);
}
bool AKM8975::testConnection()
{
  return (getDeviceID()==0x48);
}

bool AKM8975::getDataReadyStatus()
{
  I2C::readByte(devAddr, AKM8975_RA_ST1, buffer);
  return buffer[0];
}

void AKM8975::reset()
{
  I2C::writeByte(devAddr, AKM8975_RA_CNTL, 0x00);
}

void AKM8975::setMode(uint8_t mode)
{
  I2C::writeByte(devAddr, AKM8975_RA_CNTL, mode);
}

uint8_t AKM8975::getMode()
{
  I2C::readByte(devAddr, AKM8975_RA_CNTL, buffer);
  return buffer[0];
}

void AKM8975::getMagnetic(int16_t *mx, int16_t *my, int16_t *mz)
{
  I2C::readBytes(devAddr, AKM8975_RA_HXL, 6, buffer);
  setMode(AKM8975_OP_MODE_SINGLE);
  *mx = ((int16_t)buffer[1]<<8) | buffer[0];
  *my = ((int16_t)buffer[3]<<8) | buffer[2];
  *mz = ((int16_t)buffer[5]<<8) | buffer[4];
}
int16_t AKM8975::getMagneticX()
{
  I2C::readBytes(devAddr, AKM8975_RA_HXL, 2, buffer);
  setMode(AKM8975_OP_MODE_SINGLE);
  return (((int16_t)buffer[1]<<8) | buffer[0]);
}

int16_t AKM8975::getMagneticY()
{
  I2C::readBytes(devAddr, AKM8975_RA_HYL, 2, buffer);
  setMode(AKM8975_OP_MODE_SINGLE);
  return (((int16_t)buffer[1]<<8) | buffer[0]);
}

int16_t AKM8975::getMagneticZ()
{
  I2C::readBytes(devAddr, AKM8975_RA_HZL, 2, buffer);
  setMode(AKM8975_OP_MODE_SINGLE);
  return (((int16_t)buffer[1]<<8) | buffer[0]);
}
