#ifndef HY1602F6_H_
#define HY1602F6_H_

#define AT24C_ADDRESS   56

/**
 * Metoda inicjalizujaca konfiguracje wyswietlacza LCD
 */
void HY1602F6_Init(void);
/**
 * Metoda wypisujaca na ekran tekst podany jako parametr
 *
 * \param text tekst do wypisania
 */
void HY1602F6_PrintText(const char* text);
/**
 * Metoda usuwajaca caly tekst z wyswietlacza
 */
void HY1602F6_ClearDisplay(void);
/**
 * Metoda ustawiajaca kursor na podanej pozycji
 *
 * \param pos pozycja kursora
 */
void HY1602F6_SetCursorPos(unsigned char pos);
/**
 * Metoda ustawiajaca kursor na poczatku pierwszego wiersza
 */
void HY1602F6_SetCursorHome(void);
/**
 * Metoda ustawiajaca kursor na poczatku drugiego wiersza
 */
void HY1602F6_StartNextLine();
/**
 * Metoda animujaca pasek postepu zapomoca wyswietlacza
 *
 * \param val aktualna wartosc
 * \param clear 1 jezeli wyswietlacz ma zostac w calosci
 * 					wyczyszczony przed aktualizacja
 */
void HY1602F6_SetProgress(unsigned char val, unsigned char clear);
/**
 * Metoda pozwalajaca na wypisanie dwu linijkowego tekstu
 *
 * \param line1 tekst w pierwszej linijce
 * \param line2 tekst w drugiej linijce
 */
void HY1602F6_Log(const char* line1, const char* line2);

#endif /* HY1602F6_H_ */
