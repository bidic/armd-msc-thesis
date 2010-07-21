/*
 * dallas_ds1820.c
 *
 *  Created on: Jul 21, 2010
 *      Author: lhanusiak
 */

#include "dallas_ds1820.h"

void DS1820_SwitchPinState(unsigned short int bit) {
	Pin ds1820_output_pin[] = { DS1820_OUTPUT_PIN };
	if (bit) {
		PIO_Set(ds1820_output_pin);
	} else {
		PIO_Clear(ds1820_output_pin);
	}
}

void DS1820_ConfigureOutput(void) {
	Pin ds1820_output_pin[] = { DS1820_OUTPUT_PIN };
	PIO_Configure(ds1820_output_pin, 1);
}

void DS1820_ConfigureInput(void) {
	Pin ds1820_input_pin[] = { DS1820_INPUT_PIN };
	PIO_Configure(ds1820_input_pin, 1);
}

unsigned char DS1820_Reset(void) {
	unsigned short int present;

	DS1820_ConfigureOutput();
	DS1820_SwitchPinState(0);
	delay(700);
	DS1820_SwitchPinState(1);
	DS1820_ConfigureInput();
	delay(80);

	Pin ds1820_input_pin[] = { DS1820_INPUT_PIN };
	present = PIO_Get(ds1820_input_pin);

	delay(400);
	DS1820_ConfigureOutput();

	return !present;
}

void DS1820_TX(unsigned char bit) {
	DS1820_ConfigureOutput();
	DS1820_SwitchPinState(0);

	if (bit & 1) {
		delay(10);
		DS1820_SwitchPinState(1);
		delay(80);
	} else {
		delay(80);
		DS1820_SwitchPinState(1);
		delay(10);
	}
}

void DS1820_Send(unsigned char data) {
	unsigned char l;
	l = 8;

	do {
		DS1820_TX(data & 1);
		data >>= 1;
	} while (--l);
}

unsigned char DS1820_RX(void) {
	unsigned short int bit;

	DS1820_ConfigureOutput();
	delay(50);
	DS1820_SwitchPinState(0);
	delay(10);
	DS1820_SwitchPinState(1);
	DS1820_ConfigureInput();
	delay(10);

	Pin ds1820_input_pin[] = { DS1820_INPUT_PIN };
	bit = PIO_Get(ds1820_input_pin);

	if (!bit) {
		delay(100);
	}
	DS1820_ConfigureOutput();
	return (bit) ? 1 : 0;
}
