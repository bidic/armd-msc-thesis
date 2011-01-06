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
#include "modules/hy1602f6.h"
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

#define BOARD_ADC_FREQ 6000000
#define ADC_VREF       3300  // 3.3 * 1000
#define PIN_LED13  {PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED14  {PIO_PA14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

/// Indicates that the conversion is finished.
static volatile unsigned char conversionDone;

unsigned int diode_state = 0;
unsigned char numbers[10] =
		{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

//-----------------------------------------------------------------------------
/// Convert a digital value in milivolt
/// /param valueToconvert Value to convert in milivolt
//-----------------------------------------------------------------------------
static unsigned int ConvHex2mV(unsigned int valueToConvert) {
	return ((ADC_VREF * valueToConvert) / 0x3FF);
}

//------------------------------------------------------------------------------
/// Interrupt handler for the ADC. Signals that the conversion is finished by
/// setting a flag variable.
//------------------------------------------------------------------------------
static void ISR_Adc(void) {
	unsigned int status;
	unsigned int id_channel;

	if (ADC_IsChannelInterruptStatusSet(ADC_GetStatus(AT91C_BASE_ADC), ADC_CHANNEL_4)) {
		ADC_DisableIt(AT91C_BASE_ADC, ADC_CHANNEL_4);
		conversionDone = 1;
	}
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

	// Enable ADC
	Pin adcPin[] = { PINS_ADC };
	PIO_Configure(adcPin, 1);

	printf("-- ADC_Initialize --\n\r");
	ADC_Initialize(AT91C_BASE_ADC, AT91C_ID_ADC, AT91C_ADC_TRGEN_DIS, 0,
			AT91C_ADC_SLEEP_NORMAL_MODE, AT91C_ADC_LOWRES_10_BIT, BOARD_MCK,
			BOARD_ADC_FREQ, 10, 1200);

	printf("-- ADC_EnableChannel --\n\r");
	ADC_EnableChannel(AT91C_BASE_ADC, ADC_CHANNEL_4);

	AIC_ConfigureIT(AT91C_ID_ADC, 0, ISR_Adc);
	AIC_EnableIT(AT91C_ID_ADC);

	// LCD Init & Clear
	HY1602F6_Configure();
	HY1602F6_Clear();

	while (1) {
		printf("-- ADC_EnableIt --\n\r");
		ADC_EnableIt(AT91C_BASE_ADC, ADC_CHANNEL_4);

		// Start measurement
		printf("-- ADC_StartConversion --\n\r");
		ADC_StartConversion(AT91C_BASE_ADC);

		conversionDone = 0;
		while (conversionDone) {

		}

		unsigned int output = ConvHex2mV(ADC_GetConvertedData(AT91C_BASE_ADC,
				ADC_CHANNEL_4));
		printf("\n-- Channel %d : %d mV --\n\r", ADC_CHANNEL_4, output);

		char text[7] = "     V\0";
		int val = output / 10;
		text[4] = numbers[val % 10];
		val = val / 10;
		text[3] = numbers[val % 10];
		text[2] = '.';
		val = val / 10;
		text[1] = numbers[val % 10];
		val = val / 10;
		text[0] = numbers[val % 10];

		HY1602F6_Print(text);

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

