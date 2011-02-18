/*
 * obstacle_avoidance.c
 *
 *  Created on: Feb 17, 2011
 *      Author: lhanusiak
 */

#include "obstacle_avoidance.h"

OAA_CONFIG def_config;

void use_default_configuration() {
	def_config.distance_level_1 = 2100;
	def_config.distance_level_2 = 1100;
	def_config.distance_level_3 = 700;

	def_config.speed_level_1 = 150;
	def_config.speed_level_2 = 200;
	def_config.speed_level_3 = 255;
}

unsigned int create_level_mask(unsigned int rs_data, unsigned int ls_data) {
	unsigned int level_mask = 0;

	if (rs_data >= def_config.distance_level_3) {
		level_mask |= 32;
	}

	if (rs_data >= def_config.distance_level_2) {
		level_mask |= 16;
	}

	if (rs_data >= def_config.distance_level_1) {
		level_mask |= 8;
	}

	if (ls_data >= def_config.distance_level_3) {
		level_mask |= 4;
	}

	if (ls_data >= def_config.distance_level_2) {
		level_mask |= 2;
	}

	if (ls_data >= def_config.distance_level_1) {
		level_mask |= 1;
	}

	return level_mask;
}

OAA_OUTPUT speed_output(unsigned int speed_level) {
	OAA_OUTPUT output;

	output.gear_left = FORWARD_GEAR;
	output.gear_right = FORWARD_GEAR;
	output.speed_left = speed_level;
	output.speed_right = speed_level;

	return output;
}

OAA_OUTPUT left_turn_output(unsigned int speed_level) {
	OAA_OUTPUT output;

	output.gear_left = REVERSE_GEAR;
	output.gear_right = FORWARD_GEAR;
	output.speed_left = speed_level;
	output.speed_right = speed_level;

	return output;
}

OAA_OUTPUT right_turn_output(unsigned int speed_level) {
	OAA_OUTPUT output;

	output.gear_right = REVERSE_GEAR;
	output.gear_left = FORWARD_GEAR;
	output.speed_left = speed_level;
	output.speed_right = speed_level;

	return output;
}

OAA_OUTPUT avoid_obstacles(unsigned int level_mask) {
	// max speed
	if (level_mask == 0) {
		return speed_output(def_config.speed_level_3);
	}

	// medium speed 000100 | 100000
	if (level_mask == 4 || level_mask == 32) {
		return speed_output(def_config.speed_level_2);
	}

	// slow speed 110110 | 100100 | 110100 | 100110 | 110000 | 000110
	if (level_mask == 6 || level_mask == 36 || level_mask == 38 || level_mask
			== 48 || level_mask == 52 || level_mask == 54) {
		return speed_output(def_config.speed_level_1);
	}

	// slow left turn 000111 | 100111 | 110111
	if (level_mask == 7 || level_mask == 39 || level_mask == 55) {
		return left_turn_output(def_config.speed_level_2);
	}

	// slow right turn 111000 | 111100 | 111110
	if (level_mask == 56 || level_mask == 60 || level_mask == 62) {
		return right_turn_output(def_config.speed_level_2);
	}

	// fast left turn 111111
	//if (level_mask == 63) {
	return left_turn_output(def_config.speed_level_3);
	//}
}
