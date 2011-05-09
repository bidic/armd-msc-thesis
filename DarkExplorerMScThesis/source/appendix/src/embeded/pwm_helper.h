#ifndef PWM_HELPER_H_
#define PWM_HELPER_H_

#define PWM_CHANNEL 0

// Tryb pracy silnikow: STOP
#define STOP_GEAR 0
// Tryb pracy silnikow: DO PRZODU
#define FORWARD_GEAR 1
// Tryb pracy silnikow: WSTECZ
#define REVERSE_GEAR 2

// Silniki lewe
#define LEFT_ENGINES 1
// Silniki prawe
#define RIGHT_ENGINES 2

/**
 * Konfiguracja kanalu PWM dla silnikow
 */
void PWM_Configure();

/**
 * Sterowanie kierunkiem obrotu silnikow
 *
 * \param engines identyfikator grupy silnikow
 * \param gear identyfikator trybu pracy silnikow
 */
inline void PWM_SetGear(unsigned char engines, unsigned char gear);

/**
 * Konfiguracja pracy silnikow
 *
 * \param gear identyfikator trybu pracy silnikow
 * \param l_speed predkosc silnikow lewych
 * \param r_speed predkosc silnikow prawych
 */
inline void ConfigureEngines(unsigned char gear, unsigned char l_speed, unsigned char r_speed);

#endif /* PWM_HELPER_H_ */
