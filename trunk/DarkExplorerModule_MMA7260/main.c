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
#include <rtt/rtt.h>

#include "modules/hy1602f6.h"
#include "modules/freescale_mma7260.h"

#include "algorithms/pedometer.h"

void onOutputReadyCallback(MMA7260_OUTPUT output) {
	printf("%d %d %d\n", output.x_mv, output.y_mv, output.z_mv);
}

void onStepCallback(void) {
	printf("-- Step detected            %d --\n\r", get_step_count());
}

void MMA7260_TestCase() {
	MMA7260_InitializeADC();

	while (1) {
		MMA7260_ReadOutput(ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6,
				onOutputReadyCallback);
	}
}

int main(void) {
	// Enable the clock of the PIOA
	PMC_EnablePeripheral(AT91C_ID_PIOA);

	TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);
	printf("-- DarkExplorerModule_MMA7260 with AT91LIB v. %s --\n\r",
			SOFTPACK_VERSION);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

//	MMA7260_TestCase();

	init_pedometer_config();
	start_steps_detection(onStepCallback);


//	stop_counting_steps();

	return 0;
}
