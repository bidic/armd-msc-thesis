/*
 * po6030k.c
 *
 *  Created on: Mar 5, 2011
 *      Author: lhanusiak
 */
#include "po6030k.h"
#include "de_pio.h"

#include "peripherals.h"

//pamiec obrazu
char mem[MAX_BUFF_IDX * PAGE_SIZE];

void PO6030K_Initalize() {
	memset(mem, 0x00, MAX_BUFF_IDX * PAGE_SIZE);

	TRACE_DEBUG("  in  PO6030K_Initalize()\r\n");
	AT91F_PMC_EnablePCK(AT91C_BASE_PMC, 1, AT91C_PMC_CSS_MAIN_CLK
			| AT91C_PMC_PRES_CLK_64); //MAIN_CLK podzielony przez 64
	TRACE_DEBUG("  out  PO6030K_Initalize()\r\n");
}

void PO6030K_InitRegisters(Twid *twid) {
	//0x03 set register to A,B or C, 0x40 turn on test image(0x1a), 0x91 clock divider - 0xB0 == 101 == 1/8, 0x90 == 100 == 1/4, 0x70 == 011 == 1/3, 0x70 == 010 == 1/2
	// with color correction
	//	unsigned int reg[] = { 0x03, 0x91 };
	//	unsigned char comm[] = { 0x00, 0x50 };

	// without color correction
//	unsigned int reg[] = { 0x03, 0x91, 0x03, 0x30 };
//	unsigned char comm[] = { 0x00, 0x50, 0x01, 0x7f };

	// without color correction + auto filcker
	unsigned int reg[] = { 0x03, 0x91, 0x03, 0x11 };
	unsigned char comm[] = { 0x00, 0x50, 0x01, 0x02 };

	// test img
	//	unsigned int reg[] = { 0x03, 0x91, 0x03, 0x40, 0x30 };
	//	unsigned char comm[] = { 0x00, 0xB0, 0x01, 0x1a, 0x7f };

	int twiit = 0;
	TRACE_DEBUG("\r\nSetting camera registers... \r\n");
	for (twiit = 0; twiit < 2; twiit++) {
		TWID_Write(twid, PO6030K_DEVICE_ID, reg[twiit], 1, comm + twiit, 1, 0);
		waitms(200);
	}
	TRACE_DEBUG("\r\nCamera registers has been set. \r\n");
}

unsigned char command[4];
__inline void AT45DB321D_SendCommand(At45 *pAt45, unsigned char cmd,
		unsigned char cmdSize, unsigned char *pData, unsigned int dataSize,
		unsigned int address) {
	// Compute command pattern
	command[0] = cmd;

	// Add address bytes if necessary
	if (cmdSize & 0xfe) {
		command[1] = ((address & 0x00FF0000) >> 16);
		command[2] = ((address & 0x0000FF00) >> 8);
		command[3] = ((address & 0x000000FF) >> 0);
	}

	// Enable the SPI clock
	AT91C_BASE_PMC->PMC_PCER = (1 << pAt45->pSpid->spiId);

	// Disable transmitter and receiver
	pAt45->pSpid->pSpiHw->SPI_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;

	pAt45->pSpid->pSpiHw->SPI_MR = ((pAt45->pSpid->pSpiHw->SPI_MR
			| AT91C_SPI_PCS) & (~(1 << 17)));

	// Initialize the two SPI PDC buffer
	pAt45->pSpid->pSpiHw->SPI_RPR = (int) command;
	pAt45->pSpid->pSpiHw->SPI_RCR = cmdSize;
	pAt45->pSpid->pSpiHw->SPI_TPR = (int) command;
	pAt45->pSpid->pSpiHw->SPI_TCR = cmdSize;
	pAt45->pSpid->pSpiHw->SPI_RNPR = (int) pData;
	pAt45->pSpid->pSpiHw->SPI_RNCR = dataSize;
	pAt45->pSpid->pSpiHw->SPI_TNPR = (int) pData;
	pAt45->pSpid->pSpiHw->SPI_TNCR = dataSize;

	// Enable transmitter and receiver
	pAt45->pSpid->pSpiHw->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
}

void PO6030K_TakePicture() {
	TRACE_DEBUG("\r\nTaking picture in progress, please stand by... \r\n");
	At45 *pAt45 = AT45DB321D_GetPointer();
	//zrob 2 razy zdjecie kamera zeby AE przystosowalo sie do swiatla
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA1);
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, DIODA1);

	unsigned int iter = 0;
	for (iter = 0; iter < 5; iter++) {
		AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA2);
		while (!GET_VSYNC)
			;
		AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, DIODA2);
		while (GET_VSYNC)
			;
	}

	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA1);
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA2);

	register int row = 0;
	register int wsk = 0;
	register int buf = 0;
	register int idx = 0;
	register int pix = 0;

	SYNC_WAIT(!GET_VSYNC);

	while (GET_VSYNC) //podczas gdy VSYNC == 1
	{
		while (!GET_HSYNC && GET_VSYNC)
			;

		while (GET_HSYNC) //podczas gdy HSYNC == 1
		{
			SYNC_WAIT(!GET_PSYNC);

			READ_CAM_DATA(mem[idx*PAGE_SIZE + wsk++]);
			pix++;

			if (wsk & 0x200) {
				wsk = 0;

				SHIFT_BUFF_IDX(idx);

				while (!IS_COMMAND_SENT(pAt45)) {
					SYNC_WAIT(GET_PSYNC);
					SYNC_WAIT(!GET_PSYNC);

					READ_CAM_DATA(mem[idx*PAGE_SIZE + wsk++]);
					pix++;
				}

				RELEASE_SPI(pAt45);

				unsigned char status = 0;
				while (!AT45_STATUS_READY(status)) {
					AT45DB321D_SendCommand(pAt45, AT45_STATUS_READ, 1, &status,
							1, 0);

					while (!IS_COMMAND_SENT(pAt45)) {
						SYNC_WAIT(GET_PSYNC);
						SYNC_WAIT(!GET_PSYNC);

						READ_CAM_DATA(mem[idx*PAGE_SIZE + wsk++]);
						pix++;
					}

					RELEASE_SPI(pAt45);
				}

				AT45DB321D_SendCommand(pAt45, buf ? AT45_BUF1_MEM_NOERASE
						: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9);

				while (!IS_COMMAND_SENT(pAt45)) {
					SYNC_WAIT(GET_PSYNC);
					SYNC_WAIT(!GET_PSYNC);

					READ_CAM_DATA(mem[idx*PAGE_SIZE + wsk++]);
					pix++;
				}

				RELEASE_SPI(pAt45);

				AT45DB321D_SendCommand(pAt45, !buf ? AT45_BUF1_WRITE
						: AT45_BUF2_WRITE, 4, mem + (PREV_BUFF_IDX(idx)
						* PAGE_SIZE), PAGE_SIZE, 0);

				buf = !buf;
			}

			SYNC_WAIT(GET_PSYNC);
		}

		wsk = 0;
		RELEASE_SPI_WHEN_READY(pAt45);

		unsigned char status = 0;
		while (!AT45_STATUS_READY(status)) {
			AT45DB321D_SendCommand(pAt45, AT45_STATUS_READ, 1, &status, 1, 0);
			RELEASE_SPI_WHEN_READY(pAt45);
		}

		AT45DB321D_SendCommand(pAt45, buf ? AT45_BUF1_MEM_NOERASE
				: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9);
		RELEASE_SPI_WHEN_READY(pAt45);

		AT45DB321D_SendCommand(pAt45, !buf ? AT45_BUF1_WRITE : AT45_BUF2_WRITE,
				4, mem + (idx * PAGE_SIZE), PAGE_SIZE, 0);

		buf = !buf;
		SHIFT_BUFF_IDX(idx);
	}

	TRACE_DEBUG("\r\n... acquisition completed. (Pix: %d) \r\n", pix);
}
