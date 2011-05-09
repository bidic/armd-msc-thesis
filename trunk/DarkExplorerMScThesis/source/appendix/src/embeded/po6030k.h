#ifndef PO6030K_H_
#define PO6030K_H_

// Adres kamery do zapisu przez TWI
#define PO6030K_DEVICE_ID 0x6E

// Pobiera informacje tylko o 25 pinie (VSYNC)
#define GET_VSYNC (AT91C_BASE_PIOA->PIO_PDSR & 0x2000000)

// Pobiera informacje tylko o 26 pinie (HSYNC)
#define GET_HSYNC (AT91C_BASE_PIOA->PIO_PDSR & 0x4000000)

// Pobiera informacje tylko o 30 pinie (PCLK)
#define GET_PSYNC (AT91C_BASE_PIOA->PIO_PDSR & 0x40000000)

#define MAX_BUFF_IDX 3
#define SHIFT_BUFF_IDX(idx) (idx = ((idx)+1 < MAX_BUFF_IDX) ? ((idx)+1) : 0)
#define PREV_BUFF_IDX(idx) (((idx)-1 < 0) ? MAX_BUFF_IDX-1 : (idx)-1)
#define READ_CAM_DATA(data) {
	register unsigned int buff= (AT91C_BASE_PIOA->PIO_PDSR & 0x95F0000) >> 14;\
    	data = (( buff & 0x7C ) )\
    	| ((buff & 0x2000) >> 6)\
        | ((buff & 0x400) >> 10)\
        | ((buff & 0x100) >> 7);\
    }
#define IS_COMMAND_SENT(pSpid) ((pSpid)->pSpiHw->SPI_SR & 0x40)
#define RELEASE_SPI_WHEN_READY(pSpid){	while (!IS_COMMAND_SENT(pSpid)); }
#define SYNC_WAIT(psync) {	while ((psync)); }

/**
 * Metoda inicjalizujaca moduly kamery
 * wbudowanej w robota
 */
void PO6030K_Initalize();
/**
 * Metoda inicjalizujaca wlasciwosci kamery
 * odpowiezialne za rodzaj zdjecia
 *
 * \param twid Wskaznik do interfejsu TWI
 */
void PO6030K_InitRegisters(Twid *twid);
/**
 * Metoda pobierajaca zdjecie z kamery i zapisujaca
 * je w pamieci flash robota
 */
void PO6030K_TakePicture();

#endif /* PO6030K_H_ */
