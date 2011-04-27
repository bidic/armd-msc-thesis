/*
 * l3g4200d.h
 *
 */

#ifndef L3G4200D_H_
#define L3G4200D_H_

#include <drivers/twi/twid.h>

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#define L3G4200D_ADDRESS                        0x69
#define L3G4200D_CTRL_REG1                      0x20
#define L3G4200D_CTRL_REG4                      0x23
#define L3G4200D_CMD_CTRL_REG1_POWER_ON         0x0F
#define L3G4200D_CTRL_REG5                      0x24
#define L3G4200D_CMD_CTRL_REG5_FIFO_EN          0x40
#define L3G4200D_FIFO_CTRL_REG                  0x2E
#define L3G4200D_CMD_FIFO_CTRL_REG_STREAM_MODE  0x40
#define L3G4200D_READ_REG                       0xA6
#define L3G4200D_FIFO_SRC_REG                   0x2F

#define BIT8TO16(b1,b2) ((b1) << 8 | (b2))

typedef struct _gyro_data
{
  short sAngle_x;
  short sAngle_y;
  short sAngle_z;
  char sTemperature;
  char status;
} gyro_data;

gyro_data L3G4200D_GetData();
void L3G4200D_ReadData(Twid *twid);
void L3G4200D_PowerOn(Twid *twid);
void L3G4200D_StreamMode(Twid *twid);
void L3G4200D_Reset(Twid *twid);
void L3G4200D_Calibrate(Twid *twid);

#endif /* MMC212XM_H_ */
