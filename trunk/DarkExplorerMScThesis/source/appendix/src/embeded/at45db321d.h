#ifndef AT45DB321D_H_
#define AT45DB321D_H_

#define PAGES_AMOUNT 1500
#define BLOCKS_AMOUNT (PAGES_AMOUNT / 8)
#define PAGE_SIZE 512
#define BLOCK_SIZE (8 * PAGE_SIZE)

#define AT45DB321_MAX_BUFF_IDX 3
#define AT45DB321_SHIFT_BUFF_IDX(idx)
		( idx = ((idx)+1 < AT45DB321_MAX_BUFF_IDX) ? ((idx)+1) : 0)

/**
 * Metoda inicjalizujaca konfiguracje
 * wbudowanej pamieci flash
 */
void AT45DB321D_Initalize();
/**
 * Metoda usuwajaca wszystkie dane z pamieci
 */
void AT45DB321D_ClearChip();
/**
 * Metoda przeprowadzajca test poprawnosci
 * zapisu i odczytu z pamieci
 */
void AT45DB321D_SelfTest();
/**
 * Metoda odczytujaca dane zapisane w pamieci
 */
void AT45DB321D_Read(unsigned char *pBuffer,
		unsigned int size, unsigned int address);
/**
 * Metoda zwaracajaca wskaznik do pamieci flash
 */
At45* AT45DB321D_GetPointer();
/**
 * Metoda pobierajaca dane o aktualnym statusie pamieci
 */
inline unsigned char AT45_GetStatus(At45 *pAt45);
/**
 * Wyslanie komendy zapisu do bufora pamieci
 * przy uzyciu DMA, poprzez interfejs SPI
 */
inline void AT45_WriteBuffer(At45* pAt45, short int siBuffNr,
			unsigned char* pBuff, unsigned int uiSize);

#endif /* AT45DB321D_H_ */
