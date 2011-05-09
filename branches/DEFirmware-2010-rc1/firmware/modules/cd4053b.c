/*
 * cd4053b.c
 *
 *  Created on: Apr 7, 2011
 *      Author: lhanusiak
 */

#include "cd4053b.h"
#include "peripherals/pio_helper.h"
#include <pio/pio.h>

void CD4053_Initialize()
{
  Pin pin_cd4053b_ctrl = PIN_CD4053B_CTRL;
  //PIO_Configure(&pin_cd4053b_ctrl, 1);
}

void CD4053_EnableOutput(char output)
{
  Pin pin_cd4053b_ctrl = PIN_CD4053B_CTRL;

  if (output)
  {
    PIO_Set(&pin_cd4053b_ctrl);
  }
  else
  {
    PIO_Clear(&pin_cd4053b_ctrl);
  }
}

void CD4053_EnableAccelerometer()
{
  CD4053_EnableOutput(CD4053B_X_OUTPUTS);
}

void CD4053_EnableIRSensors()
{
  CD4053_EnableOutput(CD4053B_Y_OUTPUTS);
}
