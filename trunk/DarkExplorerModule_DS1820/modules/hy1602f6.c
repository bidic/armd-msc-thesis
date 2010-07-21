/*
 * hy1602f6.c
 *
 *  Created on: Jul 21, 2010
 *      Author: lhanusiak
 */

#include "hy1602f6.h"

void HY1602F6_SendNibble(char nib) {
	if (nib & 0x01) {
		HY1602F6_D4(1);
	} else {
		HY1602F6_D4(0);
	}

	if (nib & 0x02) {
		HY1602F6_D5(1);
	} else {
		HY1602F6_D5(0);
	}

	if (nib & 0x04) {
		HY1602F6_D6(1);
	} else {
		HY1602F6_D6(0);
	}

	if (nib & 0x08) {
		HY1602F6_D7(1);
	} else {
		HY1602F6_D7(0);
	}

	HY1602F6_EN(1);
	delay(SHORT_DELAY);
	HY1602F6_EN(0);
	delay(SHORT_DELAY);
}

void HY1602F6_SendByte(char ch, int xt) {
	HY1602F6_SendNibble(ch >> 4);
	delay(xt);
	HY1602F6_SendNibble(ch);
	delay(xt);
}

void HY1602F6_WriteData(char ch) {
	HY1602F6_RS(1);
	HY1602F6_SendByte(ch, SHORT_DELAY);
}

void HY1602F6_WriteCmd(unsigned char cmd, int xt) {
	HY1602F6_RS(0);
	HY1602F6_SendByte(cmd, xt);
}

void HY1602F6_SetCursor(unsigned char pos) {
	HY1602F6_WriteCmd(0x80 + pos, LONG_DELAY);
}

void HY1602F6_CursorHome(void) {
	HY1602F6_WriteCmd(0x02, LONG_DELAY);
}

void HY1602F6_SetPosition(unsigned char x, unsigned char y) {
	switch (y) {
	case 1:
		y = 0x40;
		break;
	case 2:
		y = 0x14;
		break;
	}

	HY1602F6_WriteCmd(0x80 + y + x, LONG_DELAY);
}

/*void LcdCursorLeft(void)
 {
 LcdWriteCmd(0x10, LONG_DELAY);
 }

 void LcdCursorRight(void)
 {
 LcdWriteCmd(0x14, SHORT_DELAY);
 }
 */
void HY1602F6_Clear(void) {
	HY1602F6_WriteCmd(0x01, LONG_DELAY);
}

/*void LcdCursorMode(uchar on)
 {
 LcdWriteCmd(on ? 0x0F : 0x0C, LONG_DELAY);
 }
 */
void HY1602F6_Print(const char *s) {
	while (*s)
		HY1602F6_WriteData(*s++);
}

void HY1602F6_PrintChar(char ch) {
	HY1602F6_WriteData(ch);
}

void HY1602F6_Configure() {
	Pin pins[] = { DEFAULT_LCD_PINS };
	PIO_Configure(pins, 1);
	delay(100000);

	HY1602F6_EN(0);
	delay(100000);

	/*
	 * Send a dummy data byte.
	 */
	HY1602F6_WriteData(0);

	/*
	 * Initialize for 4-bit operation.
	 */
	HY1602F6_RS(0);
	HY1602F6_SendNibble(0x33);
	delay(LONG_DELAY);
	HY1602F6_SendNibble(0x33);
	delay(LONG_DELAY);
	HY1602F6_SendNibble(0x33);
	delay(LONG_DELAY);
	HY1602F6_SendNibble(0x22);
	delay(LONG_DELAY);
	HY1602F6_WriteData(0);
	delay(LONG_DELAY);

	/*
	 * Clear display.
	 */
	HY1602F6_WriteCmd(0x07, 2 * LONG_DELAY);

	/*
	 * Disable display shift and set incremental mode.
	 */
	HY1602F6_WriteCmd(0x06, 2 * LONG_DELAY);

	/*
	 * Enable the display and a blinking cursor.
	 */
	HY1602F6_WriteCmd(0x0C, 2 * LONG_DELAY);
	HY1602F6_WriteCmd(0x01, LONG_DELAY);
	HY1602F6_WriteCmd(0x02, LONG_DELAY);
}
