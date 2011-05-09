#ifndef MMC212XM_H_
#define MMC212XM_H_

#define MMC2120M_ADDRESS    0x30
#define MMC2120M_CMD_TM     0x1
#define MMC2120M_CMD_SET    0x1 << 1
#define MMC2120M_CMD_RESET  0x1 << 2
#define MMC2120M_MAX_VAL    0x1 << 11

#define BIT12TO32(b1,b2) -((MMC2120M_MAX_VAL) - ((b1) << 8 | (b2)))

/**
 * Struktura z danymi wyjsciowymi z magnetometru
 */
typedef struct _mag_info {
	// Wartosc skladowej X indukcji pola magnetycznego
	double x;
	// Wartosc skladowej Y indukcji pola magnetycznego
	double y;
} mag_info;

/**
 * Metoda pobierajaca dane wysciowe z magnetometru
 *
 * \param twid Wskaznik do interfejsu TWI
 * \return struktura z danymi wyjsciowymi
 */
mag_info MMC212xM_GetMagneticFieldInfo(Twid *twid);
/**
 * Metoda
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void MMC212xM_SendSetCmd(Twid *twid);
/**
 * Metoda resetujaca modul magnetometru
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void MMC212xM_SendResetCmd(Twid *twid);
/**
 * Metoda kalibrujaca magnetometr
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void MMC212xM_Calibrate(Twid *twid);

#endif /* MMC212XM_H_ */
