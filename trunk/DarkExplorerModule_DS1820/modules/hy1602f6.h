/*
 * hy1602f6.h
 *
 *  Created on: Jul 19, 2010
 *      Author: lhanusiak
 */

#ifndef HY1602F6_H_
#define HY1602F6_H_


#include <at91sam7s256/AT91SAM7S256.h>
#include <pio/pio.h>
#include <delay.h>

#define DEFAULT_LCD_PINS   {0xff, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define SHORT_DELAY 300
#define LONG_DELAY 3000
#define	OUT	1
#define	IN	0


// Wyświetlacz podłączony według następującej konfiguracji
// D4-A0; D5-A1; D6-A2; D7-A3; RS-A4; EN-A5
#define HY1602F6_D4(arg)  if(arg==1) *AT91C_PIOA_SODR=0x1 << 0; else if(arg==0) *AT91C_PIOA_CODR=0x1 << 0
#define HY1602F6_D5(arg)  if(arg==1) *AT91C_PIOA_SODR=0x1 << 1; else if(arg==0) *AT91C_PIOA_CODR=0x1 << 1
#define HY1602F6_D6(arg)  if(arg==1) *AT91C_PIOA_SODR=0x1 << 2; else if(arg==0) *AT91C_PIOA_CODR=0x1 << 2
#define HY1602F6_D7(arg)  if(arg==1) *AT91C_PIOA_SODR=0x1 << 3; else if(arg==0) *AT91C_PIOA_CODR=0x1 << 3
#define HY1602F6_RS(arg)  if(arg==1) *AT91C_PIOA_SODR=0x1 << 4; else if(arg==0) *AT91C_PIOA_CODR=0x1 << 4
#define HY1602F6_EN(arg)  if(arg==1) *AT91C_PIOA_SODR=0x1 << 5; else if(arg==0) *AT91C_PIOA_CODR=0x1 << 5
//#define LCDCONT(arg)  if(arg==1) IOPORT0_PD|=0x01; else if(arg==0) IOPORT0_PD&=~0x01
//#define LCDLIGHT(arg) if(arg==1) IOPORT0_PD|=0x01; else if(arg==0) IOPORT0_PD&=~0x01


void HY1602F6_SendNibble(char nib);
void HY1602F6_SendByte(char ch, int xt);
void HY1602F6_WriteData(char ch);
void HY1602F6_WriteCmd(unsigned char cmd, int xt);
void HY1602F6_SetCursor(unsigned char pos);
void HY1602F6_CursorHome(void);
void HY1602F6_SetPosition(unsigned char  x,unsigned char y);
void HY1602F6_Clear(void);
void HY1602F6_Print(const char *s);
void HY1602F6_PrintChar(char ch);
void HY1602F6_Configure();

#endif /* HY1602F6_H_ */
