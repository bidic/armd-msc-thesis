#ifndef FREESCALE_MMA7260_H_
#define FREESCALE_MMA7260_H_

/**
 * Struktura opisujaca wartosci przyspieszenia
 * odczytane za pomoca akcelerometru
 */
typedef struct {
	// znormalizowana wartosc napiecia dla osi X,Y,Z
	// wartosc [-850mV, 850mV] odpowiada odpowiednio[-1g, 1g]
	int x_normal_mv;
	int y_normal_mv;
	int z_normal_mv;

	// wartosc napiecia odczytana bezposrednio z akcelerometru
	unsigned int x_mv;
	unsigned int y_mv;
	unsigned int z_mv;
} MMA7260_OUTPUT;

/**
 * Metoda korygujaca wartosci odczytane bezposrednio z akcelerometru
 * o roznice ustalone w procesie kalibracji urzadzenia
 *
 * \param channel_x wartosc napiecia odczytana na osi X
 * \param channel_y wartosc napiecia odczytana na osi Y
 * \param channel_z wartosc napiecia odczytana na osi Z
 * \param callback funkcja uruchamiana po odczytaniu danych
 * 					z akcelerometru
 */
void MMA7260_ReadOutput(unsigned int channel_x, unsigned int channel_y,
		unsigned int channel_z, void(*callback)(MMA7260_OUTPUT));

#endif /* FREESCALE_MMA7260_H_ */
