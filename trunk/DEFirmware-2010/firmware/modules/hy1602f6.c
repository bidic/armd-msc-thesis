/*
 * hy1602f6.c
 *
 *  Created on: Apr 6, 2011
 *      Author: lhanusiak
 */

#include "hy1602f6.h"

extern Twid twid;
unsigned char lcd_buffer = 0;

#define HY1602F6_RS(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 8; 	else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 8); 	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
#define HY1602F6_EN(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 4; 	else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 4); 	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
#define HY1602F6_D4(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 16;  else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 16);  	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
#define HY1602F6_D5(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 32;  else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 32);  	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
#define HY1602F6_D6(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 64;  else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 64);  	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
#define HY1602F6_D7(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 128; else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 128);  	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);

void HY1602F6_SendNibble(char nibble) {
	if (nibble & 0x01) {
		HY1602F6_D4(1);
	} else {
		HY1602F6_D4(0);
	}
	if (nibble & 0x02) {
		HY1602F6_D5(1);
	} else {
		HY1602F6_D5(0);
	}
	if (nibble & 0x04) {
		HY1602F6_D6(1);
	} else {
		HY1602F6_D6(0);
	}
	if (nibble & 0x08) {
		HY1602F6_D7(1);
	} else {
		HY1602F6_D7(0);
	}

	HY1602F6_EN(1);
	//delay(SHORT_DELAY);
	waitms(1);
	HY1602F6_EN(0);
	//delay(SHORT_DELAY);
	waitms(1);
}

void HY1602F6_SendByte(char byte, int delay) {
	HY1602F6_SendNibble(byte >> 4);
	//delay(xt);
	waitms(delay);
	HY1602F6_SendNibble(byte);
	//  delay(xt);
	waitms(delay);
}

void HY1602F6_WriteData(char data) {
	HY1602F6_RS(1);
	HY1602F6_SendByte(data, 1);
}

void HY1602F6_WriteCmd(char cmd, int delay) {
	HY1602F6_RS(0);
	HY1602F6_SendByte(cmd, delay);
}

void HY1602F6_ClearDisplay(void) {
	HY1602F6_WriteCmd(0x01, 10);
}

void HY1602F6_PrintText(const char *text) {
	while (*text) {
		HY1602F6_WriteData(*text++);
	}
}

//void HY1602F6_PrintChar(char character) {
//	HY1602F6_WriteData(character);
//}

void HY1602F6_SetCursorPos(unsigned char pos) {
	HY1602F6_WriteCmd(0x80 + pos, 10);
}

void HY1602F6_SetCursorHome(void) {
	HY1602F6_WriteCmd(0x02, 10);
}

void HY1602F6_SetCursor(unsigned char x, unsigned char y) {
	switch (y) {
	case 1:
		y = 0x40;
		break;
	case 2:
		y = 0x14;
		break;
	}
	HY1602F6_WriteCmd(0x80 + y + x, 10);
}

void HY1602F6_Init(void) {
	//	unsigned int i;
//
//	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TWI;
//	TWI_ConfigureMaster(AT91C_BASE_TWI, TWCK, BOARD_MCK);
//	TWID_Initialize(&twid, AT91C_BASE_TWI);

	// Wait at least 10 ms
	//	for (i = 0; i < 100000; i++)
	//		;
	waitms(10);

	HY1602F6_EN(0);
	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
	TWI_Stop(twid.pTwi);
	//	delay(100000);
	waitms(10);

	HY1602F6_WriteData(0);

	HY1602F6_RS(0);
	HY1602F6_SendNibble(0x33);
	//	delay(LONG_DELAY);
	waitms(10);
	HY1602F6_SendNibble(0x33);
	//	delay(LONG_DELAY);
	waitms(10);
	HY1602F6_SendNibble(0x33);
	//	delay(LONG_DELAY);
	waitms(10);
	HY1602F6_SendNibble(0x22);
	//	delay(LONG_DELAY);
	waitms(10);
	HY1602F6_WriteData(0);
	//	delay(LONG_DELAY);
	waitms(10);

	/* Clear display.
	 */
	HY1602F6_WriteCmd(0x07, 2 * 10);
	HY1602F6_WriteCmd(0x06, 2 * 10); // disable display shift and set incremental mode
	HY1602F6_WriteCmd(0x0C, 2 * 10); // disable a blinking cursor
	HY1602F6_WriteCmd(0x01, 10); // clear lcd display
	HY1602F6_WriteCmd(0x02, 10); // set cursor home
}
