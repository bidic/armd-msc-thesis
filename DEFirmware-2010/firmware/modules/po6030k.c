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
char mem[MAX_BUFF_IDX * BUFFER_SIZE];

void PO6030K_Initalize() {
	memset(mem, 0x00, MAX_BUFF_IDX * BUFFER_SIZE);

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
		unsigned int reg[] = { 0x03, 0x91, 0x03, 0x30 };
		unsigned char comm[] = { 0x00, 0x50, 0x01, 0x7f };

	// test img
	//	unsigned int reg[] = { 0x03, 0x91, 0x03, 0x40, 0x30 };
	//	unsigned char comm[] = { 0x00, 0xB0, 0x01, 0x1a, 0x7f };

	int twiit = 0;
	TRACE_DEBUG("\r\nSetting camera registers... \r\n");
	for (twiit = 0; twiit < 4; twiit++) {
		TWID_Write(twid, PO6030K_DEVICE_ID, reg[twiit], 1, comm + twiit, 1, 0);
		waitms(200);
	}
	TRACE_DEBUG("\r\nCamera registers has been set. \r\n");
}

/// Write PMC register
#define WRITE_PMC2(pPmc, regName, value) pPmc->regName = (value)

/// Write SPI register
#define WRITE_SPI2(pSpi, regName, value) pSpi->regName = (value)

//#define SPI_MR2          (AT91_CAST(AT91_REG *) 	0x00000004)
unsigned char command[4];
__inline unsigned char AT45DB321D_SendCommand(At45 *pAt45, unsigned char cmd,
		unsigned char cmdSize, unsigned char *pData, unsigned int dataSize,
		unsigned int address) {

	// Compute command pattern

	command[0] = cmd;

	// Add address bytes if necessary
	if (cmdSize > 1) {
		command[1] = ((address & 0x00FF0000) >> 16);
		command[2] = ((address & 0x0000FF00) >> 8);
		command[3] = ((address & 0x000000FF) >> 0);
	}

	unsigned int spiMr;

	// Enable the SPI clock
	WRITE_PMC2(AT91C_BASE_PMC, PMC_PCER, (1 << pAt45->pSpid->spiId));

	// Disable transmitter and receiver
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);

	// Write to the MR register
	spiMr = pAt45->pSpid->pSpiHw->SPI_MR;
	spiMr |= AT91C_SPI_PCS;
	spiMr &= ~((1 << 1) << 16);
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_MR, spiMr);

	// Initialize the two SPI PDC buffer
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_RPR, (int) command);
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_RCR, cmdSize);
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_TPR, (int) command);
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_TCR, cmdSize);

	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_RNPR, (int) pData);
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_RNCR, dataSize);
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_TNPR, (int) pData);
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_TNCR, dataSize);

	// Enable transmitter and receiver
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTEN | AT91C_PDC_TXTEN);

	// Enable buffer complete interrupt
	WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IER, AT91C_SPI_RXBUFF);

//			if (SPID_SendCommand(pAt45->pSpid, pCommand)) {
//				return AT45_ERROR_SPI;
//			}

	return 0;
}

void PO6030K_TakePicture() {
	TRACE_DEBUG("\r\nTaking picture in progress, please stand by... \r\n");
	At45 *pAt45 = AT45DB321D_GetPointer();
	//zrob 2 razy zdjecie kamera zeby AE przystosowalo sie do swiatla
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA1);
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, DIODA1);

	unsigned int iter = 0;
	for (iter = 0; iter < 10; iter++) {
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
	register int bufSel = 0;
	register int curr_idx = 0;
	register int last_idx_written = 0;
	register int pix = 0;

	while (!GET_VSYNC) {
		while (!GET_VSYNC)
			; //czekaj gdy VSYNC == 0
	}

	while (GET_VSYNC) //podczas gdy VSYNC == 1
	{
		while (!GET_HSYNC && GET_VSYNC)
			;

		while (GET_HSYNC) //podczas gdy HSYNC == 1
		{
			while (!GET_PSYNC) {
				while (!GET_PSYNC)
					;
				//czekaj gdy PCLK == 0
			}

			CamR(mem[curr_idx*BUFFER_SIZE + wsk++]);
			pix++;

			if (wsk & 0x200) {
				wsk = 0;

				SHIFT_BUFF_IDX(curr_idx);

				while (!(pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF))
				{
					while (GET_PSYNC) {
						while (GET_PSYNC)
							;
					}

					while (!GET_PSYNC) {
						while (!GET_PSYNC)
							;
					}

					CamR(mem[curr_idx*BUFFER_SIZE + wsk++]);
					pix++;
				}
				WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
				WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
				WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);

//				if ((pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF)) {
//
//					WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
//					WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
//					WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);
//					pAt45->pSpid->semaphore++;

					//					while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
					;

					unsigned char status = 0;
					while (!AT45_STATUS_READY(status)) {
						AT45DB321D_SendCommand(pAt45, AT45_STATUS_READ, 1,
								&status, 1, 0);


						while (!(pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF))
						{
							while (GET_PSYNC) {
								while (GET_PSYNC)
									;
							}

							while (!GET_PSYNC) {
								while (!GET_PSYNC)
									;
							}

							CamR(mem[curr_idx*BUFFER_SIZE + wsk++]);
							pix++;
						}
						WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
						WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
						WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);

					}

					AT45DB321D_SendCommand(pAt45,
							(!bufSel) == 0 ? AT45_BUF1_MEM_NOERASE
									: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++
									<< 9);

					//					ASSERT(!error, "\r\n AT45_BUF_MEM_NOERASE command failed \r\n")



					while (!(pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF))
					{
						while (GET_PSYNC) {
							while (GET_PSYNC)
								;
						}

						while (!GET_PSYNC) {
							while (!GET_PSYNC)
								;
						}

						CamR(mem[curr_idx*BUFFER_SIZE + wsk++]);
						pix++;
					}
					WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
					WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
					WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);



					AT45DB321D_SendCommand(pAt45,
							(bufSel == 0) ? AT45_BUF1_WRITE : AT45_BUF2_WRITE,
							4, mem + (last_idx_written * PAGE_SIZE), PAGE_SIZE,
							0);

					bufSel = !bufSel;
					SHIFT_BUFF_IDX(last_idx_written);

//				} else {
//					SHIFT_BUFF_IDX(curr_idx);
//				};
			}

			while (GET_PSYNC) {
				while (GET_PSYNC)
					;
			}
		}

		wsk = 0;

//		if ((pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF)) {

//			WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS
//					| AT91C_PDC_TXTDIS);
//			WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
//			WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);
//			pAt45->pSpid->semaphore++;

		while (!(pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF));
		WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
		WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
		WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);

			unsigned char status = 0;
			while (!AT45_STATUS_READY(status)) {
				AT45DB321D_SendCommand(pAt45, AT45_STATUS_READ, 1,
						&status, 1, 0);

				while (!(pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF));
				WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
				WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
				WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);

			}

			AT45DB321D_SendCommand(pAt45,
					(!bufSel) == 0 ? AT45_BUF1_MEM_NOERASE
							: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9);

			while (!(pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF));
			WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
			WRITE_PMC2(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
			WRITE_SPI2(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);

			AT45DB321D_SendCommand(pAt45, (bufSel == 0) ? AT45_BUF1_WRITE
					: AT45_BUF2_WRITE, 4, mem + (last_idx_written * PAGE_SIZE),
					512, 0);

			bufSel = !bufSel;
			SHIFT_BUFF_IDX(last_idx_written);
			SHIFT_BUFF_IDX(curr_idx);

//		} else {
//			SHIFT_BUFF_IDX(curr_idx);
//		};
	}

	TRACE_DEBUG("\r\ncurr_idx: %d, written_idx: %d, pix: %d \r\n", curr_idx, last_idx_written, pix);
//	while (!pAt45->pSpid->semaphore) {
//		while (!pAt45->pSpid->semaphore)
//			SPID_Handler(pAt45->pSpid);
//	}
//
//	while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
//		;
//	AT45DB321D_SendCommand(pAt45, !bufSel == 0 ? AT45_BUF1_MEM_NOERASE
//			: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9);
//
//	while (!pAt45->pSpid->semaphore) {
//		while (!pAt45->pSpid->semaphore)
//			SPID_Handler(pAt45->pSpid);
//	}
//
//	while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
//		;
//
//	AT45DB321D_SendCommand(pAt45, bufSel == 0 ? AT45_BUF1_MEM_NOERASE
//			: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9);
//
//	while (!pAt45->pSpid->semaphore) {
//		while (!pAt45->pSpid->semaphore)
//			SPID_Handler(pAt45->pSpid);
//	}
//
//	while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
//		;

	TRACE_DEBUG("\r\n... DONE!!!\r\n");
}
