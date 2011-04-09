/*
 * cd4053b.h
 *
 *  Created on: Apr 7, 2011
 *      Author: lhanusiak
 */

#ifndef CD4053B_H_
#define CD4053B_H_

#include <de_pio.h>
#include <pio/pio.h>

#define CD4053B_X_OUTPUTS 0
#define CD4053B_Y_OUTPUTS 1
#define CD4053B_CTRL  {PIO_PA15, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}

void CD4053_Initialize();

void CD4053_EnableOutput(char ouput_id);

void CD4053_EnableAccelerometer();

void CD4053_EnableIRSensors();

#endif /* CD4053B_H_ */
