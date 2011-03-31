#include "de_pio.h"

#include <math.h>

#include <pio/pio.h>
#include <aic/aic.h>
#include <pmc/pmc.h>
#include <pit/pit.h>
#include <dbgu/dbgu.h>
#include <twi/twi.h>
#include <utility/math.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <drivers/async/async.h>
#include <drivers/twi/twid.h>

#define SHORT_DELAY 300   // us przy 8MHz
#define LONG_DELAY 3000  // ms przy 8MHz

#define  IN 0
#define  OUT  1

/// TWI clock frequency in Hz.
#define TWCK            100000

/// Slave address of AT24C chips.
#define AT24C_ADDRESS   56

/// Page size of an AT24C1024 chip (in bytes)
#define PAGE_SIZE       2

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Pio pins to configure.
static const Pin pins[] = {PINS_TWI};

/// TWI driver instance.
static Twid twid;

unsigned char lcdBuff = 0;
// Wyswietlacz podpiety odpowiednio do linii:
// D4-A0
// D5-A1
// D6-A2
// D7-A3
// RS-A4
// EN-A5
#define LCDRS(arg)  if(arg==1) lcdBuff=lcdBuff | 32; else if(arg==0) lcdBuff= ~(~lcdBuff | 32); TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);
#define LCDE(arg)   if(arg==1) lcdBuff=lcdBuff | 16; else if(arg==0) lcdBuff= ~(~lcdBuff | 16); TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);
#define LCDD4(arg)  if(arg==1) lcdBuff=lcdBuff | 8;  else if(arg==0) lcdBuff= ~(~lcdBuff | 8);  TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);
#define LCDD5(arg)  if(arg==1) lcdBuff=lcdBuff | 4;  else if(arg==0) lcdBuff= ~(~lcdBuff | 4);  TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);
#define LCDD6(arg)  if(arg==1) lcdBuff=lcdBuff | 2;  else if(arg==0) lcdBuff= ~(~lcdBuff | 2);  TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);
#define LCDD7(arg)  if(arg==1) lcdBuff=lcdBuff | 1;  else if(arg==0) lcdBuff= ~(~lcdBuff | 1);  TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);

//#define LCDCONT(arg)  if(arg==1) IOPORT0_PD|=0x01; else if(arg==0) IOPORT0_PD&=~0x01
//#define LCDLIGHT(arg) if(arg==1) IOPORT0_PD|=0x01; else if(arg==0) IOPORT0_PD&=~0x01


void delay(volatile unsigned int d)
{
 volatile unsigned int i;
 for (i=0; i<d; i++){}
}

void LcdSendNibble(char nib)
{
  if(nib&0x01) {LCDD4(1);} else {LCDD4(0);}
  if(nib&0x02) {LCDD5(1);} else {LCDD5(0);}
  if(nib&0x04) {LCDD6(1);} else {LCDD6(0);}
  if(nib&0x08) {LCDD7(1);} else {LCDD7(0);}

    LCDE(1);
  delay(SHORT_DELAY);
  LCDE(0);
        delay(SHORT_DELAY);
}

void LcdSendByte(char ch, int xt)
{
    LcdSendNibble(ch >> 4);
  delay(xt);
    LcdSendNibble(ch);
  delay(xt);
}

void LcdWriteData(char ch)
{
    LCDRS(1);
    LcdSendByte(ch, SHORT_DELAY);
}

void LcdWriteCmd(unsigned char cmd, int xt)
{
    LCDRS(0);
    LcdSendByte(cmd, xt);
}

void LcdSetCursor(unsigned char pos)
{
    LcdWriteCmd(0x80 + pos, LONG_DELAY);
}

void LcdCursorHome(void)
{
    LcdWriteCmd(0x02, LONG_DELAY);
}

void LCD_xy(unsigned char  x,unsigned char y)
 {
  switch(y)
  {
   case 1: y=0x40; break;
   case 2: y=0x14; break;
  }
  LcdWriteCmd(0x80+y+x,LONG_DELAY);
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
void LcdClear(void)
{
    LcdWriteCmd(0x01, LONG_DELAY);
}

/*void LcdCursorMode(uchar on)
{
    LcdWriteCmd(on ? 0x0F : 0x0C, LONG_DELAY);
}
*/
void LcdPrint(const char *s)
{
  while(*s) LcdWriteData(*s++);
}

void LcdPrintChar(char ch)
{
  LcdWriteData(ch);
}

void LcdInit()
{
  unsigned int i;

  PIO_Configure(pins, PIO_LISTSIZE(pins));

  // Configure TWI
  // In IRQ mode: to avoid problems, the priority of the TWI IRQ must be max.
  // In polling mode: try to disable all IRQs if possible.
  // (in this example it does not matter, there is only the TWI IRQ active)
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TWI;
  TWI_ConfigureMaster(AT91C_BASE_TWI, TWCK, BOARD_MCK);
  TWID_Initialize(&twid, AT91C_BASE_TWI);
//    AIC_ConfigureIT(AT91C_ID_TWI, 0, ISR_Twi);
//    AIC_EnableIT(AT91C_ID_TWI);

  // Wait at least 10 ms

  for (i=0; i < 100000; i++);

//    printf("-I- Filling page #1 with zeroes ...\n\r");
//  int dev = 0;
//  unsigned char pData[PAGE_SIZE];
//  memset(pData, 0, PAGE_SIZE);
//  for(dev=0;dev<255; dev++)
//  {
//
//    TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);
////    TWI_Stop((&twid)->pTwi);
//  // Wait at least 10 ms
//    for (i=0; i < 100000; i++);
//    memset(pData, 0, PAGE_SIZE);
//    TWID_Read(&twid, AT24C_ADDRESS, 0, 0, pData, 1, 0);
//    pData[0]++;
//  }

    LCDE(0);
    TWID_Write(&twid, AT24C_ADDRESS, 0, 0, &lcdBuff, 1, 0);
    TWI_Stop(twid.pTwi);
    delay(100000);

    /*
     * Send a dummy data byte.
     */
    LcdWriteData(0);

    /*
     * Initialize for 4-bit operation.
     */
    LCDRS(0);
    LcdSendNibble(0x33);
    delay(LONG_DELAY);
    LcdSendNibble(0x33);
    delay(LONG_DELAY);
    LcdSendNibble(0x33);
    delay(LONG_DELAY);
    LcdSendNibble(0x22);
    delay(LONG_DELAY);
    LcdWriteData(0);
    delay(LONG_DELAY);

     /* Clear display.
     */
    LcdWriteCmd(0x07, 2 * LONG_DELAY);

    /*
     * Disable display shift and set incremental mode.
     */
    LcdWriteCmd(0x06,  2 * LONG_DELAY);

    /*
     * Enable the display and a blinking cursor.
     */
    LcdWriteCmd(0x0C,  2 * LONG_DELAY);   // kursor wylaczony

    LcdWriteCmd(0x01, LONG_DELAY);          // lcd clear
    LcdWriteCmd(0x02, LONG_DELAY);          // kursor home
}

//eof

