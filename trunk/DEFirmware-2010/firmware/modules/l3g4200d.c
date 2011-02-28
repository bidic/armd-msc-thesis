#include "l3g4200d.h"

static double L3G4200D_offset[3] = {6,130,3};
static double L3G4200D_angle[3] = {0};
static char   L3G4200D_temp = 0;

gyro_data L3G4200D_GetData()
{
  gyro_data gyro;

  gyro.sAngle_x = L3G4200D_angle[0];
  gyro.sAngle_y = L3G4200D_angle[1];
  gyro.sAngle_z = L3G4200D_angle[2];
  gyro.sTemperature = L3G4200D_temp;

  return gyro;
}

void L3G4200D_ReadData(Twid *twid)
{
  unsigned char szBuff[8] = {0}; //0 - temp, 1 - status, 2-3 - x, 4-5 - y, 6-7 - z
  short iter;
  short val[3] = {0};

  TWID_Read(twid, L3G4200D_ADDRESS, L3G4200D_READ_REG, 1, szBuff, 8, 0);

  L3G4200D_temp = szBuff[0];
  for (iter =0; iter<3;iter++)
  {
    val[iter] = BIT8TO16(szBuff[3 + iter*2], szBuff[2 + iter*2]) + L3G4200D_offset[iter];
    L3G4200D_angle[iter]+= ((double)val[iter])/3900.;
  }
}

void L3G4200D_Calibrate(Twid *twid)
{
  unsigned char szBuff[8] = {0}; //0 - temp, 1 - status, 2-3 - x, 4-5 - y, 6-7 - z
  short iter = 0;
  long n = 0;
  long avg[3] = {0};

  for(n = 0; n < 100000; n++)
  {
    TWID_Read(twid, L3G4200D_ADDRESS, L3G4200D_READ_REG, 1, szBuff, 8, 0);

    for (iter = 0; iter<3; iter++)
      avg[iter] += szBuff[3 + iter*2]<<8 | szBuff[2 + iter*2];
  }

  L3G4200D_offset[0] = avg[0]/10000.;
  L3G4200D_offset[1] = avg[1]/10000.;
  L3G4200D_offset[2] = avg[2]/10000.;
}

void L3G4200D_Reset()
{
  L3G4200D_angle[0] = 0;
  L3G4200D_angle[1] = 0;
  L3G4200D_angle[2] = 0;
  L3G4200D_temp = 0;
}

void L3G4200D_PowerOn(Twid *twid)
{
  unsigned char cmd = L3G4200D_CMD_CTRL_REG1_POWER_ON;
  TWID_Write(twid,L3G4200D_ADDRESS,L3G4200D_CTRL_REG1,1,&cmd,1,0);
}

void L3G4200D_StreamMode(Twid *twid)
{
  unsigned char cmd = L3G4200D_CMD_CTRL_REG5_FIFO_EN;
  TWID_Write(twid,L3G4200D_ADDRESS,L3G4200D_CTRL_REG5,1,&cmd,1,0);
  cmd = L3G4200D_CMD_FIFO_CTRL_REG_STREAM_MODE;
  TWID_Write(twid,L3G4200D_ADDRESS,L3G4200D_FIFO_CTRL_REG,1,&cmd,1,0);
}
