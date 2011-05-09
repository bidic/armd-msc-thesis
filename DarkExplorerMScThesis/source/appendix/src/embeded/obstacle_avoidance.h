#ifndef OBSTACLE_AVOIDANCE_H_
#define OBSTACLE_AVOIDANCE_H_

#define STOP_GEAR 0
#define FORWARD_GEAR 1
#define REVERSE_GEAR 2
/**
 * Struktura opisujaca wyjsciowa konfiguracje silnikow
 * pozwalajaca na bezpieczne ominiecie przeszkody
 */
typedef struct {
	// Predkosc silnikow lewych
	unsigned int speed_left;
	// Predkosc silnikow prawych
	unsigned int speed_right;

	// Tryb pracy silnikow lewych
	unsigned int gear_left;
	// Tryb pracy silnikow prawych
	unsigned int gear_right;
} OAA_OUTPUT;
/**
 * Struktura przechowywujaca konfiguracje algorytmu
 * omijania przeszkod
 */
typedef struct {
	// Predkosc na 1 poziomie odleglosci
	unsigned int speed_level_1;
	// Predkosc na 2 poziomie odleglosci
	unsigned int speed_level_2;
	// Predkosc na 3 poziomie odleglosci
	unsigned int speed_level_3;

	// Definicja 1 poziomu odleglosci
	unsigned int distance_level_1;
	// Definicja 2 poziomu odleglosci
	unsigned int distance_level_2;
	// Definicja 3 poziomu odleglosci
	unsigned int distance_level_3;
} OAA_CONFIG;

/**
 * Metoda inicjalizujaca konfiguracje algorytmu
 * omijania przeszkod
 */
void init_oa_configuration();
/**
 * Metoda obliczajaca maske poziomow na podstawie danych
 * o odleglosci robota od przeszkody
 *
 * \param rs_data dane o odleglosci z prawego czujnika
 * \param ls_data dane o odleglosci z lewego czujnika
 *
 * \return maska bitowa z opisem poziomow odleglosci
 */
unsigned int create_level_mask(unsigned int rs_data, unsigned int ls_data);

/**
 * Metoda wyznaczajaca na podstawie maski poziomow konfiguracje
 * silnikow pozwlajaca na bezpieczne ominiecie przeszkody
 *
 * \param level_mask maska bitowa z opisem poziomow odleglosci
 * \return konfiguracja silnikow pozwalajaca na ominiecie przeszkody
 */
OAA_OUTPUT avoid_obstacles(unsigned int level_mask);

#endif /* OBSTACLE_AVOIDANCE_H_ */
