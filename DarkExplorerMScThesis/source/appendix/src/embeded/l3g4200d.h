#ifndef L3G4200D_H_
#define L3G4200D_H_

#define L3G4200D_ADDRESS                        0x69
#define L3G4200D_CTRL_REG1                      0x20
#define L3G4200D_CTRL_REG4                      0x23
#define L3G4200D_CMD_CTRL_REG1_POWER_ON         0x0F
#define L3G4200D_CTRL_REG5                      0x24
#define L3G4200D_CMD_CTRL_REG5_FIFO_EN          0x40
#define L3G4200D_FIFO_CTRL_REG                  0x2E
#define L3G4200D_CMD_FIFO_CTRL_REG_STREAM_MODE  0x40
#define L3G4200D_READ_REG                       0xA6
#define L3G4200D_FIFO_SRC_REG                   0x2F

#define BIT8TO16(b1,b2) ((b1) << 8 | (b2))

/**
 * Struktura z danymi wyjsciowymi z zyroskopu
 */
typedef struct _gyro_data {
	// Kat obrotu wykryty na osi X
	short sAngle_x;
	// Kat obrotu wykryty na osi Y
	short sAngle_y;
	// Kat obrotu wykryty na osi Z
	short sAngle_z;

	// Aktualna temperatura otoczenia
	char sTemperature;
	// Maska statusu uzadzenia
	char status;
} gyro_data;

/**
 * Metoda pobierajaca dane o obrocie zarejestrowane
 * przy pomocy zyroskopu
 *
 * \return struktura z daymi wyjsciowymi
 */
gyro_data L3G4200D_GetData();
/**
 * Metoda odczytujaca dane o predkosci katowej
 * z bufora zyroskopu i rejsetrujaca je w lokalnej pamieci
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void L3G4200D_ReadData(Twid *twid);
/**
 * Metoda konfigurujaca zyroskop
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void L3G4200D_PowerOn(Twid *twid);
/**
 * Metoda uruchamiajaca strumieniowy odczyt
 * danych z zyroskopu
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void L3G4200D_StreamMode(Twid *twid);
/**
 * Metoda resetujaca dane w zyroskopie oraz
 * w lokalnej pamieci
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void L3G4200D_Reset(Twid *twid);
/**
 * Metoda odpowiedzialna za kalibracje modulu
 * zyroskopu
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void L3G4200D_Calibrate(Twid *twid);

#endif /* MMC212XM_H_ */
