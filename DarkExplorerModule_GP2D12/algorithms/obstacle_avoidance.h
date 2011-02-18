/*
 * obstacle_avoidance.h
 *
 *  Created on: Feb 16, 2011
 *      Author: lhanusiak
 */

#ifndef OBSTACLE_AVOIDANCE_H_
#define OBSTACLE_AVOIDANCE_H_

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#define STOP_GEAR 0
#define FORWARD_GEAR 1
#define REVERSE_GEAR 2

typedef struct {
	unsigned int speed_left;
	unsigned int speed_right;

	unsigned int gear_left;
	unsigned int gear_right;
} OAA_OUTPUT;

typedef struct {
	unsigned int speed_level_1;
	unsigned int speed_level_2;
	unsigned int speed_level_3;

	unsigned int distance_level_1;
	unsigned int distance_level_2;
	unsigned int distance_level_3;
} OAA_CONFIG;

void use_default_configuration();

unsigned int create_level_mask(unsigned int rs_data, unsigned int ls_data);

OAA_OUTPUT avoid_obstacles(unsigned int level_mask);


#endif /* OBSTACLE_AVOIDANCE_H_ */
