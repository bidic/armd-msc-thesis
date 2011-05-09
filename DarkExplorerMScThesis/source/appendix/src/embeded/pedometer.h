#ifndef PEDOMETER_H_
#define PEDOMETER_H_

#define POSITIVE_PEAK 1
#define NEGATIVE_PEAK 2
#define ABS(n) (((n) < 0) ? -(n) : (n))

/**
 * Struktura przechowywujaca aktualna konfiguracje wykrywacza krokow
 */
typedef struct {
	// prog napiecia ponizej ktorego
	// przyspieszenie bedzie traktowane jako ujemne
	unsigned int negative_thld;

	// prog napiecia powyzej ktorego
	// przyspieszenie bedzie traktowane jako dodatnie
	unsigned int positive_thld;

	// czas w ktorym musi wystapic zmiana +/-
	// aby zostala uznana za krok
	unsigned int sample_time;
} PEDOMETER_CONFIG;

/**
 * Metoda inicjalizujaca konfiguracje algorytmu
 * do wykrywania krokow
 */
void init_pedometer_config();
/**
 * Metoda uruchamiajaca algorytm zliczania
 * kolejnych krokow
 *
 * \param onStepCallback funkcja wywolywana po wykryciu kroku
 */
void start_steps_detection(void (*onStepCallback)(void));
/**
 * Metoda zatrzymujaca algorytm rozpoznawania
 * i zliczania krokow
 */
unsigned int stop_counting_steps();
/**
 * Metoda zwracajaca aktualnie wykryta liczbe krokow
 *
 * \return liczbe wykrytych w aktualnej sesji krokow
 */
unsigned int get_step_count();

#endif /* PEDOMETER_H_ */
