/*
 * mmc212xm.h
 *
 */

#ifndef MMC212XM_H_
#define MMC212XM_H_

#include <drivers/twi/twid.h>

#define MMC2120M_ADDRESS    0x30
#define MMC2120M_CMD_TM     0x1
#define MMC2120M_CMD_SET    0x1 << 1
#define MMC2120M_CMD_RESET  0x1 << 2
#define MMC2120M_MAX_VAL    0x1 << 11

#define BIT12TO32(b1,b2) -((MMC2120M_MAX_VAL) - ((b1) << 8 | (b2)))

typedef struct _mag_info
{
  double x;
  double y;
} mag_info;

mag_info MMC212xM_GetMagneticFieldInfo(Twid *twid);

void  MMC212xM_SendSetCmd(Twid *twid);
void  MMC212xM_SendResetCmd(Twid *twid);
void  MMC212xM_Calibrate(Twid *twid);

#endif /* MMC212XM_H_ */
