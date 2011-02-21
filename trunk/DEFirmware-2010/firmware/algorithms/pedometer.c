/*
 * pedometer.c
 *
 *  Created on: Feb 18, 2011
 *      Author: lhanusiak
 */

//TODO Remove
#include "pedometer.h"
//#include "modules/mmc212xm.h"
//#include "algorithms/reverse_track_reconstruction.h"
//extern Twid twid;
PEDOMETER_CONFIG def_config;
volatile unsigned int pedometer_enabled;
void (*onStepCallback)(void) = NULL;

unsigned int detected_steps;
unsigned int positive_peak_timestamp;
unsigned int positive_acc_peak;
unsigned int negative_peak_timestamp;
unsigned int negative_acc_peak;

void init_pedometer_config() {
	def_config.negative_thld = 500;
	def_config.positive_thld = 1100;
//	def_config.negative_thld = 700;
//	def_config.positive_thld = 900;

	def_config.sample_time = 4;
}

unsigned int get_step_count() {
	return detected_steps / 2;
}

volatile int max_v = -1000;
volatile int min_v = 1000;

volatile unsigned int min_acc[3] = { 4000, 4000, 4000 };
volatile unsigned int max_acc[3] = { 0, 0, 0 };

void step_detector(MMA7260_OUTPUT mma7260_output) {
	TRACE_DEBUG("-- Step detector (MMA7260) --\n\r");
	unsigned int peak_type = 0;
	int output =
			sqrt(pow(mma7260_output.x_normal_mv, 2) + pow(
					mma7260_output.y_normal_mv, 2) + pow(
					mma7260_output.z_normal_mv, 2));

//	printf("%4d %4d %4d %4d %4d %4d %4d \r\n", output, mma7260_output.x_normal_mv,
//			mma7260_output.y_normal_mv, mma7260_output.z_normal_mv,
//			((int) mma7260_output.x_mv) - 1650, ((int) mma7260_output.y_mv)
//					- 1650, ((int) mma7260_output.z_mv) - 1650);

	//	if (min_acc[0] > mma7260_output.x_mv)
	//		min_acc[0] = mma7260_output.x_mv;
	//	if (max_acc[0] < mma7260_output.x_mv)
	//		max_acc[0] = mma7260_output.x_mv;
	//	if (min_acc[1] > mma7260_output.y_mv)
	//		min_acc[1] = mma7260_output.y_mv;
	//	if (max_acc[1] < mma7260_output.y_mv)
	//		max_acc[1] = mma7260_output.y_mv;
	//	if (min_acc[2] > mma7260_output.z_mv)
	//		min_acc[2] = mma7260_output.z_mv;
	//	if (max_acc[2] < mma7260_output.z_mv)
	//		max_acc[2] = mma7260_output.z_mv;
	//
	//	printf(
	//			"min_x: %d max_x: %d min_y: %d max_y: %d min_z: %d max_z: %d \r\n",
	//			min_acc[0], max_acc[0], min_acc[1], max_acc[1], min_acc[2],
	//			max_acc[2]);

	//	MMC212xM_SendSetCmd(&twid);
	//	MMC212xM_SendResetCmd(&twid);
	//	mag_info mg_i = MMC212xM_GetMagneticFieldInfo(&twid);
	//	int angle  = compute_angle(mg_i.x,mg_i.y);
	//	printf("%d %d %d %d %d %d %d \n", output, mma7260_output.x_normal_mv, mma7260_output.y_normal_mv, mma7260_output.z_normal_mv, angle, (int)mg_i.x, (int)mg_i.y);
	//TRACE_INFO("-- Axis value: %5d %5d %5d %5d [%4d]--\n\r", output, mma7260_output.x_normal_mv, mma7260_output.y_normal_mv, mma7260_output.z_normal_mv, RTT_GetTime(AT91C_BASE_RTTC));

	if (output <= def_config.negative_thld) {
		negative_acc_peak = output;
		negative_peak_timestamp = RTT_GetTime(AT91C_BASE_RTTC);
		peak_type = NEGATIVE_PEAK;
		TRACE_DEBUG("-- Negative peak detected: %dmV, limit: %dmV, time: %d --\n\r", output, def_config.negative_thld, negative_peak_timestamp);
	}

	if (output >= def_config.positive_thld) {
		positive_acc_peak = output;
		positive_peak_timestamp = RTT_GetTime(AT91C_BASE_RTTC);
		peak_type = POSITIVE_PEAK;
		TRACE_DEBUG("-- Positive peak detected: %dmV, limit: %dmV, time: %d --\n\r", output, def_config.positive_thld, positive_peak_timestamp);
	}

	if (peak_type && negative_peak_timestamp && positive_peak_timestamp) {
		int sample_duration = ABS(negative_peak_timestamp
				- positive_peak_timestamp);
		TRACE_DEBUG("-- Sample duration: %d --\n\r", sample_duration);

		if (sample_duration <= def_config.sample_time) {
			TRACE_INFO("-- Step detected. Total steps: %d --\n\r", (detected_steps + 1) / 2);
			detected_steps++;
			negative_peak_timestamp = 0;
			positive_peak_timestamp = 0;
			if (!(detected_steps % 2)) {
				onStepCallback();
			}
		} else {
			if (peak_type == POSITIVE_PEAK) {
				TRACE_DEBUG("-- Reset negative sample --\n\r");
				negative_peak_timestamp = 0;
			} else {
				TRACE_DEBUG("-- Reset positive sample --\n\r");
				positive_peak_timestamp = 0;
			}
		}
	}

}

void start_steps_detection(void(*callback)(void)) {
	TRACE_DEBUG("-- Step detector init --\n\r");

	max_v = -1000;
	min_v = 1000;
	detected_steps = 0;
	pedometer_enabled = 1;
	positive_peak_timestamp = 0;
	negative_peak_timestamp = 0;
	onStepCallback = callback;

	TRACE_DEBUG("-- RTT Timer init --\n\r");
	RTT_SetPrescaler(AT91C_BASE_RTTC, 32768 / 2);

	TRACE_DEBUG("-- Accelerometer initialization (MMA7260) --\n\r");
	MMA7260_InitializeADC();

	while (pedometer_enabled) {
		MMA7260_ReadOutput(ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7,
				step_detector);
	}
}

unsigned int stop_counting_steps() {
	pedometer_enabled = 0;
	TRACE_DEBUG("-- Step detector disabled. Total steps: %d --\n\r", detected_steps / 2);
	return detected_steps;
}
