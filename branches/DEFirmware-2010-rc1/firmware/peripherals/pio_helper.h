/*
 * de_pin.h
 *
 *  Created on: Apr 19, 2011
 *      Author: lhanusiak
 */

#ifndef PIO_HELPER_H_
#define PIO_HELPER_H_

#include <at91sam7s256/AT91SAM7S256.h>
#include <board.h>
#include <pio/pio.h>

#define CAM_D7      	AT91C_PIO_PA27
#define CAM_D6      	AT91C_PIO_PA20
#define CAM_D5      	AT91C_PIO_PA19
#define CAM_D4      	AT91C_PIO_PA18
#define CAM_D3      	AT91C_PIO_PA17
#define CAM_D2      	AT91C_PIO_PA16
#define CAM_D1      	AT91C_PIO_PA22
#define CAM_D0      	AT91C_PIO_PA24

#define PIN_CAM_RESET  { AT91C_PIO_PA23, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT }
#define PINS_CAM_DATA { CAM_D0 | CAM_D1 |CAM_D2 |CAM_D3 |CAM_D4 |CAM_D5 |CAM_D6 | CAM_D7,\
                        AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT }
#define PIN_CAM_HSYNC  { AT91C_PIO_PA26 , AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT }
#define PIN_CAM_VSYNC { AT91C_PIO_PA25 , AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT }
#define PIN_CAM_PCLK  { AT91C_PIO_PA30, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT }
#define PIN_CAM_MCLK  { AT91C_PA21_PCK1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT }
#define PIN_DIODE1 { AT91C_PIO_PA28, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT }
#define PIN_DIODE2 { AT91C_PIO_PA29, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT }
#define PIN_PWM1  { AT91C_PA11_PWM0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT }
#define PIN_ENGINE1 { AT91C_PIO_PA7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_PULLUP }
#define PIN_ENGINE2 { AT91C_PIO_PA8, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_PULLUP }
#define PIN_ENGINE3 { AT91C_PIO_PA9, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_PULLUP }
#define PIN_ENGINE4 { AT91C_PIO_PA10, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_PULLUP }
#define PIN_SERWO_PWM { AT91C_PIO_PA0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT }
#define PIN_SERWO_POWER { AT91C_PIO_PA2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT }
#define PIN_LED_POWER { AT91C_PIO_PA1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT }
#define PIN_CD4053B_CTRL  {AT91C_PIO_PA15, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT }

void Configure_DarkExplorer_Pins();

#endif /* PIO_HELPER_H_ */