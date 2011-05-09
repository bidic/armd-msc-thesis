/*
 * usart_helper.h
 *
 *  Created on: 08-05-2011
 *      Author: m_nowy
 */
void UART0_DMA_Configure(unsigned long baudrate);

void UART0_DMA_Write(char *data, int ile);
