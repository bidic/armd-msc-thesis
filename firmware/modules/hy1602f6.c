/*
 * hy1602f6.c
 *
 *  Created on: Apr 6, 2011
 *      Author: lhanusiak
 */

#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <twi/twi.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <drivers/async/async.h>
#include <drivers/twi/twid.h>

#include <peripherals.h>

#include "hy1602f6.h"

extern Twid twid;
unsigned char lcd_buffer = 0;

#define HY1602F6_RS(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 8; 	else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 8); 	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
#define HY1602F6_EN(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 4; 	else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 4); 	TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
#define HY1602F6_D4(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 16;  else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 16);
#define HY1602F6_D5(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 32;  else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 32);
#define HY1602F6_D6(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 64;  else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 64);
#define HY1602F6_D7(arg)  if((arg) == 1) lcd_buffer = lcd_buffer | 128; else if((arg) == 0) lcd_buffer= ~(~lcd_buffer | 128);

void HY1602F6_SendNibble(char nibble)
{
  if (nibble & 0x01)
  {
    HY1602F6_D4(1);
  }
  else
  {
    HY1602F6_D4(0);
  }
  if (nibble & 0x02)
  {
    HY1602F6_D5(1);
  }
  else
  {
    HY1602F6_D5(0);
  }
  if (nibble & 0x04)
  {
    HY1602F6_D6(1);
  }
  else
  {
    HY1602F6_D6(0);
  }
  if (nibble & 0x08)
  {
    HY1602F6_D7(1);
  }
  else
  {
    HY1602F6_D7(0);
  }

  HY1602F6_EN(1);
  while(!TWI_TransferComplete(twid.pTwi));
  HY1602F6_EN(0);
  while(!TWI_TransferComplete(twid.pTwi));
}

void HY1602F6_SendByte(char byte, int delay)
{
  HY1602F6_SendNibble(byte >> 4);
  waitms(delay);
  HY1602F6_SendNibble(byte);
  waitms(delay);
}

void HY1602F6_WriteData(char data)
{
  HY1602F6_RS(1);
  HY1602F6_SendByte(data, 1);
}

void HY1602F6_WriteCmd(char cmd, int delay)
{
  HY1602F6_RS(0);
  HY1602F6_SendByte(cmd, delay);
}

void HY1602F6_ClearDisplay(void)
{
  HY1602F6_WriteCmd(0x01, 1);
}

void HY1602F6_PrintText(const char *text)
{
  int counter = 0;
  while (*(text+counter) != '\0' && (counter < 16))
  {
    HY1602F6_WriteData(*(text+counter));
    counter++;
  }
}

void HY1602F6_SetCursorPos(unsigned char pos)
{
  HY1602F6_WriteCmd(0x80 + pos, 1);
}

void HY1602F6_SetCursorHome(void)
{
  HY1602F6_WriteCmd(0x02, 10);
}

void HY1602F6_StartNextLine()
{
  HY1602F6_WriteCmd(0x80 + 0x40, 10);
}

void HY1602F6_Init(void)
{
  TRACE_DEBUG("-- HY1602F6 Display initialization -- \r\n");

  HY1602F6_EN(0);
  TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcd_buffer, 1, 0);
  HY1602F6_WriteData(0);
  HY1602F6_RS(0);
  HY1602F6_SendNibble(0x33);
  HY1602F6_SendNibble(0x33);
  HY1602F6_SendNibble(0x33);
  HY1602F6_SendNibble(0x22);
  HY1602F6_WriteData(0);
  HY1602F6_WriteCmd(0x07, 1);
  HY1602F6_WriteCmd(0x06, 1); // disable display shift and set incremental mode
  HY1602F6_WriteCmd(0x0C, 1); // disable a blinking cursor
  HY1602F6_WriteCmd(0x01, 1); // clear lcd display
  HY1602F6_WriteCmd(0x02, 1); // set cursor home

  TRACE_DEBUG("-- HY1602F6 Init completed -- \r\n");
}

void HY1602F6_Log(const char* line1, const char* line2)
{
  HY1602F6_ClearDisplay();
  HY1602F6_PrintText(line1);
  HY1602F6_StartNextLine();
  HY1602F6_PrintText(line2);
}

void HY1602F6_SetProgress(unsigned char state, unsigned char clear)
{
  int level = (32 * state) / 255;
  unsigned char iter = 0;
  if (clear)
  {
    HY1602F6_ClearDisplay();
  }
  else
  {
    HY1602F6_SetCursorHome();
  }

  while (level-- > 0)
  {
    HY1602F6_PrintText("#");
    if (++iter == 16)
    {
      HY1602F6_StartNextLine();
    }
  }
}
