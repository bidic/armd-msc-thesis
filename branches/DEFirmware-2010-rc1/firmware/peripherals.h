/*
 * peripherals.h
 *
 *  Created on: 2010-06-21
 *      Author: m_nowy
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include <board.h>

////////////////////////////////////////////////////////////////////////////////
// Opoznienie ok 1ms przy MCK=48MHz
////////////////////////////////////////////////////////////////////////////////
void waitms(volatile unsigned long d);

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja timera PIT (serwo)
////////////////////////////////////////////////////////////////////////////////
void PIT_Configure(int czas_us);

////////////////////////////////////////////////////////////////////////////////
// Zmiana interwa�u timera PIT
////////////////////////////////////////////////////////////////////////////////
void PIT_ChangeInterval(int czas_us);

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja 4-rech kana��w PWM dla silnik�w
////////////////////////////////////////////////////////////////////////////////
void PWM_Configure();

////////////////////////////////////////////////////////////////////////////////
// Ustawianie wype�nienia dla wybranego kana�u
////////////////////////////////////////////////////////////////////////////////
void PWM_Set(int channel, char duty);

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - wy��czenie kana��w nadawczego i odbiorczego
////////////////////////////////////////////////////////////////////////////////
void PDC_Disable(AT91PS_PDC pPDC);

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - w��czenie kana�u nadawczego
////////////////////////////////////////////////////////////////////////////////
void PDC_Enable(AT91PS_PDC pPDC);

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - przekazanie danych do nadania
////////////////////////////////////////////////////////////////////////////////
void PDC_SetTX(char *data, int ile, AT91PS_PDC pPDC);

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - przekazanie danych do odbioru
////////////////////////////////////////////////////////////////////////////////
void PDC_SetRX(char *data, int ile, AT91PS_PDC pPDC);

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja USART0 w trybie DMA (DMA tylko do nadawania)
// Odbi�r danych za pomoc� przerwania
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Configure(unsigned long baudrate);

////////////////////////////////////////////////////////////////////////////////
// USART0 - wysy�anie danych za pomoc� kontrolera DMA
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Write(char *data, int ile);

////////////////////////////////////////////////////////////////////////////////
// USART0 - odbieranie danych po wystapieniu przerwania RXRDY
////////////////////////////////////////////////////////////////////////////////
char UART0_Read(void);

////////////////////////////////////////////////////////////////////////////////
// USART0 - odbieranie danych za pomoc� kontrolera DMA - obecnie nie uzywane
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Read(char *data, int ile);

#endif /* PERIPHERALS_H_ */