/*
 * pio_helper.c
 *
 *  Created on: Apr 27, 2011
 *      Author: lhanusiak
 */

#include "pio_helper.h"

void Configure_DarkExplorer_Pins()
{
  const Pin de_pins[] = { PINS_TWI, PIN_SPI_SPCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_NPCS1, PIN_DIODE1, PIN_DIODE2, PIN_PWM1,
      PIN_ENGINE1, PIN_ENGINE2, PIN_ENGINE3, PIN_ENGINE4, PIN_SERWO_PWM, PIN_SERWO_POWER, PIN_LED_POWER, PIN_CD4053B_CTRL,
      PIN_CAM_RESET, PINS_CAM_DATA, PIN_CAM_HSYNC, PIN_CAM_VSYNC, PIN_CAM_PCLK, PIN_CAM_MCLK
  };

  PIO_Configure(de_pins, PIO_LISTSIZE(de_pins));
}
