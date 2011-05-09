#ifndef CD4053B_H_
#define CD4053B_H_

#define CD4053B_X_OUTPUTS 0
#define CD4053B_Y_OUTPUTS 1

/**
 * Inicjalizajcja multipleksera obslugujacego
 * analogowa czesc plyty rozszerzen
 */
void CD4053_Initialize();
/**
 * Metoda przelaczajaca odpowiednie wejscia
 * i wyjsca multipleksera
 *
 * \param output_id identyfikator wyjsc multipleksera
 * 					ktore maja zostac wlaczone
 */
void CD4053_EnableOutput(char ouput_id);
/**
 * Metoda przelaczajaca multiplekser w tryb
 * odbioru danych z akcelerometru
 */
void CD4053_EnableAccelerometer();
/**
 * Metoda przelaczajaca multiplekser w tryb
 * odbioru danych z czujnikow odleglosci
 */
void CD4053_EnableIRSensors();

#endif /* CD4053B_H_ */
