/*
 * de_pin.h
 *
 *  Created on: Apr 19, 2011
 *      Author: lhanusiak
 */

#ifndef PIO_HELPER_H_
#define PIO_HELPER_H_

#include <at91sam7s256/AT91SAM7S256.h>
#include <pio/pio.h>

#define PIO_PA0     0x1 << 0
#define PIO_PA1     0x1 << 1
#define PIO_PA2     0x1 << 2
#define PIO_PA3     0x1 << 3
#define PIO_PA4     0x1 << 4
#define PIO_PA5     0x1 << 5
#define PIO_PA6     0x1 << 6
#define PIO_PA7     0x1 << 7
#define PIO_PA8     0x1 << 8
#define PIO_PA9     0x1 << 9
#define PIO_PA10    0x1 << 10
#define PIO_PA11    0x1 << 11
#define PIO_PA12    0x1 << 12
#define PIO_PA13    0x1 << 13
#define PIO_PA14    0x1 << 14
#define PIO_PA15    0x1 << 15
#define PIO_PA16    0x1 << 16
#define PIO_PA17    0x1 << 17
#define PIO_PA18    0x1 << 18
#define PIO_PA19    0x1 << 19
#define PIO_PA20    0x1 << 20
#define PIO_PA21    0x1 << 21
#define PIO_PA22    0x1 << 22
#define PIO_PA23    0x1 << 23
#define PIO_PA24    0x1 << 24
#define PIO_PA25    0x1 << 25
#define PIO_PA26    0x1 << 26
#define PIO_PA27    0x1 << 27
#define PIO_PA28    0x1 << 28
#define PIO_PA29    0x1 << 29
#define PIO_PA30    0x1 << 30
#define PIO_PA31    0x1 << 31

#define SERWO_PWM   	PIO_PA0
#define LED_POWER   	PIO_PA1
#define SERWO_POWER 	PIO_PA2
#define I2C_DATA    	PIO_PA3
#define I2C_CLOCK   	PIO_PA4
#define UART_RX     	PIO_PA5
#define UART_TX     	PIO_PA6
#define IN1         	PIO_PA7
#define IN2         	PIO_PA8
#define IN3         	PIO_PA9
#define IN4         	PIO_PA10
#define PWM1        	PIO_PA11
#define PWM2        	PIO_PA12
#define PWM3        	PIO_PA13
#define PWM4        	PIO_PA14
#define CD4053B_CTRL 	PIO_PA15
#define VSYNC       	PIO_PA25
#define HSYNC       	PIO_PA26
#define MCLK        	PIO_PA21
#define CAM_D7      	PIO_PA27
#define CAM_D6      	PIO_PA20
#define CAM_D5      	PIO_PA19
#define CAM_D4      	PIO_PA18
#define CAM_D3      	PIO_PA17
#define CAM_D2      	PIO_PA16
#define CAM_D1      	PIO_PA22
#define CAM_D0      	PIO_PA24
#define CAM_RESET   	PIO_PA23
#define DIODA1      	PIO_PA28
#define DIODA2      	PIO_PA29

#define PIN_CAM_RESET  { CAM_RESET, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP }
#define PIN_UART_RX { UART_RX, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP }
#define PIN_UART_TX { UART_TX, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP }
#define PIN_PA30 { PIO_PA30, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D0 { CAM_D0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D1 { CAM_D1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D2 { CAM_D2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D3 { CAM_D3, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D4 { CAM_D4, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D5 { CAM_D5, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D6 { CAM_D6, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_CAM_D7 { CAM_D7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_HSYNC  { HSYNC, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_VSYNC  { VSYNC, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP }
#define PIN_DIODA1 { DIODA1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_PULLUP }
#define PIN_DIODA2 { DIODA2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_PULLUP }
#define PIN_MCLK  { MCLK, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT }
#define PIN_PWM1  { PWM1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_PWM2  { PWM2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_PWM3  { PWM3, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_PWM4  { PWM4, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_IN1  { IN1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_IN2  { IN2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_IN3  { IN3, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_IN4  { IN4, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_SERWO_PWM { SERWO_PWM, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_LED_POWER { LED_POWER, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_PULLUP }
#define PIN_SERWO_POWER { SERWO_POWER, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_PULLUP }
#define PIN_I2C_DATA { I2C_DATA, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_I2C_CLOCK { I2C_CLOCK, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
#define PIN_CD4053B_CTRL  {PIO_PA15, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT }

#endif /* PIO_HELPER_H_ */
