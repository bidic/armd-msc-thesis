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
#include "modules/dallas_ds1820.h"
#include <pmc/pmc.h>

#define PIN_LED13  {PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

unsigned char l, remain, temp_txt[] = "  zxx.xxxx\xdf\x43";
unsigned short int temp;
float t;
unsigned int diode_state = 0;

int main(void) {
	//-------------------------------- MAIN START ----------------------------------

	// Enable the clock of the PIOA
	PMC_EnablePeripheral(AT91C_ID_PIOA);

	Pin pins[] = { PIN_LED13 };
	PIO_Configure(pins, PIO_LISTSIZE(pins));

	// LCD Init & Clear
	HY1602F6_Configure();
	HY1602F6_Clear();

	while (1) {
		diode_state = !diode_state;
		if (diode_state) {
			PIO_Clear(pins);
		} else {
			PIO_Set(pins);
		}

		//		HY1602F6_Clear();

		HY1602F6_Print("DarkExplorer 2.0");
		if (!DS1820_Reset()) {

			HY1602F6_SetCursor(0x40);
			HY1602F6_Print(" No DS1820 device");
			delay(1000000);
			HY1602F6_SetCursor(0x40);
			HY1602F6_Print("                ");
		} else {
			HY1602F6_SetCursor(0x40);
			HY1602F6_Print(" Dallas DS1820  ");
			delay(1000000);
			HY1602F6_SetCursor(0x40);
			HY1602F6_Print("                ");
			while (1) {
				DS1820_Reset();
				DS1820_Send(0xCC); //Master issues Skip ROM command.
				DS1820_Send(0x44); //Master issues Convert T command.
				delay(750000); //750ms
				if (DS1820_Reset()) {
					DS1820_Send(0xCC); //Master issues Skip ROM command.
					DS1820_Send(0xBE); //Master issues Read Scratchpad command.

					/* Read Scratchpad command response :
					 byte 0 Temperature LSB (AAh)
					 byte 1 Temperature MSB (00h)
					 byte 2 TH Register or User Byte 1* TH Register or User Byte 1
					 byte 3 TL Register or User Byte 2* TL Register or User Byte 2
					 byte 4 Reserved (FFh)
					 byte 5 Reserved (FFh)
					 byte 6 COUNT REMAIN (0Ch)
					 byte 7 COUNT PER °C (10h)
					 byte 8 CRC*
					 */

					l = 16; //read temperature
					temp = 0x0;
					do {
						temp |= DS1820_RX() << (16 - l);
					} while (--l);

					l = 4 * 8; //skip some data
					do {
						DS1820_RX();
					} while (--l);

					l = 8; //read count remain
					remain = 0x0;
					do {
						remain |= DS1820_RX() << (8 - l);
					} while (--l);

					if (temp & 0x8000) //if temp < 0
					{
						temp = ~temp + 1;
						temp_txt[2] = '-';
					} else
						// temp > 0
						temp_txt[2] = '+';

					//High Resolution Temperature Measurement
					t = (temp >> 1) - 0.25 + (16 - remain) / (float) 16;

					//convert temperature to string
					l = t / 10;
					temp_txt[3] = '0' + l;
					l = (int) t % 10;
					temp_txt[4] = '0' + l;
					l = (int) (t * 10) % 10;
					temp_txt[6] = '0' + l;
					l = (int) (t * 100) % 10;
					temp_txt[7] = '0' + l;
					l = (int) (t * 1000) % 10;
					temp_txt[8] = '0' + l;
					l = (int) (t * 10000) % 10;
					temp_txt[9] = '0' + l;

					HY1602F6_SetCursor(0x41);
					HY1602F6_Print(temp_txt);
				} else {
					HY1602F6_SetCursor(0x40);
					HY1602F6_Print(" No DS1820 device");
					delay(1000000);
					HY1602F6_SetCursor(0x40);
					HY1602F6_Print("                ");
				}
			}
		}
	}
	return 0;
}

