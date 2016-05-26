#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include "I2C.h"

#define I2CDEV "/dev/i2c-1"
I2C::I2C(){};
int8_t I2C::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
  int8_t count = 0;
  int fd;
  fd = open(I2CDEV, O_RDWR);
  if(fd < 0){
    fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
    return -1;
  }
  if(ioctl(fd, I2C_SLAVE, devAddr) < 0){
    fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
    close(fd);
    return -1;
  }
  if(write(fd, &regAddr, 1) != 1){
    fprintf(stderr, "Failed to write reg: %s\n", strerror(errno));
    close(fd);
    return -1;
  }
  count = read(fd, data, length);
  if(count < 0 ){
    fprintf(stderr, "Failed to read: %s\n", strerror(errno));
    close(fd);
    return -1;
  }else if(count != length){
    fprintf(stderr, "Expect read length:%d\n, accutal read length:%d\n", length, count);
    close(fd);
    return -1;
  }
  close(fd);
  return count;
}

int8_t I2C::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data)
{
  return readBytes(devAddr, regAddr, 1, data);
}

int8_t I2C::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t *data)
{
   uint8_t b;
   int8_t count = readBytes(devAddr, regAddr, 1, &b);
   b &= (1 << bitPos);
   *data = b;
   return count;
}
int8_t I2C::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
  int8_t count;
  uint8_t  b, mask;
  if ((count = readByte(devAddr, regAddr, &b))) {
      mask = ((1 << length) - 1) << (bitStart - length + 1);
      b &= mask;
      b >>= (bitStart - length + 1);
      *data = b;
  }
  return count;
}

bool I2C::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
    uint8_t tmpBuf[128], count;
	int fd;
    if(length > 127){
      fprintf(stderr, "The write length %d > 127\n", length);
      return false;
    }
    fd = open(I2CDEV, O_RDWR);
    if(fd < 0){
      fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
      return false;
    }
    if(ioctl(fd, I2C_SLAVE, devAddr) < 0){
      fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
      close(fd);
      return false;
    }

    tmpBuf[0] = regAddr;
    memcpy(tmpBuf+1, data, length);
    count = write(fd, tmpBuf, length+1);
    if (count < 0) {
        fprintf(stderr, "Failed to write device(%d): %s\n", count, strerror(errno));
        close(fd);
        return false;
    } else if (count != length+1) {
        fprintf(stderr, "Short write to device, expected %d, got %d\n", length+1, count);
        close(fd);
        return false;
    }
    close(fd);
    return true;
}

bool I2C::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    return writeBytes(devAddr, regAddr, 1, &data);
}

bool I2C::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t data)
{
  uint8_t preData;
  readByte(devAddr, regAddr, &preData);
  preData = (data != 0)?(preData | (1 << bitPos)):(preData & ~(1 << bitPos));
  return writeByte(devAddr, regAddr, preData);
}

bool I2C::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
  uint8_t b, mask;
  if (readByte(devAddr, regAddr, &b) != 0) {
      mask = ((1 << length) - 1) << (bitStart - length + 1);
      data <<= (bitStart - length + 1); // shift data into correct position
      data &= mask; // zero all non-important bits in data
      b &= ~(mask); // zero all important bits in existing byte
      b |= data; // combine data with existing byte
      return writeByte(devAddr, regAddr, b);
  } else {
      return false;
  }
}
