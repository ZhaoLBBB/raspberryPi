#ifndef _AKM_H_
#define _AKM8975_H_
#include <stdint.h>

//device address definition
#define AKM8975_ADDR_CAD1L_CAD0L    0x0C
#define AKM8975_ADDR_CAD1L_CAD0H    0x0D
#define AKM8975_ADDR_CAD1H_CAD0L    0x0E
#define AKM8975_ADDR_CAD1H_CAD0H    0x0F
#define AKM8975_DEFAULT_ADDR        AKM8975_ADDR_CAD1L_CAD0L

//Device register description
#define AKM8975_RA_WHO_AM_I         0x00
#define AKM8975_RA_INFO             0x01
#define AKM8975_RA_ST1              0x02
#define AKM8975_RA_HXL              0x03
#define AKM8975_RA_HXH              0x04
#define AKM8975_RA_HYL              0x05
#define AKM8975_RA_HYH              0x06
#define AKM8975_RA_HZL              0x07
#define AKM8975_RA_HZH              0x08
#define AKM8975_RA_ST2              0x09
#define AKM8975_RA_CNTL             0x0A
#define AKM8975_RA_RSV              0x0B
#define AKM8975_RA_ASTC             0x0C
#define AKM8975_RA_TS1              0x0D
#define AKM8975_RA_TS2              0x0E
#define AKM8975_RA_I2CDIS           0x0F
#define AKM8975_RA_ASAX             0x10
#define AKM8975_RA_ASAY             0x11
#define AKM8975_RA_ASAZ             0x12
//Device register bit description

#define AKM8975_ST1_DRDY_BIT        0
#define AKM8975_ST2_HOFL_BIT        3
#define AKM8975_ST2_DERR_BIT        2
#define AKM8975_CNTL_MODE_BIT       3
#define AKM8975_CNTL_MODE_LENGTH    4
#define AKM8975_ASTC_SELF_BIT       6
#define AKM8975_I2CDIS_I2CDIS_BIT   0
//----------------------
#define AKM8975_OP_MODE_SINGLE      0x01
#define AKM8975_OP_MODE_POWERDOWN   0x00
#define AKM8975_OP_MODE_SELFTEST    0x08
#define AKM8975_OP_MODE_FUSEROM     0x0F
class AKM8975{
  public:
    AKM8975();
    AKM8975(uint8_t addr);
    void initialize();
    void reset();
    bool testConnection();
    uint8_t getDeviceID();
    bool getDataReadyStatus();
    void getMagnetic(int16_t *mx, int16_t *my, int16_t *mz);
    void setMode(uint8_t mode);
    uint8_t getMode();
    int16_t getMagneticX();
    int16_t getMagneticY();
    int16_t getMagneticZ();
  private:
    uint8_t devAddr;
    uint8_t buffer[32];
};
#endif
