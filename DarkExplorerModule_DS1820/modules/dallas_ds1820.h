/*
 * dallas_ds1820.h
 *
 *  Created on: Jul 21, 2010
 *      Author: lhanusiak
 */

#ifndef DALLAS_DS1820_H_
#define DALLAS_DS1820_H_

#include <at91sam7s256/AT91SAM7S256.h>
#include <pio/pio.h>
#include <delay.h>

#define DS1820_PA 0x1 << 8
#define DS1820_OUTPUT_PIN	{DS1820_PA, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define DS1820_INPUT_PIN	{DS1820_PA, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}

/*
 * Function: sets ds1820 data pin as output
 */
void DS1820_ConfigureOutput(void);

/*
 * Function: sets ds1820 data pin as input
 */
void DS1820_ConfigureInput(void);

/*
 * Function: ds1820 data bus reset
 * Returns:
 * '0' - there is no device on the bus
 * '1' - there is at least one device on the bus
 */
unsigned char DS1820_Reset(void);

/*
 * Function: transfer 1 bit of data
 * Arguments: bit - data to be sent
 */
void DS1820_TX(unsigned char bit);

/*
 * Function: send 8bit set of data
 * Arguments: data - data to be sent
 */
void DS1820_Send(unsigned char data);

/*
 * Function: receives 1 bit of data
 * Returns: received one bit of data
 */
unsigned char DS1820_RX(void);

#endif /* DALLAS_DS1820_H_ */
