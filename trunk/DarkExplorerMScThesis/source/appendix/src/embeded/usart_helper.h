#ifndef USART_HELPER_H_
#define USART_HELPER_H_

/**
 * Metoda konfigurujaca modul bluetooth
 *
 * \param baudrate Szybkosc pracy modulu bluetooth
 */
void UART0_DMA_Configure(unsigned long baudrate);

/**
 * Metoda wysylajaca dane za posrednictwem modulu bluetooth
 *
 * \param data Bufor z danymi
 * \param size Ilosc danych do wyslania
 */
void UART0_DMA_Write(char *data, int size);

#endif /* USART_HELPER_H_ */
