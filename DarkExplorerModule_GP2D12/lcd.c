#include "delay.h"
#define SHORT_DELAY 300   // us przy 8MHz
#define LONG_DELAY 3000  // ms przy 8MHz

#define	 IN	0
#define	 OUT	1

// Wyswietlacz podpiety odpowiednio do linii:
// D4-A0
// D5-A1
// D6-A2
// D7-A3
// RS-A4
// EN-A5
#define LCDD4(arg)  if(arg==1) *AT91C_PIOA_SODR=0x01; else if(arg==0) *AT91C_PIOA_CODR=0x01
#define LCDD5(arg)  if(arg==1) *AT91C_PIOA_SODR=0x02; else if(arg==0) *AT91C_PIOA_CODR=0x02
#define LCDD6(arg)  if(arg==1) *AT91C_PIOA_SODR=0x04; else if(arg==0) *AT91C_PIOA_CODR=0x04
#define LCDD7(arg)  if(arg==1) *AT91C_PIOA_SODR=0x08; else if(arg==0) *AT91C_PIOA_CODR=0x08
#define LCDRS(arg)  if(arg==1) *AT91C_PIOA_SODR=0x10; else if(arg==0) *AT91C_PIOA_CODR=0x10
#define LCDE(arg)   if(arg==1) *AT91C_PIOA_SODR=0x20; else if(arg==0) *AT91C_PIOA_CODR=0x20
//#define LCDCONT(arg)  if(arg==1) IOPORT0_PD|=0x01; else if(arg==0) IOPORT0_PD&=~0x01
//#define LCDLIGHT(arg) if(arg==1) IOPORT0_PD|=0x01; else if(arg==0) IOPORT0_PD&=~0x01

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
    LCDE(0);
     // ustaw PA0-PA7 jako wyjscia
     AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, 0x000000FF);          
    // wyzeruj wyjscia
    AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, 0x000000FF); 
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
    LcdWriteCmd(0x0C,  2 * LONG_DELAY);		// kursor wylaczony
    
    LcdWriteCmd(0x01, LONG_DELAY);          // lcd clear
    LcdWriteCmd(0x02, LONG_DELAY);          // kursor home
}

//eof

