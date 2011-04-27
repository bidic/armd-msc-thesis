/*
 * cd4053b.h
 *
 *  Created on: Apr 7, 2011
 *      Author: lhanusiak
 */

#ifndef CD4053B_H_
#define CD4053B_H_

#define CD4053B_X_OUTPUTS 0
#define CD4053B_Y_OUTPUTS 1

void CD4053_Initialize();

void CD4053_EnableOutput(char ouput_id);

void CD4053_EnableAccelerometer();

void CD4053_EnableIRSensors();

#endif /* CD4053B_H_ */
