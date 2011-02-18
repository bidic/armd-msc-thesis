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

#include "algorithms/obstacle_avoidance.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define PIN_LED16  {PIO_PA16, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED11  {PIO_PA11, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED12  {PIO_PA12, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED13  {PIO_PA13, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED14  {PIO_PA14, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED15  {PIO_PA15, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

unsigned int diode_state = 0;

void onMeasureCallback(unsigned int output) {
	unsigned int distance = GP2D12_ComputeDistanceFromCharacterisic(output);
	printf("\n-- Distance: %d --\n\r", distance);
	char displayText[10];
	sprintf(displayText, "%dmm", distance);
	HY1602F6_Print(displayText);
}

void SHARP_GP2D12_TestCase() {
	Pin pins[] = { PIN_LED13, PIN_LED14 };
	PIO_Configure(pins, 2);

	// LCD Init & Clear
	HY1602F6_Configure();
	HY1602F6_Clear();

	// GP2D12
	GP2D12_InitializeADC();

	while (1) {
		GP2D12_MeasureDistance(ADC_CHANNEL_5, onMeasureCallback);

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
}

volatile unsigned int data_ready = 0;
volatile unsigned int ls_data = 0;
volatile unsigned int rs_data = 0;

void onLSDataReady(unsigned int output) {
	unsigned int distance = GP2D12_ComputeDistanceFromCharacterisic(output);
	printf("-- LS distance: %d, output: %d --\n\r", distance, output);
	ls_data = output;
	data_ready = 1;
}

void onRSDataReady(unsigned int output) {
	unsigned int distance = GP2D12_ComputeDistanceFromCharacterisic(output);
	printf("\n-- RS distance: %d, output: %d --\n\r", distance, output);
	rs_data = output;
	data_ready = 1;
}

int main(void) {
	// Enable the clock of the PIOA
	PMC_EnablePeripheral(AT91C_ID_PIOA);

	TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);
	printf("-- DarkExplorerModule_GP2D12 with AT91LIB v. %s --\n\r",
			SOFTPACK_VERSION);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	//	SHARP_GP2D12_TestCase();

	Pin pins[] = { PIN_LED16, PIN_LED11, PIN_LED12, PIN_LED13, PIN_LED14,
			PIN_LED15 };
	PIO_Configure(pins, 6);

	// GP2D12
	GP2D12_InitializeADC();

	while (1) {
		data_ready = 0;
		GP2D12_MeasureDistance(ADC_CHANNEL_4, onRSDataReady);
		while (!data_ready) {
		}

		data_ready = 0;
		GP2D12_MeasureDistance(ADC_CHANNEL_5, onLSDataReady);
		while (!data_ready) {
		}

		use_default_configuration();

		unsigned int level_mask = create_level_mask(rs_data, ls_data);

		if (CHECK_BIT(level_mask, 0))
			PIO_Clear(pins);
		else
			PIO_Set(pins);

		if (CHECK_BIT(level_mask, 1))
			PIO_Clear(pins + 1);
		else
			PIO_Set(pins + 1);

		if (CHECK_BIT(level_mask, 2))
			PIO_Clear(pins + 2);
		else
			PIO_Set(pins + 2);

		if (CHECK_BIT(level_mask, 3))
			PIO_Clear(pins + 3);
		else
			PIO_Set(pins + 3);

		if (CHECK_BIT(level_mask, 4))
			PIO_Clear(pins + 4);
		else
			PIO_Set(pins + 4);

		if (CHECK_BIT(level_mask, 5))
			PIO_Clear(pins + 5);
		else
			PIO_Set(pins + 5);

		OAA_OUTPUT output = avoid_obstacles(level_mask);

		printf("\n-- Level mask: %d --\n\r", level_mask);
		printf("-- Speed right: %d --\n\r", output.speed_right);
		printf("-- Speed left: %d --\n\r", output.speed_left);
		printf("-- Gear right: %d --\n\r", output.gear_right);
		printf("-- Gear left: %d --\n\r", output.gear_left);

		delay(1000000);
	}

	return 0;
}

