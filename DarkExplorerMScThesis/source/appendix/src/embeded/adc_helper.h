#ifndef ADC_HELPER_H_
#define ADC_HELPER_H_

#define BOARD_ADC_FREQ 6000000
#define ADC_VREF 3300

/**
 * Metoda konfigurujaca modul konwertera analogowo-cyfrowego
 */
void ADC_Configure();
/**
 * Metoda rozpoczynajaca konwersje dla pojedynczego kanalu
 *
 * \param channel identyfikator kanalu dla ktorego ma sie odbyc konwersja
 * \param callback metoda uruchamiana po zakonczeniu konwersji
 */
void ADC_StartSingleChannelConversion(unsigned int channel,
		void(*callback)(unsigned int));
/**
 * Metoda rozpoczynajaca konwersje dla dwoch kanalow
 *
 * \param channel_a identyfikator 1 kanalu dla ktorego ma sie odbyc konwersja
 * \param channel_b identyfikator 2 kanalu dla ktorego ma sie odbyc konwersja
 * \param callback metoda uruchamiana po zakonczeniu konwersji
 */
void ADC_StartDoubleChannelConversion(unsigned int channel_a,
		unsigned int channel_b, void(*callback)(unsigned int, unsigned int));
/**
 * Metoda rozpoczynajaca konwersje dla trzech kanalow
 *
 * \param channel_a identyfikator 1 kanalu dla ktorego ma sie odbyc konwersja
 * \param channel_b identyfikator 2 kanalu dla ktorego ma sie odbyc konwersja
 * \param channel_c identyfikator 3 kanalu dla ktorego ma sie odbyc konwersja
 * \param callback metoda uruchamiana po zakonczeniu konwersji
 */
void ADC_StartTripleChannelConversion(unsigned int channel_a,
		unsigned int channel_b, unsigned int channel_c,
		void(*callback)(unsigned int, unsigned int, unsigned int));

unsigned char ADC_IsDataReady();

#endif /* ADC_HELPER_H_ */
