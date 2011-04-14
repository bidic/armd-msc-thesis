/*
 * reverse_track_reconstruction.h
 *
 *  Created on: Feb 19, 2011
 *      Author: lhanusiak
 */

#ifndef REVERSE_TRACK_RECONSTRUCTION_H_
#define REVERSE_TRACK_RECONSTRUCTION_H_

#define MAX_STEPS 300
#ifndef ABS
#define ABS(n) ( ((n) < 0) ? -(n) : (n))
#endif
#define SIGN(n) (((n) < 0) ? -1 : 1)

void start_recording_track();
void stop_recording_track();
void reconstruct_reverse_track();
void turn_at_angle(double angle);

double compute_angle(double x, double y);

void turn_of_angle(double angle);


#endif /* REVERSE_TRACK_RECONSTRUCTION_H_ */
