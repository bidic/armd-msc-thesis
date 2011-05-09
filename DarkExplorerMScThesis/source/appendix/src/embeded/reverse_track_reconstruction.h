#ifndef REVERSE_TRACK_RECONSTRUCTION_H_
#define REVERSE_TRACK_RECONSTRUCTION_H_

// Maksymalna ilosc krokow jaka moze zapamietac robot
#define MAX_STEPS 300

#define SIGN(n) (((n) < 0) ? -1 : 1)
#ifndef ABS
	#define ABS(n) ( ((n) < 0) ? -(n) : (n))
#endif

/**
 * Metoda rozpoczynajaca nagrywanie sciezki
 */
void start_recording_track();
/**
 * Metoda konczaca proces nagrywania sciezki
 */
void stop_recording_track();
/**
 * Metoda rozpoczynajaca proces rekonstrukcji sciezki
 */
void reconstruct_reverse_track();

#endif /* REVERSE_TRACK_RECONSTRUCTION_H_ */
