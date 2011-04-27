#include "mmc212xm.h"

double mag_offset[2] = {2083,2025.5};
double mag_sensitivity[2]= {91,88};

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
  while(buff[0] & MMC2120M_CMD_TM); //if TM bit isn't set to 0 then measurement wasn't done

  rv.x = buff[1] << 8 | buff[2];
  rv.y = buff[3] << 8 | buff[4];

//  if(sens[0] != 0)
//  {
	  double d = (rv.x - mag_offset[0]);
	  rv.x = d;//1000.0 * d / mag_sensitivity[0];
	  d = (rv.y - mag_offset[1]);
	  rv.y = d;//1000.0 *  d / mag_sensitivity[1];
//  }

  return rv;
}

void MMC212xM_Calibrate(Twid *twid)
{
//	mag_info mi;
//	long i = 0;
//	int maxv[2] = {-65000, -65000};
//	int minv[2] = {65000, 65000};
//	for(i=0;i<2;i++)
//	{
//		sens[i] = 0;
//		offs[i] = 0;
//	}
//
//	for(i = 0; i< 1000; i++)
//	{
//		mi =  MMC212xM_GetMagneticFieldInfo(twid);
//		if ( maxv[0] < mi.x )
//		   maxv[0] = mi.x;
//		if ( minv[0] > mi.x )
//		   minv[0] = mi.x;
//		if ( maxv[1] < mi.y )
//		   maxv[1] = mi.y;
//		if ( minv[1] > mi.y )
//		   minv[1] = mi.y;
//
//	}
//
//	for(i = 0; i< 2; i++)
//	{
//		sens[i] = (maxv[i] - minv[i]) / 2;
//		offs[i] = (maxv[i] + minv[i]) / 2;
//	}
}

void  MMC212xM_SendSetCmd(Twid *twid)
{
  unsigned char cmd = MMC2120M_CMD_SET;
  TWID_Write(twid, MMC2120M_ADDRESS, 0, 1, &cmd, sizeof(cmd), 0);
}

void  MMC212xM_SendResetCmd(Twid *twid)
{
  unsigned char cmd = MMC2120M_CMD_RESET;
  TWID_Write(twid, MMC2120M_ADDRESS, 0, 1, &cmd, sizeof(cmd), 0);
}
