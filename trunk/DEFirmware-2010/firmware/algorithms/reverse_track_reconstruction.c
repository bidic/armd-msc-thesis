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

		if (angle < 22.5)
			return 0;
		if (angle > 22.5 && angle < 67.5)
			return 45 * sign;
		if (angle > 67.5 && angle < 112.5)
			return 90 * sign;
		if (angle > 112.5 && angle < 157.5)
			return 135 * sign;
		if (angle > 157.5 && angle < 202.5)
			return 180 * sign;
		if (angle > 202.5 && angle < 247.5)
			return 225 * sign;
		if (angle > 247.5 && angle < 292.5)
			return 270 * sign;
		if (angle > 292.5 && angle < 337.5)
			return 315 * sign;
		if (angle > 337.5)
			return 0;

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

	return angle;
}

void onStepDetectedCallback(void) {
	TRACE_DEBUG("-- Step detected %3d -- \r\n", get_step_count());

	//	mag_info mg_i = MMC212xM_GetMagneticFieldInfo(&twid);
	steps_data[get_step_count() - 1] = get_descrete_angle(curr_gyroscope_angle);

	TRACE_INFO("-- Detected angle: %4d, Descrete angle: %d -- \r\n", (int) curr_gyroscope_angle, (int)steps_data[get_step_count() - 1]);

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
	reset_angle();

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
	while (ABS(angle) - ABS(get_current_angle()) > 2) {
		read_gyroscope_output(ADC_CHANNEL_6);

		//		if (!(++i % 1000)) {
		//			TRACE_INFO("-- angle %d  curr: %d-- \r\n", (int)angle, (int)get_current_angle());
		//			i = 1;
		//		}
	}

	reset_angle();
	Kierunek(RIGHT_ENGINES, STOP_GEAR);
	Kierunek(LEFT_ENGINES, STOP_GEAR);

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

void reconstruct_reverse_track() {
	if (get_step_count() > 0) {
		TRACE_DEBUG("-- reconstruct_reverse_track -- \r\n");

		int step_count = get_step_count() - 1;

		for (; step_count >= 0; step_count--) {
			turn_of_angle(steps_data[step_count]);

			PWM_Set(0, 15);
			Kierunek(RIGHT_ENGINES, FORWARD_GEAR);
			Kierunek(LEFT_ENGINES, FORWARD_GEAR);

			waitms(2000);
		}

		Kierunek(RIGHT_ENGINES, STOP_GEAR);
		Kierunek(LEFT_ENGINES, STOP_GEAR);
	} else {
		TRACE_WARNING("-- No steps recorded. --\r\n");
	}
}
