/*
 * pwm_helper.c
 *
 *  Created on: Apr 17, 2011
 *      Author: lhanusiak
 */

#include "pwm_helper.h"

#include <pio/pio.h>

////////////////////////////////////////////////////////////////////////////////
// Sterowanie kierunkiem obrotu silnikow
// silnik = 1 - silniki lewe, 2 - silniki prawe
// kierunek = 0 - stop, 1 - przod, 2 - tyl
////////////////////////////////////////////////////////////////////////////////
inline void PWM_SetGear(unsigned char engines, unsigned char gear) {
	if (engines == 1) {
		switch (gear) {
		case 0:
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA7);
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA8);
			break;

		case 1:
//			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, PIO_PA7);
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA8);
			break;

		case 2:
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA7);
//			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, PIO_PA8);
			break;
		}
	}

	if (engines == 2) {
		switch (gear) {
		case 0:
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA9);
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA10);
			break;

		case 1:
//			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, PIO_PA9);
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA10);
			break;

		case 2:
//			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA9);
//			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, PIO_PA10);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Proste sterowanie silnikami
// kierunek 0 - stop, 1 - w lewo, 2 - prosto, 3 - w prawo
////////////////////////////////////////////////////////////////////////////////
inline void go(char kierunek, char pwm1, char pwm2) {
//	switch (kierunek) {
//	case 0:
//		Kierunek(1, 0);
//		Kierunek(2, 0);
//		PWM_Set(0, pwm1);
//		PWM_Set(1, pwm1);
//		PWM_Set(2, pwm2);
//		PWM_Set(3, pwm2);
//		break;
//
//	case 1:
//		Kierunek(1, 2);
//		Kierunek(2, 1);
//		PWM_Set(0, pwm1);
//		PWM_Set(1, pwm1);
//		PWM_Set(2, pwm2);
//		PWM_Set(3, pwm2);
//		break;
//
//	case 2:
//		Kierunek(1, 1);
//		Kierunek(2, 1);
//		PWM_Set(0, pwm1);
//		PWM_Set(1, pwm1);
//		PWM_Set(2, pwm2);
//		PWM_Set(3, pwm2);
//		break;
//
//	case 3:
//		Kierunek(1, 1);
//		Kierunek(2, 2);
//		PWM_Set(0, pwm1);
//		PWM_Set(1, pwm1);
//		PWM_Set(2, pwm2);
//		PWM_Set(3, pwm2);
//		break;
//	}
}
