/*
 * usart_helper.c
 *
 *  Created on: 08-05-2011
 *      Author: m_nowy
 */

#include "de_board.h"
#include "usart_helper.h"
#include "pio_helper.h"

#include <pmc/pmc.h>
#include <usart/usart.h>
#include <aic/aic.h>

__ramfunc void UART0_DMA_irq_handler(void);

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - wy��czenie kana��w nadawczego i odbiorczego
////////////////////////////////////////////////////////////////////////////////
void PDC_Disable(AT91PS_PDC pPDC)
{
  //wylaczenie nadajnika i odbiornika
  pPDC->PDC_PTCR = (AT91C_PDC_TXTDIS | AT91C_PDC_RXTDIS);

  //zerowanie rejestrow
  pPDC->PDC_TPR = 0; // Transmit Pointer Register
  pPDC->PDC_TCR = 0; // Transmit Counter Register
  pPDC->PDC_TNPR = 0; // Transmit Next Pointer Register
  pPDC->PDC_TNCR = 0; // Transmit Next Counter Register

  pPDC->PDC_RPR = 0; // Receive Pointer Register
  pPDC->PDC_RCR = 0; // Receive Counter Register
  pPDC->PDC_RNPR = 0; // Receive Next Pointer Register
  pPDC->PDC_RNCR = 0; // Receive Next Counter Register
}

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - w��czenie kana�u nadawczego
////////////////////////////////////////////////////////////////////////////////
void PDC_Enable(AT91PS_PDC pPDC)
{
  //wlaczenie nadajnika /*i odbiornika*/
  pPDC->PDC_PTCR = (AT91C_PDC_TXTEN /*| AT91C_PDC_RXTEN*/);
}

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - przekazanie danych do nadania
////////////////////////////////////////////////////////////////////////////////
void PDC_SetTX(char *data, int ile, AT91PS_PDC pPDC)
{
  //ustawienie rejestr�w nadawczych
  pPDC->PDC_TPR = (unsigned int) data; // Transmit Pointer Register
  pPDC->PDC_TCR = ile; // Transmit Counter Register
}

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - przekazanie danych do odbioru
////////////////////////////////////////////////////////////////////////////////
void PDC_SetRX(char *data, int ile, AT91PS_PDC pPDC)
{
  //ustawienie rejestr�w odbiorczych
  pPDC->PDC_RPR = (unsigned int) data; // Receive Pointer Register
  pPDC->PDC_RCR = ile; // Receive Counter Register
}

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja USART0 w trybie DMA (DMA tylko do nadawania)
// Odbi�r danych za pomoc� przerwania
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Configure(unsigned long baudrate)
{
  float podzielnik_float;
  int calkowity, ulamkowy;
  unsigned int mode = AT91C_US_USMODE_NORMAL | /* Normal Mode */
  AT91C_US_CLKS_CLOCK | /* Clock = MCK */
  AT91C_US_CHRL_8_BITS | /* 8-bit Data  */
  AT91C_US_PAR_NONE | /* No Parity   */
  AT91C_US_NBSTOP_1_BIT; /* 1 Stop Bit  */
  Pin pins_usart[] = { PIN_USART0_RXD, PIN_USART0_TXD };

  //Wlaczenie zegara dla UART0
  PMC_EnablePeripheral(AT91C_ID_US0);

  USART_Configure(AT91C_BASE_US0, mode, baudrate, BOARD_MCK);

  podzielnik_float = ((float) MCK / 16.0 / (float) baudrate);
  calkowity = podzielnik_float;
  podzielnik_float = (podzielnik_float - calkowity) * 8;
  ulamkowy = podzielnik_float;
  AT91C_BASE_US0->US_BRGR = (calkowity) | (ulamkowy << 16); /* podzielnik calkowity + podzielnik ulamkowy */

  //wylaczenie przyjmowania przerwan
  AT91C_BASE_US0->US_IDR = 0xFFFFFFFF;

  USART_SetReceiverEnabled(AT91C_BASE_US0, 0);
  USART_SetTransmitterEnabled(AT91C_BASE_US0, 0);

  //wylaczenie DMA
  PDC_Disable(AT91C_BASE_PDC_US0);

  //konfiguracja przerwania
  AIC_ConfigureIT(AT91C_ID_US0, 0x3 | AT91C_AIC_SRCTYPE_HIGH_LEVEL,
      UART0_DMA_irq_handler);
  AIC_EnableIT(AT91C_ID_US0);

  //w��czenie USART0
  USART_SetReceiverEnabled(AT91C_BASE_US0, 1);
  USART_SetTransmitterEnabled(AT91C_BASE_US0, 1);

  //wlaczenie DMA (nadawanie)
  PDC_Enable(AT91C_BASE_PDC_US0);

  PIO_Configure(pins_usart, PIO_LISTSIZE(pins_usart));

  //wlaczenie przerwania od odbiornika
  AT91C_BASE_US0->US_IER = (0x1 << 0); //RXRDYd
}

////////////////////////////////////////////////////////////////////////////////
// USART0 - wysy�anie danych za pomoc� kontrolera DMA
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Write(char *data, int ile)
{
  USART_WriteBuffer(AT91C_BASE_US0,data,ile);
  return;
  //oczekiwanie na zakonczenie wysylania poprzedniego zestawu
  while (AT91C_BASE_PDC_US0->PDC_TCR)
    ;

  //rozpoczecie wysylania nowego zestawu danych
  PDC_SetTX(data, ile, AT91C_BASE_PDC_US0);

  //przerwanie gdy transfer sie zakonczy
  //AT91C_BASE_US0->US_IER = (0x1 << 4);
}

////////////////////////////////////////////////////////////////////////////////
// USART0 - odbieranie danych po wystapieniu przerwania RXRDY
////////////////////////////////////////////////////////////////////////////////
char UART0_Read(void)
{
  return USART_Read(AT91C_BASE_US0, 10);
}
