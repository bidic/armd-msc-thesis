//////////////////////////////////////////////////////////
// Filename: main.c
// Author:   Lukasz Hanusiak
// Date:     Monday, July 19 2010 03:50 PM
//
// Description: This file contains the implementation of
//              example application of sharp gp2d12
//              module.
//
//////////////////////////////////////////////////////////

#include <de_board.h>
#include <de_pio.h>
#include <pmc/pmc.h>
#include <at91sam7s256/AT91SAM7S256.h>
#include <pio/pio.h>
#include <delay.h>
#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#include <board.h>
#include <aic/aic.h>
#include <adc/adc.h>

#include "modules/hy1602f6.h"
#include "modules/sharp_gp2d12.h"

#define PIN_LED13  {PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED14  {PIO_PA14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

unsigned int diode_state = 0;

void onMeasureCallback(unsigned int distance) {
	printf("\n-- Distance: %d --\n\r", distance);
	char displayText[10];
	sprintf(displayText, "%dmm", distance);
	HY1602F6_Print(displayText);
}

int main(void) {
	//-------------------------------- MAIN START ----------------------------------

	// Enable the clock of the PIOA
	PMC_EnablePeripheral(AT91C_ID_PIOA);

	TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);
	printf("-- DarkExplorerModule_GP2D12 with AT91LIB v. %s --\n\r",
			SOFTPACK_VERSION);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	Pin pins[] = { PIN_LED13, PIN_LED14 };
	PIO_Configure(pins, 2);

	// LCD Init & Clear
	HY1602F6_Configure();
	HY1602F6_Clear();

	// GP2D12
	GP2D12_InitializeADC();

	while (1) {
		GP2D12_MeasureDistance(ADC_CHANNEL_4, onMeasureCallback);

		delay(1000000);
		HY1602F6_Clear();
		diode_state = !diode_state;
		if (diode_state) {
			PIO_Clear(pins);
			PIO_Clear(pins + 1);
		} else {
			PIO_Set(pins);
			PIO_Set(pins + 1);
		};
	}

	return 0;
}

