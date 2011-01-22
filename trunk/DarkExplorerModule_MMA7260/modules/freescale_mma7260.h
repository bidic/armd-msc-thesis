#ifndef FREESCALE_MMA7260_H_
#define FREESCALE_MMA7260_H_

#include <math.h>

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#include <aic/aic.h>
#include <adc/adc.h>

#define BOARD_ADC_FREQ 6000000
#define ADC_VREF 3300

struct MMA7260State{
	unsigned int x_mv;
	unsigned int y_mv;
	unsigned int z_mv;
};

void MMA7260_InitializeADC();

void MMA7260_ReadState(unsigned int channel_x, unsigned int channel_y,
		unsigned int channel_z, void(*callback)(
		struct MMA7260State));

#endif /* FREESCALE_MMA7260_H_ */
