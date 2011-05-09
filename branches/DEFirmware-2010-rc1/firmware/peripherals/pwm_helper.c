/*
 * pwm_helper.c
 *
 *  Created on: Apr 17, 2011
 *      Author: lhanusiak
 */

#include "pwm_helper.h"
#include "pio_helper.h"

#include <pio/pio.h>
#include <pmc/pmc.h>
#include <pwmc/pwmc.h>

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja kanalu PWM dla silnikow
////////////////////////////////////////////////////////////////////////////////
void PWM_Configure()
{
  //wlaczenie zegara dla pwm
  PMC_EnablePeripheral(AT91C_ID_PWMC);

  PWMC_ConfigureChannel(PWM_CHANNEL, 0x6 << 0, AT91C_PWMC_CALG, 0x1 << 9 | 0x0
      << 10);
  // rozdzielczosc = 255
  PWMC_SetPeriod(0, 255);

  // wypelnienie poczatkowe = 1
  PWMC_SetDutyCycle(0, 1);

  //Uruchomienie wszystkich kana��w PWM
  PWMC_EnableChannel(0);
}

////////////////////////////////////////////////////////////////////////////////
// Ustawianie wype�nienia dla wybranego kana�u
////////////////////////////////////////////////////////////////////////////////
void PWM_Set(int channel, char duty)
{
  //ustawienie wypelnienia
  // channel = 0 - 3, duty = 2 - 255
  PWMC_SetDutyCycle(channel, duty);
}

////////////////////////////////////////////////////////////////////////////////
// Sterowanie kierunkiem obrotu silnikow
// silnik = 1 - silniki lewe, 2 - silniki prawe
// kierunek = 0 - stop, 1 - przod, 2 - tyl
////////////////////////////////////////////////////////////////////////////////
void PWM_SetGear(unsigned char engines, unsigned char gear)
{
  Pin pin_engine1 = PIN_ENGINE1;
  Pin pin_engine2 = PIN_ENGINE2;
  Pin pin_engine3 = PIN_ENGINE3;
  Pin pin_engine4 = PIN_ENGINE4;

  if (engines == 1)
  {
    switch (gear)
    {
      case 0:
        PIO_Clear(&pin_engine1);
        PIO_Clear(&pin_engine2);
        break;

      case 1:
        PIO_Set(&pin_engine1);
        PIO_Clear(&pin_engine2);
        break;

      case 2:
        PIO_Clear(&pin_engine1);
        PIO_Set(&pin_engine2);
        break;
    }
  }

  if (engines == 2)
  {
    switch (gear)
    {
      case 0:
        PIO_Clear(&pin_engine3);
        PIO_Clear(&pin_engine4);
        break;

      case 1:
        PIO_Set(&pin_engine3);
        PIO_Clear(&pin_engine4);
        break;

      case 2:
        PIO_Clear(&pin_engine3);
        PIO_Set(&pin_engine4);
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Proste sterowanie silnikami
// kierunek 0 - stop, 1 - w lewo, 2 - prosto, 3 - w prawo
////////////////////////////////////////////////////////////////////////////////
inline void go(char kierunek, char pwm1, char pwm2)
{
  	/*switch (kierunek) {
  	case 0:
  		Kierunek(1, 0);
  		Kierunek(2, 0);
  		PWM_Set(0, pwm1);
  		PWM_Set(1, pwm1);
  		PWM_Set(2, pwm2);
  		PWM_Set(3, pwm2);
  		break;

  	case 1:
  		Kierunek(1, 2);
  		Kierunek(2, 1);
  		PWM_Set(0, pwm1);
  		PWM_Set(1, pwm1);
  		PWM_Set(2, pwm2);
  		PWM_Set(3, pwm2);
  		break;

  	case 2:
  		Kierunek(1, 1);
  		Kierunek(2, 1);
  		PWM_Set(0, pwm1);
  		PWM_Set(1, pwm1);
  		PWM_Set(2, pwm2);
  		PWM_Set(3, pwm2);
  		break;

  	case 3:
  		Kierunek(1, 1);
  		Kierunek(2, 2);
  		PWM_Set(0, pwm1);
  		PWM_Set(1, pwm1);
  		PWM_Set(2, pwm2);
  		PWM_Set(3, pwm2);
  		break;
  	}*/
}
