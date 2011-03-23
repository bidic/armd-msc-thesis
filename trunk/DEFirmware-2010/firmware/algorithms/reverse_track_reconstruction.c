/*
 * reverse_track_reconstruction.c
 *
 *  Created on: Feb 19, 2011
 *      Author: lhanusiak
 */

#include "reverse_track_reconstruction.h"

double steps_data[MAX_STEPS];

extern Twid twid;
extern double curr_gyroscope_angle;

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

	if (angle < 20) {
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

	TRACE_INFO("-- Detected angle: %4d, Descrete angle: %d -- \r\n", L3G4200D_GetData().sAngle_z, (int)steps_data[get_step_count() - 1]);

}

void start_recording_track() {
	TRACE_DEBUG("-- record_track -- \r\n");

	init_pedometer_config();
	start_steps_detection(onStepDetectedCallback);
}

void stop_recording_track() {
	TRACE_DEBUG("-- stop_recording_track -- \r\n");

	stop_counting_steps();
}

void turn_of_angle(double angle) {
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, 0x1 << 29);
	//	reset_angle();
	L3G4200D_Reset(&twid);

	PWM_Set(0, 15);
	waitms(1000);
	if (angle < 0) {
		TRACE_INFO("-- w lewo -- \r\n");
		Kierunek(RIGHT_ENGINES, FORWARD_GEAR);
		Kierunek(LEFT_ENGINES, REVERSE_GEAR);
	} else {
		TRACE_INFO("-- w prawo -- \r\n");
		Kierunek(RIGHT_ENGINES, REVERSE_GEAR);
		Kierunek(LEFT_ENGINES, FORWARD_GEAR);
	}
	//	waitms(1000);
	//	PWM_Set(0, 15);
	//	int i = 0;
	while (ABS(angle) - ABS(L3G4200D_GetData().sAngle_z) > 2) {
		//		read_gyroscope_output(ADC_CHANNEL_6);


		L3G4200D_ReadData(&twid);

		//		if (!(++i % 1000)) {
		//			TRACE_INFO("-- angle %d  curr: %d-- \r\n", (int)angle, (int)L3G4200D_GetData().sAngle_z);
		//			i = 1;
		//		}
	}

	//	reset_angle();
	L3G4200D_Reset(&twid);
	Kierunek(RIGHT_ENGINES, STOP_GEAR);
	Kierunek(LEFT_ENGINES, STOP_GEAR);

	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, 0x1 << 29);
}

void turn_at_magnetic_angle(double angle) {
	double turn_angle = 0;

	mag_info current_mg_i = MMC212xM_GetMagneticFieldInfo(&twid);
	double current_angle = compute_angle(current_mg_i.y, current_mg_i.x);
	turn_angle = compute_turn_angle(angle, current_angle);

	PWM_Set(0, 15);
	if (turn_angle > 0) {
		Kierunek(RIGHT_ENGINES, REVERSE_GEAR);
		Kierunek(LEFT_ENGINES, FORWARD_GEAR);
	} else {
		Kierunek(RIGHT_ENGINES, FORWARD_GEAR);
		Kierunek(LEFT_ENGINES, REVERSE_GEAR);
	}

	do {
		current_mg_i = MMC212xM_GetMagneticFieldInfo(&twid);
		current_angle = compute_angle(current_mg_i.y, current_mg_i.x);
		turn_angle = compute_turn_angle(angle, current_angle);

		//	TRACE_INFO("-- x: %d y: %d ta: %d ca: %d --\r\n", current_mg_i.y, current_mg_i.x, (int) turn_angle, (int) current_angle);


		waitms(5);

	} while (ABS(turn_angle) > 1);

	Kierunek(RIGHT_ENGINES, STOP_GEAR);
	Kierunek(LEFT_ENGINES, STOP_GEAR);

}

volatile unsigned int oa_data_ready = 0;
//volatile unsigned int ls_data = 0;
//volatile unsigned int rs_data = 0;
volatile unsigned int level_mask;

void onDistanceDataRdy(unsigned int rs_output, unsigned int ls_output) {
	level_mask = create_level_mask(rs_output, ls_output);
	//printf("%d %d\r\n",rs_output,ls_output);
	oa_data_ready = 1;
}

volatile int offset_angle = 0;

char obstacle_detect(OAA_OUTPUT *result) {
	ADC_StartDoubleChannelConversion(ADC_CHANNEL_5, ADC_CHANNEL_6,
			onDistanceDataRdy);

	while (!oa_data_ready)
		;

	oa_data_ready = 0;
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

	for (; step_count < 7; step_count++) {
		ADC_StartDoubleChannelConversion(ADC_CHANNEL_5, ADC_CHANNEL_6,
				onDistanceDataRdy);

		while (!oa_data_ready)
			;

		oa_data_ready = 0;
		OAA_OUTPUT oa_result = avoid_obstacles(level_mask);

		if (oa_result.gear_left == oa_result.gear_right) {
			PWM_Set(0, oa_result.speed_left);
			Kierunek(RIGHT_ENGINES, FORWARD_GEAR);
			Kierunek(LEFT_ENGINES, FORWARD_GEAR);
			waitms(200);
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
					if (go_prosto_count > 0) {
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

		init_oa_configuration();

		int step_count = get_step_count() - 1;

		for (; step_count >= 0; step_count--) {

			turn_of_angle(steps_data[step_count]);

			go_prosto();
			//			unsigned int mariuszek_count = 0;
			//			for (; mariuszek_count < 4; mariuszek_count++) {
			//				ADC_StartDoubleChannelConversion(ADC_CHANNEL_5, ADC_CHANNEL_6,
			//						onDistanceDataRdy);
			//
			//				while (!oa_data_ready)
			//					;
			//
			//				oa_data_ready = 0;
			//				OAA_OUTPUT oa_result = avoid_obstacles(level_mask);
			//
			//				if (oa_result.gear_left == oa_result.gear_right) {
			//					PWM_Set(0, oa_result.speed_left);
			//					Kierunek(RIGHT_ENGINES, FORWARD_GEAR);
			//					Kierunek(LEFT_ENGINES, FORWARD_GEAR);
			//					waitms(200);
			//				}
			//
			//				//			waitms(2000);
			//				avoid_obstacle();
			//
			//			}
		}

		Kierunek(RIGHT_ENGINES, STOP_GEAR);
		Kierunek(LEFT_ENGINES, STOP_GEAR);
	} else {
		TRACE_WARNING("-- No steps recorded. --\r\n");
	}
}
