/*
 * pedometer.c
 *
 *  Created on: Feb 18, 2011
 *      Author: lhanusiak
 */

#include "pedometer.h"

extern Twid twid;

volatile PEDOMETER_CONFIG def_config;
volatile unsigned int pedometer_enabled;
void (*onStepCallback)(void) = NULL;

unsigned int detected_steps;
unsigned int positive_peak_timestamp;
unsigned int positive_acc_peak;
unsigned int negative_peak_timestamp;
unsigned int negative_acc_peak;

volatile double curr_gyroscope_angle = 0;

void init_pedometer_config() {
	def_config.negative_thld = 700;
	def_config.positive_thld = 900;

	def_config.sample_time = 4;
}

unsigned int get_step_count() {
	return detected_steps / 2;
}

volatile char step = 0;
volatile MMA7260_OUTPUT mma_output;

void step_detector(MMA7260_OUTPUT mma7260_output){

	step =1;
	mma_output = mma7260_output;
}

void step_detect(MMA7260_OUTPUT mma7260_output) {
	step = 0;
	//	TRACE_DEBUG("-- Step detector (MMA7260) --\n\r");
	unsigned int peak_type = 0;
	//	int output =
	//			sqrt(pow(mma7260_output.x_normal_mv, 2) + pow(
	//					mma7260_output.y_normal_mv, 2) + pow(
	//					mma7260_output.z_normal_mv, 2));
	int output = ABS(mma7260_output.z_normal_mv);

	//	curr_gyroscope_angle += (mma7260_output.y_mv - 1223.0) / 11500.0;

	if (output <= def_config.negative_thld) {
		negative_acc_peak = output;
		negative_peak_timestamp = RTT_GetTime(AT91C_BASE_RTTC);
		peak_type = NEGATIVE_PEAK;
		TRACE_DEBUG("-- Negative peak detected: %dmV, limit: %dmV, time: %d --\n\r",
				output, def_config.negative_thld, negative_peak_timestamp);
	}

	if (output >= def_config.positive_thld) {
		positive_acc_peak = output;
		positive_peak_timestamp = RTT_GetTime(AT91C_BASE_RTTC);
		peak_type = POSITIVE_PEAK;
		TRACE_DEBUG("-- Positive peak detected: %dmV, limit: %dmV, time: %d --\n\r",
				output, def_config.positive_thld, positive_peak_timestamp);
	}

	if (peak_type && negative_peak_timestamp && positive_peak_timestamp) {
		int sample_duration = ABS(negative_peak_timestamp
				- positive_peak_timestamp);
		TRACE_DEBUG("-------------- Sample duration: %d --\n\r", sample_duration);

		if (sample_duration <= def_config.sample_time) {
			TRACE_INFO("+++++++++++++++ Step detected. Total steps: %d --\n\r", (detected_steps + 1) / 2);
			detected_steps++;
			negative_peak_timestamp = 0;
			positive_peak_timestamp = 0;
			if (!(detected_steps % 2)) {
				onStepCallback();
				L3G4200D_Reset(&twid);
			}
		} else {
			if (peak_type == POSITIVE_PEAK) {
				TRACE_DEBUG("=============== Reset negative sample --\n\r");
				negative_peak_timestamp = 0;
			} else {
				TRACE_DEBUG("************ Reset positive sample --\n\r");
				positive_peak_timestamp = 0;
			}
		}
	}
}

void start_steps_detection(void(*callback)(void)) {
	TRACE_DEBUG("-- Step detector init --\n\r");

	detected_steps = 0;
	pedometer_enabled = 1;
	positive_peak_timestamp = 0;
	negative_peak_timestamp = 0;
	curr_gyroscope_angle = 0;
	onStepCallback = callback;

	TRACE_DEBUG("-- RTT Timer init --\n\r");
	RTT_SetPrescaler(AT91C_BASE_RTTC, 32768 / 2);

	while (pedometer_enabled) {
		L3G4200D_ReadData(&twid);
		if(step){
			step_detect(mma_output);
		}
		MMA7260_ReadOutput(ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7,
				step_detector);
	}
}

unsigned int stop_counting_steps() {
	pedometer_enabled = 0;
	TRACE_DEBUG("-- Step detector disabled. Total steps: %d --\n\r", detected_steps
			/ 2);
	return detected_steps;
}

char is_walking(){
	return 1;
}
