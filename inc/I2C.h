#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#ifndef TRUE
#define TRUE	(1==1)
#define FALSE	(0==1)
#endif
#include <stdint.h>
class I2C{
  public:
    I2C();
    static int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
    static int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
    static int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t *data);
    static int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);

    static bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
    static bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
    static bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, uint8_t data);
    static bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
};
#endif
