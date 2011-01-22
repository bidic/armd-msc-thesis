/*
 * delay.c
 *
 *  Created on: Jul 21, 2010
 *      Author: lhanusiak
 */

#include "delay.h"

void wait(volatile unsigned int d) {
	for (; d; --d);
}

