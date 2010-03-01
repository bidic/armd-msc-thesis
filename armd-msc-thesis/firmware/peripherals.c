/**
* Projekt    : Dark Explorer firmware
* Plik       : perupherals.c
* Zawartosc  : Funkcje do obs³ugi urz¹dzeñ peryferyjnych procesora
* Autor      : Pawe³ Kmak
* Data       : 1.08.2009
**/

////////////////////////////////////////////////////////////////////////////////
// Prototypy funkcji obs³ugi przerwan
////////////////////////////////////////////////////////////////////////////////
__ramfunc void SYSTEM_INTERRUPT_irq_handler(void);
__ramfunc void UART0_DMA_irq_handler(void);

////////////////////////////////////////////////////////////////////////////////
// Opoznienie ok 1ms przy MCK=48MHz
////////////////////////////////////////////////////////////////////////////////
void waitms(volatile unsigned long d)
{
  // dla MCK 48MHz
  d=2000*d;
  for (;d ;--d);
}

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja timera PIT (serwo)
////////////////////////////////////////////////////////////////////////////////
void PIT_Configure(int czas_us)
{
  unsigned int dummy, piv;
  float tmp;
   
  //wy³¹czenie timera PIT na czas konfiguracji
  AT91C_BASE_PITC->PITC_PIMR = ~(AT91C_PITC_PITEN | AT91C_PITC_PITIEN);
  
  //oczekiwanie na wyzerowanie licznika (pole CPIV w rejestrze PIVR)
  while(AT91C_BASE_PITC->PITC_PIVR & AT91C_PITC_CPIV);
  
  //wyzerowanie potencjalnego ¿¹dania przerwania
  dummy = AT91C_BASE_PITC->PITC_PIVR;
  
  //konfiguracja przerwania
  AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST, AT91C_AIC_SRCTYPE_HIGH_LEVEL, SYSTEM_INTERRUPT_irq_handler);	
  AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_SYS);
  
  //w³¹czenie timera PIT i ustawienie interwa³u
  //piv = (   ( czas_us * ( (MCK/16)/1000 ) ) / 1000   );
  tmp = ( (((float)MCK)/(16000000.0)) * (float)czas_us ) - 1.0;
  piv = tmp;
  AT91C_BASE_PITC->PITC_PIMR = ( piv | AT91C_PITC_PITEN | AT91C_PITC_PITIEN ); 
}

////////////////////////////////////////////////////////////////////////////////
// Zmiana interwa³u timera PIT
////////////////////////////////////////////////////////////////////////////////
void PIT_ChangeInterval(int czas_us)
{
  unsigned int piv;
  float tmp;
  tmp = ( (((float)MCK)/(16000000.0)) * (float)czas_us ) - 1.0;
  piv = tmp;
  AT91C_BASE_PITC->PITC_PIMR = ( piv | AT91C_PITC_PITEN | AT91C_PITC_PITIEN ); 
}

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja 4-rech kana³ów PWM dla silników
////////////////////////////////////////////////////////////////////////////////
void PWM_Configure()
{
  //wlaczenie zegara dla pwm
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PWMC );
  
  //wylaczenie wszystkich kanalow przed zmiana konfiguracji
  AT91C_BASE_PWMC->PWMC_DIS = (AT91C_PWMC_CHID0 | AT91C_PWMC_CHID1 | AT91C_PWMC_CHID2 | AT91C_PWMC_CHID3);

  //oczekiwanie na wy³¹czenie wszystkich kanalow
  while(AT91C_BASE_PWMC->PWMC_SR & (AT91C_PWMC_CHID0 | AT91C_PWMC_CHID1 | AT91C_PWMC_CHID2 | AT91C_PWMC_CHID3) );
  
  //KONFIGURACJA TRYBU PRACY
  //    - prescaler = MCK/64
  //    - alligment = center
  //    - polarity = high
  //    - update = duty
  AT91C_BASE_PWMC->PWMC_CH[0].PWMC_CMR = (0x6 << 0 | 0x1 << 8 | 0x1 << 9 | 0x0 << 10);
  AT91C_BASE_PWMC->PWMC_CH[1].PWMC_CMR = (0x6 << 0 | 0x1 << 8 | 0x1 << 9 | 0x0 << 10);
  AT91C_BASE_PWMC->PWMC_CH[2].PWMC_CMR = (0x6 << 0 | 0x1 << 8 | 0x1 << 9 | 0x0 << 10);
  AT91C_BASE_PWMC->PWMC_CH[3].PWMC_CMR = (0x6 << 0 | 0x1 << 8 | 0x1 << 9 | 0x0 << 10);
  
  // rozdzielczosc = 255
  AT91C_BASE_PWMC->PWMC_CH[0].PWMC_CPRDR = 256;
  AT91C_BASE_PWMC->PWMC_CH[1].PWMC_CPRDR = 256;
  AT91C_BASE_PWMC->PWMC_CH[2].PWMC_CPRDR = 256;
  AT91C_BASE_PWMC->PWMC_CH[3].PWMC_CPRDR = 256;
  
  // wypelnienie poczatkowe = 1
  AT91C_BASE_PWMC->PWMC_CH[0].PWMC_CDTYR = 2;
  AT91C_BASE_PWMC->PWMC_CH[1].PWMC_CDTYR = 2;
  AT91C_BASE_PWMC->PWMC_CH[2].PWMC_CDTYR = 2;
  AT91C_BASE_PWMC->PWMC_CH[3].PWMC_CDTYR = 2;
  
  //KONFIGURACJA WYPROWADZENIA PA11, PA12, PA13, PA14
  AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, 0, (AT91C_PA11_PWM0 | AT91C_PA12_PWM1 | AT91C_PA13_PWM2 | AT91C_PA14_PWM3) );
  
  //Uruchomienie wszystkich kana³ów PWM
  AT91C_BASE_PWMC->PWMC_ENA = (AT91C_PWMC_CHID0 | AT91C_PWMC_CHID1 | AT91C_PWMC_CHID2 | AT91C_PWMC_CHID3);
}

////////////////////////////////////////////////////////////////////////////////
// Ustawianie wype³nienia dla wybranego kana³u
////////////////////////////////////////////////////////////////////////////////
void PWM_Set(int channel, char duty)
{
  //ustawienie wypelnienia
  // channel = 0 - 3, duty = 2 - 255
  AT91C_BASE_PWMC->PWMC_CH[channel].PWMC_CUPDR = duty;
}

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - wy³¹czenie kana³ów nadawczego i odbiorczego
////////////////////////////////////////////////////////////////////////////////
void PDC_Disable(AT91PS_PDC pPDC)
{
  //wylaczenie nadajnika i odbiornika
  pPDC->PDC_PTCR = (AT91C_PDC_TXTDIS | AT91C_PDC_RXTDIS);
  
  //zerowanie rejestrow
  pPDC->PDC_TPR = 0;      // Transmit Pointer Register
  pPDC->PDC_TCR = 0;      // Transmit Counter Register
  pPDC->PDC_TNPR = 0;     // Transmit Next Pointer Register
  pPDC->PDC_TNCR = 0;     // Transmit Next Counter Register
  
  pPDC->PDC_RPR = 0;      // Receive Pointer Register
  pPDC->PDC_RCR = 0;      // Receive Counter Register
  pPDC->PDC_RNPR = 0;     // Receive Next Pointer Register
  pPDC->PDC_RNCR = 0;     // Receive Next Counter Register
}

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - w³¹czenie kana³u nadawczego
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
  //ustawienie rejestrów nadawczych
  pPDC->PDC_TPR = (unsigned int)data;      // Transmit Pointer Register
  pPDC->PDC_TCR = ile;                       // Transmit Counter Register
}

////////////////////////////////////////////////////////////////////////////////
// Kontroler DMA - przekazanie danych do odbioru
////////////////////////////////////////////////////////////////////////////////
void PDC_SetRX(char *data, int ile, AT91PS_PDC pPDC)
{
  //ustawienie rejestrów odbiorczych
  pPDC->PDC_RPR = (unsigned int)data;      // Receive Pointer Register
  pPDC->PDC_RCR = ile;                       // Receive Counter Register
}

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja USART0 w trybie DMA (DMA tylko do nadawania)
// Odbiór danych za pomoc¹ przerwania
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Configure(unsigned long baudrate)
{
  float podzielnik_float;
  int calkowity, ulamkowy;
  
  //W³¹czenie zegara dla UART0
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_US0 );
  
  //wylaczenie przyjmowania przerwan
  AT91C_BASE_US0->US_IDR=0xFFFFFFFF;
  
  //reset portu
  AT91C_BASE_US0->US_CR = AT91C_US_RSTRX |          /* Reset Receiver      */
                          AT91C_US_RSTTX |          /* Reset Transmitter   */
                          AT91C_US_RXDIS |          /* Receiver Disable    */
                          AT91C_US_TXDIS;           /* Transmitter Disable */
  //wylaczenie DMA
  PDC_Disable(AT91C_BASE_PDC_US0);
  
  //konfiguracja USART0
  AT91C_BASE_US0->US_MR = AT91C_US_USMODE_NORMAL |   /* Normal Mode */
                          AT91C_US_CLKS_CLOCK    |    /* Clock = MCK */
                          AT91C_US_CHRL_8_BITS   |    /* 8-bit Data  */
                          AT91C_US_PAR_NONE      |    /* No Parity   */
                          AT91C_US_NBSTOP_1_BIT;      /* 1 Stop Bit  */
  //predkosc
  //AT91C_BASE_US0->US_BRGR = (MCK/16/baudrate); 
  //wyliczenie podzielnikow zegara MCLK
  podzielnik_float = ( (float)MCK / 16.0 / (float)baudrate );
  calkowity = podzielnik_float;
  podzielnik_float = (podzielnik_float - calkowity) * 8;
  ulamkowy = podzielnik_float;
  AT91C_BASE_US0->US_BRGR = ( calkowity ) | ( ulamkowy << 16 );       /* podzielnik calkowity + podzielnik ulamkowy */
  
  //konfiguracja przerwania
  AT91F_AIC_ConfigureIt ( AT91C_BASE_AIC, AT91C_ID_US0, 0x3, AT91C_AIC_SRCTYPE_HIGH_LEVEL, UART0_DMA_irq_handler);	
  AT91F_AIC_EnableIt (AT91C_BASE_AIC, AT91C_ID_US0);
  
  //w³¹czenie USART0
  AT91C_BASE_US0->US_CR = AT91C_US_RXEN  |          /* Receiver Enable     */
                          AT91C_US_TXEN;            /* Transmitter Enable  */
  
  //wlaczenie DMA (nadawanie)
  PDC_Enable(AT91C_BASE_PDC_US0);
  
  //konfiguracja wyprowadzen
  AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, (AT91C_PA5_RXD0 | AT91C_PA6_TXD0), 0 );
  
  //wlaczenie przerwania od odbiornika
  AT91C_BASE_US0->US_IER = (0x1 << 0); //RXRDY
}

////////////////////////////////////////////////////////////////////////////////
// USART0 - wysy³anie danych za pomoc¹ kontrolera DMA
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Write(char *data, int ile)
{
  //oczekiwanie na zakonczenie wysylania poprzedniego zestawu
  while( AT91C_BASE_PDC_US0->PDC_TCR );
  
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
  return (AT91C_BASE_US0->US_RHR & 0xFF);
}

////////////////////////////////////////////////////////////////////////////////
// USART0 - odbieranie danych za pomoc¹ kontrolera DMA - obecnie nie uzywane
////////////////////////////////////////////////////////////////////////////////
void UART0_DMA_Read(char *data, int ile)
{
  //oczekiwanie na zakonczenie odbierania poprzedniego zestawu
  while( AT91C_BASE_PDC_US0->PDC_RCR );
  
  //rozpoczecie odbierania nowego zestawu danych
  PDC_SetRX(data, ile, AT91C_BASE_PDC_US0);
  
  //przerwanie gdy transfer sie zakonczy
  AT91C_BASE_US0->US_IER = (0x1 << 3);
}

////////////////////////////////////////////////////////////////////////////////
// Konfiguracja przetwornika Analogowo-Cyfrowego
////////////////////////////////////////////////////////////////////////////////
void ADC_Configure(int adc_clock)
{
  //W³¹czenie zegara dla ADC
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_ADC );
  
  //zamaskowanie przerwan
  AT91C_BASE_ADC->ADC_IDR = 0xFFFFFFFF;
  
  //reset przetwornika
  AT91C_BASE_ADC->ADC_CR = AT91C_ADC_SWRST;
  
  //obliczenia parametrów PRESCALER, STARTUP, SHTIM dla zadanego adc_clock
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
// Dostepne 4 kana³y dedykowane
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

////////////////////////////////////////////////////////////////////////////////
// TWI - Konfiguracja
// twi_clock min 100kHz dla MCK=48MHz, 200KHz dla MCK=96MHz
// max 400KHz
////////////////////////////////////////////////////////////////////////////////
void TWI_Configure(int twi_clock)
{
  float dzielnik;
  int dzielnik_calkowity;

  //wlaczenie zegara dla twi
  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_TWI );
  
  //reset
  AT91C_BASE_TWI->TWI_CR = AT91C_TWI_SWRST;

  // Ustawienie zegara z wypelnieniem 50% na czestotliwosc twi_clock
  twi_clock = twi_clock<<1;
  dzielnik = (float)MCK / ( (float)twi_clock );
  dzielnik_calkowity = dzielnik;

  AT91C_BASE_TWI->TWI_CWGR = (AT91C_TWI_CKDIV & (0 << 16)) | 
                             (AT91C_TWI_CHDIV & (dzielnik_calkowity << 8)) | 
                             (AT91C_TWI_CLDIV & (dzielnik_calkowity << 0));

  //tryb master transfer
  AT91C_BASE_TWI->TWI_CR = AT91C_TWI_MSEN;

  //konfiguracja wyprowadzen PA3, PA4
  AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, (AT91C_PA3_TWD | AT91C_PA4_TWCK), 0 );

  //bez wewnetrznego podciagania
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~PIO_PA3 ) ;
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~PIO_PA4 ) ;
}

////////////////////////////////////////////////////////////////////////////////
// TWI - Wysylanie
// TWI_Write( adres slave'a, adres, dane )
////////////////////////////////////////////////////////////////////////////////
char TWI_Write(int SlaveAddr, int IntAddr, char data)
{
  char end_transmission=0, Return=0;

  AT91C_BASE_TWI->TWI_CR = AT91C_TWI_START;
  AT91C_BASE_TWI->TWI_MMR = AT91C_TWI_IADRSZ_1_BYTE | (SlaveAddr << 16); //adres sleve'a
  AT91C_BASE_TWI->TWI_IADR = IntAddr; //adres
  AT91C_BASE_TWI->TWI_THR = data; //dane

  waitms(1);

  //oczekiwanie na zakonczenie transmisji
  while (!end_transmission)
  {
    if (AT91C_BASE_TWI->TWI_SR & AT91C_TWI_NACK)
    {
      //brak potwierdzenie - blad i zakonczenie
      Return = 0 ;
      end_transmission=1;
    }
    else if (AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXRDY)
    {
      //jest potwierdzenie, ok i zakonczenie
      Return = 1;
      end_transmission=1;
    }
  }

  AT91C_BASE_TWI->TWI_CR = AT91C_TWI_STOP;

  //oczekiwanie na zakonczenie transmisji
  while (!(AT91C_BASE_TWI->TWI_SR & AT91C_TWI_TXCOMP));

  return Return;
}
