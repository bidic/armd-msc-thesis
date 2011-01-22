//////////////////////////////////////////////////////////
// Filename: main.c
// Author:   Lukasz Hanusiak
// Date:     Saturday, January 22 2011 04:33 PM
//
// Description: This file contains the implementation of
//              example application of freescale mma7260
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
#include "modules/freescale_mma7260.h"

#define PIN_LED13  {PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

void onStatusReadCallback(struct MMA7260State state) {
	printf("%d %d %d\n", state.x_mv, state.y_mv, state.z_mv);
	//	printf("-- X: %dmV, Y: %dmV, Z: %dmV -- \n\r", state.x_mv, state.y_mv,
	//			state.z_mv);

}

int main(void) {
	//-------------------------------- MAIN START ----------------------------------

	// Enable the clock of the PIOA
	PMC_EnablePeripheral(AT91C_ID_PIOA);

	TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);
	printf("-- DarkExplorerModule_MMA7260 with AT91LIB v. %s --\n\r",
			SOFTPACK_VERSION);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	Pin pins[] = { PIN_LED13 };
	PIO_Configure(pins, 2);
	unsigned int diode_state = 0;

	// LCD Init & Clear
	//	HY1602F6_Configure();
	//	HY1602F6_Clear();

	MMA7260_InitializeADC();

	while (1) {
		MMA7260_ReadState(ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6,
				onStatusReadCallback);

		//		delay(1000000);
		//		HY1602F6_Clear();

		diode_state = !diode_state;
		if (diode_state) {
			PIO_Clear(pins);
		} else {
			PIO_Set(pins);
		};
	}

	return 0;
}
