/*
 * reverse_track_reconstruction.c
 *
 *  Created on: Feb 19, 2011
 *      Author: lhanusiak
 */

#include <peripherals/pwm_helper.h>
#include <peripherals/adc_helper.h>

#include "utils.h"
#include "peripherals.h"

#include "algorithms/pedometer.h"
#include "algorithms/obstacle_avoidance.h"

#include "modules/mmc212xm.h"
#include "modules/gyroscope.h"
#include "modules/l3g4200d.h"
#include "modules/hy1602f6.h"
#include "modules/cd4053b.h"

#include "reverse_track_reconstruction.h"

double steps_data[MAX_STEPS];

extern Twid twid;
extern double curr_gyroscope_angle;
volatile unsigned int level_mask;

double compute_angle(double x, double y) {
	if (!x && !y)
		return 0;

	if (!x)
		return y < 0 ? 270.0 : 90.0;

	double angle = atan(y / x);

	if (y < 0 && x > 0)
		angle += M_PI * 2.0;
	else if (x < 0)
		angle += M_PI;

	return angle * 180.0 / M_PI;
}

double compute_return_angle(double compute_angle) {
	double return_angle = compute_angle + 180.0;

	if (return_angle >= 360)
		return_angle -= 360.0;

	return return_angle;
}

double compute_turn_angle(double dest_angle, double current_angle) {
	// + left - right

	double turn_angle = current_angle - dest_angle;
	if (ABS(turn_angle) > 180.0)
		turn_angle = turn_angle - SIGN(turn_angle) * 360.0;

	return turn_angle;
}

double get_descrete_angle(double in) {
	double angle = ABS(in);
	int sign = SIGN(in);

	if (angle < 15) {
		angle = 0;
	}
	//
	//		if (angle < 22.5)
	//			return 0;
	//		if (angle > 22.5 && angle < 67.5)
	//			return 45 * sign;
	//		if (angle > 67.5 && angle < 112.5)
	//			return 90 * sign;
	//		if (angle > 112.5 && angle < 157.5)
	//			return 135 * sign;
	//		if (angle > 157.5 && angle < 202.5)
	//			return 180 * sign;
	//		if (angle > 202.5 && angle < 247.5)
	//			return 225 * sign;
	//		if (angle > 247.5 && angle < 292.5)
	//			return 270 * sign;
	//		if (angle > 292.5 && angle < 337.5)
	//			return 315 * sign;
	//		if (angle > 337.5)
	//			return 0;

	//	if(angle < 45)
	//		return 0;
	//	if(angle > 45 && angle < 135)
	//		return 90;
	//	if(angle > 135 && angle < 225)
	//		return 180;
	//	if(angle > 225 && angle < 315)
	//		return 270;
	//	if(angle > 315)
	//		return 0;

	return angle * sign;
}

void onStepDetectedCallback(void) {
	TRACE_DEBUG("-- Step detected %3d -- \r\n", get_step_count());

	//	mag_info mg_i = MMC212xM_GetMagneticFieldInfo(&twid);
	steps_data[get_step_count() - 1] = get_descrete_angle(
			L3G4200D_GetData().sAngle_z);

	L3G4200D_Reset(&twid);

	char txt_buff[16];
	sprintf(txt_buff, "s: %2d a: %3d", get_step_count(),
			(int) steps_data[get_step_count() - 1]);
	HY1602F6_Log("Step detected", txt_buff);

	TRACE_INFO("-- Detected angle: %4d, Descrete angle: %d -- \r\n", L3G4200D_GetData().sAngle_z, (int)steps_data[get_step_count() - 1]);

}

void start_recording_track() {
	TRACE_DEBUG("-- record_track -- \r\n");
	HY1602F6_Log("Recording start", "");

	init_pedometer_config();
	start_steps_detection(onStepDetectedCallback);
}

void stop_recording_track() {
	TRACE_DEBUG("-- stop_recording_track -- \r\n");

	unsigned int steps = stop_counting_steps();

	char output[16];
	sprintf(output, "Steps rec: %3d", steps);

	HY1602F6_Log("Recording stop", output);
}

void turn_of_angle(double angle) {
//	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, 0x1 << 29);
	L3G4200D_Reset(&twid);

	PWM_Set(0, 15);
	waitms(1000);
	if (angle < 0) {
		TRACE_INFO("-- w lewo -- \r\n");
		PWM_SetGear(RIGHT_ENGINES, FORWARD_GEAR);
		PWM_SetGear(LEFT_ENGINES, REVERSE_GEAR);
	} else {
		TRACE_INFO("-- w prawo -- \r\n");
		PWM_SetGear(RIGHT_ENGINES, REVERSE_GEAR);
		PWM_SetGear(LEFT_ENGINES, FORWARD_GEAR);
	}

	while (ABS(angle) - ABS(L3G4200D_GetData().sAngle_z) > 0) {
		L3G4200D_ReadData(&twid);
	}

	L3G4200D_Reset(&twid);
	PWM_SetGear(RIGHT_ENGINES, STOP_GEAR);
	PWM_SetGear(LEFT_ENGINES, STOP_GEAR);

//	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, 0x1 << 29);
}

void turn_at_magnetic_angle(double angle) {
	double turn_angle = 0;

	mag_info current_mg_i = MMC212xM_GetMagneticFieldInfo(&twid);
	double current_angle = compute_angle(current_mg_i.y, current_mg_i.x);
	turn_angle = compute_turn_angle(angle, current_angle);

	PWM_Set(0, 15);
	if (turn_angle > 0) {
		PWM_SetGear(RIGHT_ENGINES, REVERSE_GEAR);
		PWM_SetGear(LEFT_ENGINES, FORWARD_GEAR);
	} else {
		PWM_SetGear(RIGHT_ENGINES, FORWARD_GEAR);
		PWM_SetGear(LEFT_ENGINES, REVERSE_GEAR);
	}

	do {
		current_mg_i = MMC212xM_GetMagneticFieldInfo(&twid);
		current_angle = compute_angle(current_mg_i.y, current_mg_i.x);
		turn_angle = compute_turn_angle(angle, current_angle);

		waitms(5);

	} while (ABS(turn_angle) > 1);

	PWM_SetGear(RIGHT_ENGINES, STOP_GEAR);
	PWM_SetGear(LEFT_ENGINES, STOP_GEAR);

}

void onDistanceDataRdy(unsigned int rs_output, unsigned int ls_output) {
	level_mask = create_level_mask(rs_output, ls_output);
}

char obstacle_detect(OAA_OUTPUT *result) {
	ADC_StartDoubleChannelConversion(ADC_CHANNEL_7, ADC_CHANNEL_5,
			onDistanceDataRdy);

	while (!ADC_IsDataReady())
		;

	*result = avoid_obstacles(level_mask);

	return result->gear_left != result->gear_right;
}

volatile unsigned char call_count = 0;
volatile char val = 0;
volatile char go_prosto_count = 0;

char avoid_obstacle();

char go_prosto() {
	char result = 0;
	char step_count = 0;

	if (val == 2) {
		go_prosto_count++;
	}

	for (; step_count < 38; step_count++) {

		ADC_StartDoubleChannelConversion(ADC_CHANNEL_7, ADC_CHANNEL_5,
				onDistanceDataRdy);

		while (!ADC_IsDataReady())
			;

		OAA_OUTPUT oa_result = avoid_obstacles(level_mask);

		if (oa_result.gear_left == oa_result.gear_right) {
			PWM_Set(0, oa_result.speed_left);
			PWM_SetGear(RIGHT_ENGINES, FORWARD_GEAR);
			PWM_SetGear(LEFT_ENGINES, FORWARD_GEAR);
			waitms(100);
		}

		result = avoid_obstacle();
	}

	return result;
}

char avoid_obstacle() {
	char result = 0;

	OAA_OUTPUT oa_result;
	call_count++;

	if (call_count > 6) {
		return 1;
	}

	if (obstacle_detect(&oa_result)) {
		char buff[16];
		sprintf(buff, "Obstacle %2d", call_count);
		HY1602F6_Log(buff, "detected");

		//		int angle = oa_result.speed_left == 190 ? 45 : 90;
		int angle = 90;
		int sign = oa_result.gear_left == FORWARD_GEAR ? 1 : -1;

		turn_of_angle(angle * sign);
		result = go_prosto();
		if (!result) {
			turn_of_angle(-(angle * sign));
			result = go_prosto();

			if (val == 1) {
				val = 2;
				turn_of_angle(-(angle * sign));
				result = go_prosto();
			}

			if (!result) {
				if (call_count == 1) {
					val = 0;
					turn_of_angle(angle * sign);
				} else {
					if (go_prosto_count > 1) {
						go_prosto_count--;
					} else {
						result = go_prosto();
					}
				}
			}
		}
	} else {
		if (call_count > 1 && !val)
			val = 1;
	}
	call_count--;

	return result;
}

void reconstruct_reverse_track() {
	if (get_step_count() > 0) {
		TRACE_DEBUG("-- reconstruct_reverse_track -- \r\n");
		char txt_buff[16];
		sprintf(txt_buff, "Total steps: %2d", get_step_count());
		HY1602F6_Log("Reversing track", txt_buff);

		init_oa_configuration();
		CD4053_EnableIRSensors();
		waitms(1000);

		int step_count = get_step_count() - 1;
		for (; step_count >= 0; step_count--) {
			char lcd_buff1[16];
			sprintf(lcd_buff1, "angle: %3d", (int) steps_data[step_count]);
			char lcd_buff2[16];
			sprintf(lcd_buff2, "step: %3d", step_count);
			HY1602F6_Log(lcd_buff1, lcd_buff2);

			turn_of_angle(steps_data[step_count]);

			go_prosto();
		}

		PWM_SetGear(RIGHT_ENGINES, STOP_GEAR);
		PWM_SetGear(LEFT_ENGINES, STOP_GEAR);
	} else {
		TRACE_WARNING("-- No steps recorded. --\r\n");
		HY1602F6_Log("Invalid track", "No data found");
	}
}
