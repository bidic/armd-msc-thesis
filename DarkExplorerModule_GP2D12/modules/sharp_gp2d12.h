#ifndef SHARP_GP2D12_H_
#define SHARP_GP2D12_H_

#include <math.h>

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#include <aic/aic.h>
#include <adc/adc.h>

#define BOARD_ADC_FREQ 6000000
#define ADC_VREF 3300

/*
 * Konfiguruje ADC do odczytu danych z czujnika odległości Sharp GP2D12
 */
void GP2D12_InitializeADC();
/*
 * Na podstawie napięcia podanego i charakterystki znanej z dokumentacji wylicza odległość od przeszkody
 */
unsigned int GP2D12_ComputeDistanceFromCharacterisic(unsigned int output);

void GP2D12_MeasureDistance(unsigned int adc_channel, void(*callback)(
		unsigned int));

#endif /* SHARP_GP2D12_H_ */
