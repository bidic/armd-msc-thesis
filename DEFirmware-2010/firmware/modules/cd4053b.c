/*
 * cd4053b.c
 *
 *  Created on: Apr 7, 2011
 *      Author: lhanusiak
 */

#include "cd4053b.h"
static Pin cd4053b_ctrl_pin[] = { CD4053B_CTRL };

void CD4053_Initialize() {
	PIO_Configure(cd4053b_ctrl_pin, 1);
}

void CD4053_EnableOutput(char output) {
	if (output) {
		PIO_Set(cd4053b_ctrl_pin);
	} else {
		PIO_Clear(cd4053b_ctrl_pin);
	}
}

void CD4053_EnableAccelerometer(){
	CD4053_EnableOutput(CD4053B_X_OUTPUTS);
}

void CD4053_EnableIRSensors(){
	CD4053_EnableOutput(CD4053B_Y_OUTPUTS);
}
