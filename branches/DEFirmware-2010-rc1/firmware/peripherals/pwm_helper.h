/*
 * pwm_helper.h
 *
 *  Created on: Apr 17, 2011
 *      Author: lhanusiak
 */

#ifndef PWM_HELPER_H_
#define PWM_HELPER_H_

#define STOP_GEAR 0
#define FORWARD_GEAR 1
#define REVERSE_GEAR 2

#define LEFT_ENGINES 1
#define RIGHT_ENGINES 2

////////////////////////////////////////////////////////////////////////////////
// Sterowanie kierunkiem obrotu silnikow
// silnik = 1 - silniki lewe, 2 - silniki prawe
// kierunek = 0 - stop, 1 - przod, 2 - tyl
////////////////////////////////////////////////////////////////////////////////
inline void PWM_SetGear(unsigned char engines, unsigned char gear);

////////////////////////////////////////////////////////////////////////////////
// Proste sterowanie silnikami
// kierunek 0 - stop, 1 - w lewo, 2 - prosto, 3 - w prawo
////////////////////////////////////////////////////////////////////////////////
inline void go(char kierunek, char pwm1, char pwm2);


#endif /* PWM_HELPER_H_ */
