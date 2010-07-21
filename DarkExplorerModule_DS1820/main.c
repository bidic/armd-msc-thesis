//////////////////////////////////////////////////////////
// Filename: main.c
// Author:   Lukasz Hanusiak
// Date:     Monday, July 19 2010 03:50 PM
//
// Description: This file contains the implementation of
//              example application of dallas ds1820
//              module.
//
//////////////////////////////////////////////////////////

//#include "modules/dallas_ds1820.h"
#include <de_board.h>
#include <de_pio.h>
#include "modules/hy1602f6.h"
#include <pmc/pmc.h>

#define PIN_LED13  {PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED14 {PIO_PA14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

unsigned char l, remain, temp_txt[] = "  zxx.xxxx\xdf\x43";
unsigned short int temp;
float t;
unsigned int diode_state = 0;

int main(void) {
	//-------------------------------- MAIN START ----------------------------------

	// Enable the clock of the PIOA
	PMC_EnablePeripheral(AT91C_ID_PIOA);

	Pin pins[] = { PIN_LED13, PIN_LED14 };
	//	PIO_LISTSIZE(pins)
	PIO_Configure(pins, 2);

	// LCD Init & Clear
	HY1602F6_Configure();
	HY1602F6_Clear();

	while (1) {
		HY1602F6_Print("DarkExplorer 2.0");
		delay(1000000);

		//diode_state = setDiodeState(!diode_state);
		diode_state = !diode_state;
		if (diode_state) {
			PIO_Clear(pins);
		} else {
			PIO_Set(pins);
		}

		HY1602F6_Clear();

		//LcdPrint("DarkExplorer 2.0");
		//        if (!wire_reset())
		//        {
		//
		//            LcdSetCursor(0x40);
		//            LcdPrint(" No DS1820 device");
		//            delay(1000000);
		//            LcdSetCursor(0x40);
		//            LcdPrint("                ");
		//        }
		//        else
		//        {
		//            LcdSetCursor(0x40);
		//            LcdPrint(" Dallas DS1820  ");
		//            delay(1000000);
		//            LcdSetCursor(0x40);
		//            LcdPrint("                ");
		//            while (1)
		//            {
		//                wire_reset();
		//                wire_send(0xCC); //Master issues Skip ROM command.
		//                wire_send(0x44); //Master issues Convert T command.
		//                delay(750000); //750ms
		//                if (wire_reset())
		//                {
		//                    wire_send(0xCC); //Master issues Skip ROM command.
		//                    wire_send(0xBE); //Master issues Read Scratchpad command.
		//
		//                    /* Read Scratchpad command response :
		//                    byte 0 Temperature LSB (AAh)
		//                    byte 1 Temperature MSB (00h)
		//                    byte 2 TH Register or User Byte 1* TH Register or User Byte 1
		//                    byte 3 TL Register or User Byte 2* TL Register or User Byte 2
		//                    byte 4 Reserved (FFh)
		//                    byte 5 Reserved (FFh)
		//                    byte 6 COUNT REMAIN (0Ch)
		//                    byte 7 COUNT PER °C (10h)
		//                    byte 8 CRC*
		//                    */
		//
		//                    l = 16; //read temperature
		//                    temp = 0x0;
		//                    do
		//                    {
		//                        temp |= wire_rx()<<(16-l);
		//                    }
		//                    while (--l);
		//
		//                    l = 4*8; //skip some data
		//                    do
		//                    {
		//                        wire_rx();
		//                    }
		//                    while (--l);
		//
		//                    l = 8; //read count remain
		//                    remain = 0x0;
		//                    do
		//                    {
		//                        remain |= wire_rx()<<(8-l);
		//                    }
		//                    while (--l);
		//
		//                    if (temp&0x8000) //if temp < 0
		//                    {
		//                        temp = ~temp+1;
		//                        temp_txt[2]='-';
		//                    }
		//                    else // temp > 0
		//                        temp_txt[2]='+';
		//
		//
		//                    //High Resolution Temperature Measurement
		//                    t=(temp>>1) - 0.25 + (16-remain)/(float)16;
		//
		//                    //convert temperature to string
		//                    l = t/10;
		//                    temp_txt[3]='0'+l;
		//                    l = (int)t%10;
		//                    temp_txt[4]='0'+l;
		//                    l = (int)(t*10)%10;
		//                    temp_txt[6]='0'+l;
		//                    l = (int)(t*100)%10;
		//                    temp_txt[7]='0'+l;
		//                    l = (int)(t*1000)%10;
		//                    temp_txt[8]='0'+l;
		//                    l = (int)(t*10000)%10;
		//                    temp_txt[9]='0'+l;
		//
		//                    LcdSetCursor(0x41);
		//                    LcdPrint(temp_txt);
		//                    diode_state = setDiodeState(!diode_state);
		//                }
		//                else
		//                {
		//                    LcdSetCursor(0x40);
		//                    LcdPrint(" No DS1820 device");
		//                    delay(1000000);
		//                    LcdSetCursor(0x40);
		//                    LcdPrint("                ");
		//                }
		//            }
		//        }
	}
	return 0;
}

