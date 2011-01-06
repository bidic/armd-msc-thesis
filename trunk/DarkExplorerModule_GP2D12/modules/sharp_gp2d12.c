/*
 * sharp_gp2d12.c
 *
 *  Created on: Jul 11, 2010
 *      Author: lhanusiak
 */

#include "sharp_gp2d12.h"
#include "delay.h"

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja przetwornika Analogowo-Cyfrowego
////////////////////////////////////////////////////////////////////////////////
void ADC_Configure(int adc_clock)
{
	Pin pinsADC[] = {PINS_ADC};
  //W��czenie zegara dla ADC
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_ADC );

  //zamaskowanie przerwan
  AT91C_BASE_ADC->ADC_IDR = 0xFFFFFFFF;

  //reset przetwornika
  AT91C_BASE_ADC->ADC_CR = AT91C_ADC_SWRST;

  //obliczenia parametr�w PRESCALER, STARTUP, SHTIM dla zadanego adc_clock
  ///// PRESCALER
  int prescaler, adc_clock_real;
  prescaler = ( MCK / (2 * adc_clock) ) - 1;
  if (prescaler > 63){prescaler = 63;} //wartosc max
  adc_clock_real = ( MCK / (2 * (prescaler + 1)) );

  ///// STARTUP
  int startup;
  startup = ( (10 * adc_clock_real) / (8*50000) );
  if ( (startup % 10) >= 1) { startup = (((startup / 10) + 1) - 1); }
    else { startup = ((startup / 10) - 1); }

  ///// SHTIM
  int sh;
  sh = ( (100 * adc_clock_real) / ( 100000 ) );
  if ( (sh % 100) >= 1 ) { sh = (((sh / 100) + 1) - 1); }
    else { sh = ((sh / 100) - 1); }

  //parametry pracy ADC
  AT91C_BASE_ADC->ADC_MR = AT91C_ADC_TRGEN_DIS          |  /* wylaczone wyzwalanie sprzetowe */
                           AT91C_ADC_LOWRES_10_BIT      |  /* rozdzielczosc 10bit */
                           AT91C_ADC_SLEEP_NORMAL_MODE  |  /* tryb normalny */
                           prescaler << 8               |  /* PRESCALER */
                           startup << 16                |  /* STARTUP */
                           sh << 24;                       /* SHTIM */
}

////////////////////////////////////////////////////////////////////////////////
// ADC - rozpoczecie konwersji analog-digital
////////////////////////////////////////////////////////////////////////////////
// Dostepne 4 kana�y dedykowane
// AT91C_ADC_CH4 - Channel 4
// AT91C_ADC_CH5 - Channel 5
// AT91C_ADC_CH6 - Channel 6
// AT91C_ADC_CH7 - Channel 7
////////////////////////////////////////////////////////////////////////////////
void ADC_StartConversion(int channel_mask)
{
  //disable
  AT91C_BASE_ADC->ADC_CHDR = 0xFFFFFFFF;

  //enable channel
  AT91C_BASE_ADC->ADC_CHER = channel_mask;

  //start conversion
  AT91C_BASE_ADC->ADC_CR = AT91C_ADC_START;
}

////////////////////////////////////////////////////////////////////////////////
// ADC - odczytanie wyniku konwersji (8 bit)
////////////////////////////////////////////////////////////////////////////////
char ADC_Read_8bit(int channel_mask)
{
  //oczekiwanie na zakonczenie konwersji
  while( AT91C_BASE_ADC->ADC_SR & channel_mask );

  //odczyt wyniku (8 mlodszych bitow)
  switch (channel_mask)
  {
    case AT91C_ADC_CH4: return (AT91C_BASE_ADC->ADC_CDR4 & 0xFF); break;
    case AT91C_ADC_CH5: return (AT91C_BASE_ADC->ADC_CDR5 & 0xFF); break;
    case AT91C_ADC_CH6: return (AT91C_BASE_ADC->ADC_CDR6 & 0xFF); break;
    case AT91C_ADC_CH7: return (AT91C_BASE_ADC->ADC_CDR7 & 0xFF); break;
    default: return 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
// ADC - odczytanie wyniku konwersji (2 najstarsze bity)
////////////////////////////////////////////////////////////////////////////////
char ADC_Read_2bit(int channel_mask)
{
  //oczekiwanie na zakonczenie konwersji
  while( AT91C_BASE_ADC->ADC_SR & channel_mask );

  //odczyt wyniku (2 najstarsze bity)
  switch (channel_mask)
  {
    case AT91C_ADC_CH4: return (AT91C_BASE_ADC->ADC_CDR4 & 0x300) >> 8; break;
    case AT91C_ADC_CH5: return (AT91C_BASE_ADC->ADC_CDR5 & 0x300) >> 8; break;
    case AT91C_ADC_CH6: return (AT91C_BASE_ADC->ADC_CDR6 & 0x300) >> 8; break;
    case AT91C_ADC_CH7: return (AT91C_BASE_ADC->ADC_CDR7 & 0x300) >> 8; break;
    default: return 0;
  }
}
