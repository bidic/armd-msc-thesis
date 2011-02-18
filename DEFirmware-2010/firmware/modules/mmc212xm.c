#include "mmc212xm.h"

mag_info MMC212xM_GetMagneticFieldInfo(Twid *twid)
{
  mag_info rv;
  unsigned char buff[5] = {0};
  unsigned char cmd = MMC2120M_CMD_TM;

  TWID_Write(twid, MMC2120M_ADDRESS, 0, 1, &cmd, sizeof(cmd), 0);

  do
  {
    TWID_Read(twid, MMC2120M_ADDRESS, 0, 1, buff, 5, 0);
  }
  while(buff[0]); //if there is something in internal_reg it means that data wasn't prepared yet

  rv.x = BIT12TO32(buff[1], buff[2]); //MMC2120M send us data in 12bit model, here we converts it to 32 bits
  rv.y = BIT12TO32(buff[3], buff[4]);

  return rv;
}

void  MMC212xM_SendSetCmd(Twid *twid)
{
  char cmd = MMC2120M_CMD_SET;
  TWID_Write(twid, MMC2120M_ADDRESS, 0, 1, &cmd, sizeof(cmd), 0);
}

void  MMC212xM_SendResetCmd(Twid *twid)
{
  char cmd = MMC2120M_CMD_RESET;
  TWID_Write(twid, MMC2120M_ADDRESS, 0, 1, &cmd, sizeof(cmd), 0);
}
