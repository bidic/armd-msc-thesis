/**
 * Projekt    : Dark Explorer firmware
 * Plik       : perupherals.c
 * Zawartosc  : Funkcje do obs�ugi urz�dze� peryferyjnych procesora
 * Autor      : Pawe� Kmak
 * Data       : 1.08.2009
 **/

#include <de_board.h>
#include <aic/aic.h>
#include <pmc/pmc.h>
#include <pwmc/pwmc.h>
#include <usart/usart.h>

////////////////////////////////////////////////////////////////////////////////
// Prototypy funkcji obs�ugi przerwan
////////////////////////////////////////////////////////////////////////////////
__ramfunc void SYSTEM_INTERRUPT_irq_handler(void);
__ramfunc void UART0_DMA_irq_handler(void);

////////////////////////////////////////////////////////////////////////////////
// Opoznienie ok 1ms przy MCK=48MHz
////////////////////////////////////////////////////////////////////////////////
void waitms(volatile unsigned long d) {
	// dla MCK 48MHz
	d = 2000 * d;
	for (; d; --d)
		;
}

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja timera PIT (serwo)
////////////////////////////////////////////////////////////////////////////////
void PIT_Configure(int czas_us) {
	unsigned int dummy, piv;
	float tmp;

	//wy��czenie timera PIT na czas konfiguracji
	AT91C_BASE_PITC->PITC_PIMR = ~(AT91C_PITC_PITEN | AT91C_PITC_PITIEN);

	//oczekiwanie na wyzerowanie licznika (pole CPIV w rejestrze PIVR)
	while (AT91C_BASE_PITC->PITC_PIVR & AT91C_PITC_CPIV)
		;

	//wyzerowanie potencjalnego ��dania przerwania
	dummy = AT91C_BASE_PITC->PITC_PIVR;

	//konfiguracja przerwania
	AIC_ConfigureIT(AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST
			| AT91C_AIC_SRCTYPE_HIGH_LEVEL, SYSTEM_INTERRUPT_irq_handler);
	//  AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST, AT91C_AIC_SRCTYPE_HIGH_LEVEL, SYSTEM_INTERRUPT_irq_handler);
	AIC_EnableIT(AT91C_ID_SYS);
	//  AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_SYS);

	//w��czenie timera PIT i ustawienie interwa�u
	//piv = (   ( czas_us * ( (MCK/16)/1000 ) ) / 1000   );
	tmp = ((((float) MCK) / (16000000.0)) * (float) czas_us) - 1.0;
	piv = tmp;
	AT91C_BASE_PITC->PITC_PIMR = (piv | AT91C_PITC_PITEN | AT91C_PITC_PITIEN);
}

////////////////////////////////////////////////////////////////////////////////
// Zmiana interwa�u timera PIT
////////////////////////////////////////////////////////////////////////////////
void PIT_ChangeInterval(int czas_us) {
	unsigned int piv;
	float tmp;
	tmp = ((((float) MCK) / (16000000.0)) * (float) czas_us) - 1.0;
	piv = tmp;
	AT91C_BASE_PITC->PITC_PIMR = (piv | AT91C_PITC_PITEN | AT91C_PITC_PITIEN);
}




