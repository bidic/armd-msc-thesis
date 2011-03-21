#include "l3g4200d.h"

static double L3G4200D_offset[3] = { -0.75, 168, 4.1 };
static double L3G4200D_angle[3] = { 0 };
static char L3G4200D_temp = 0;
static char L3G4200D_status = 0;

gyro_data L3G4200D_GetData() {
	gyro_data gyro;

	gyro.sAngle_x = L3G4200D_angle[0];
	gyro.sAngle_y = L3G4200D_angle[1];
	gyro.sAngle_z = L3G4200D_angle[2];
	gyro.sTemperature = L3G4200D_temp;
	gyro.status = L3G4200D_status;

	return gyro;
}

void L3G4200D_ReadData(Twid *twid) {
	unsigned char szBuff[8] = { 0 }; //0 - temp, 1 - status, 2-3 - x, 4-5 - y, 6-7 - z
	unsigned char fifo_src_reg = 0;
	short iter;
	short val[3] = { 0 };

	//	TRACE_INFO("------- fifo fsrc reg: %d \r\n", fifo_src_reg);
	do {

		TWID_Read(twid, L3G4200D_ADDRESS, L3G4200D_FIFO_SRC_REG, 1,
				&fifo_src_reg, 1, 0);

		if (!(fifo_src_reg & 0x20)) {
			TWID_Read(twid, L3G4200D_ADDRESS, L3G4200D_READ_REG, 1, szBuff, 8,
					0);
			L3G4200D_temp = 59 - szBuff[0];
			L3G4200D_status = szBuff[1];
			for (iter = 0; iter < 3; iter++) {
				val[iter]
						= ((short) BIT8TO16(szBuff[3 + iter*2], szBuff[2 + iter*2]));
				L3G4200D_angle[iter] += ((4.03846 * (((double) val[iter])
						+ L3G4200D_offset[iter])) / (100000.0));
			}
			//			TRACE_INFO("test");
		}

		//		if(fifo_src_reg & 0x40)
		//			TRACE_INFO("fifo over    5   run \r\n ");

	} while (!(fifo_src_reg & 0x20));

}

void L3G4200D_Calibrate(Twid *twid) {
	unsigned char szBuff[8] = { 0 }; //0 - temp, 1 - status, 2-3 - x, 4-5 - y, 6-7 - z
	short iter = 0;
	volatile long n = 0;
	unsigned char fifo_src_reg = 0;
	double sum[3] = { 0, 0, 0 };

	long max_count = 1000;

	for (n = 0; n < max_count;) {
		do
		{
			TWID_Read(twid, L3G4200D_ADDRESS, L3G4200D_FIFO_SRC_REG, 1,
					&fifo_src_reg, 1, 0);

			if (!(fifo_src_reg & 0x20)) {
				TWID_Read(twid, L3G4200D_ADDRESS, L3G4200D_READ_REG, 1, szBuff, 8, 0);
				n++;
				for (iter = 0; iter < 3; iter++)
					sum[iter]
							+= ((double) ((short) BIT8TO16(szBuff[3 + iter*2], szBuff[2 + iter*2])))
									/ ((double) max_count);
			}
		} while (!(fifo_src_reg & 0x20));

	}

	L3G4200D_offset[0] = -((double) sum[0]);
	L3G4200D_offset[1] = -((double) sum[1]);
	L3G4200D_offset[2] = -((double) sum[2]);

	TRACE_DEBUG("offset[0]: %d offset[1]: %d offset[2]: %d \r\n", (int) (L3G4200D_offset[0]*100.0), (int)(L3G4200D_offset[1]*100.0), (int)(100.0*L3G4200D_offset[2]));

}

void L3G4200D_Reset(Twid *twid) {
	L3G4200D_angle[0] = 0;
	L3G4200D_angle[1] = 0;
	L3G4200D_angle[2] = 0;
	L3G4200D_temp = 0;

	unsigned char cmd = 0xc0;
	TWID_Write(twid, L3G4200D_ADDRESS, L3G4200D_FIFO_CTRL_REG, 1, &cmd, 1, 0);
}

void L3G4200D_PowerOn(Twid *twid) {
	unsigned char cmd = L3G4200D_CMD_CTRL_REG1_POWER_ON;
	unsigned char cmd2 = 0x6;
	TWID_Write(twid, L3G4200D_ADDRESS, L3G4200D_CTRL_REG1, 1, &cmd, 1, 0);
	//	TWID_Write(twid, L3G4200D_ADDRESS, L3G4200D_CTRL_REG4, 1, &cmd2, 1, 0);
}

void L3G4200D_StreamMode(Twid *twid) {
	unsigned char cmd = L3G4200D_CMD_CTRL_REG5_FIFO_EN;
	TWID_Write(twid, L3G4200D_ADDRESS, L3G4200D_CTRL_REG5, 1, &cmd, 1, 0);
	cmd = L3G4200D_CMD_FIFO_CTRL_REG_STREAM_MODE;
	TWID_Write(twid, L3G4200D_ADDRESS, L3G4200D_FIFO_CTRL_REG, 1, &cmd, 1, 0);
}
