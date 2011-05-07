/*
 * at45db321d.c
 *
 *  Created on: Mar 5, 2011
 *      Author: lhanusiak
 */

#include "at45db321d.h"

At45 sAt45;
Spid sSpid;
extern char mem[];

At45* AT45DB321D_GetPointer() {
	return &sAt45;
}

void AT45DB321D_Read(unsigned char *pBuffer, unsigned int size,
		unsigned int address) {
	AT45D_Read(&sAt45, pBuffer, size, address);
}

void TestCallback(unsigned char habahaba, void * hahaha) {
	ASSERT(0,"\r\test callback t. \r\n");
}

inline void AT45_WriteBuffer(At45* pAt45, short int siBuffNr,
		unsigned char* pBuff, unsigned int uiSize) {
	while (AT45_SendCommand(pAt45, (siBuffNr == 0) ? AT45_BUF1_WRITE
			: AT45_BUF2_WRITE, 4, pBuff, uiSize, 0, 0, 0)) {
		ASSERT(0, "\r\n AT45_WriteBuffer failed \r\n");
		while (!pAt45->pSpid->semaphore) {
			while (!pAt45->pSpid->semaphore)
				SPID_Handler(pAt45->pSpid);
		}
	}
}

inline unsigned char AT45_GetStatus(At45 *pAt45) {
	unsigned char status = 0;

	// Issue a status register read command

	AT45_SendCommand(pAt45, AT45_STATUS_READ, 1, &status, 1, 0, 0, 0);

	while (!pAt45->pSpid->semaphore) {
		while (!pAt45->pSpid->semaphore)
			SPID_Handler(pAt45->pSpid);
	}

	return status;
}

inline void AT45_BufferToMem(At45* pAt45, short int siBuffNr,
		unsigned int uiAddress) {
	AT45_SendCommand(pAt45, (siBuffNr == 0) ? AT45_BUF1_MEM_NOERASE
			: AT45_BUF2_MEM_NOERASE, 4, 0, 0, uiAddress, 0, 0);

	// Wait until the command is sent
	while (AT45_IsBusy(pAt45)) {
		// Wait for transfer to finish
		while (AT45_IsBusy(pAt45))
			SPID_Handler(pAt45->pSpid);
	}
}

unsigned char AT45_EraseChip(At45* pAt45) {
	TRACE_INFO("Erasing chip...");

	SANITY_CHECK(pAt45);
	unsigned char opcode = 0xC7;
	unsigned char opcodes[3] = { 0x94, 0x80, 0x9A };
	unsigned char error = AT45_SendCommand(pAt45, opcode, 1, opcodes, 3, 0, 0,
			0);

	ASSERT(!error, "-F- AT45_Erase: Could not issue command.\n\r");

	// Wait until the command is sent
	while (AT45_IsBusy(pAt45)) {
		SANITY_CHECK(pAt45);

		// Wait for transfer to finish
		while (AT45_IsBusy(pAt45))
			SPID_Handler(pAt45->pSpid);
	}

	AT45D_WaitReady(pAt45);
	TRACE_INFO("Chip erased successfully.");
	return error;
}

unsigned char AT45_FastWrite(At45* pAt45, short int siBuffNr,
		unsigned char* pBuff, unsigned int uiSize, unsigned int uiAddress) {
	unsigned char cmd1;
	unsigned char cmd2;

	if (siBuffNr == 0) {
		cmd1 = AT45_BUF1_WRITE;
		cmd2 = AT45_BUF1_MEM_NOERASE;

	} else {
		cmd1 = AT45_BUF2_WRITE;
		cmd2 = AT45_BUF2_MEM_NOERASE;
	}

	unsigned char error = AT45_SendCommand(pAt45, cmd1, 4, pBuff, uiSize, 0, 0,
			0);
	ASSERT(!error, "-F- AT45_Write: Could not issue command.\n\r");

	// Wait until the command is sent
	while (AT45_IsBusy(pAt45)) {
		SANITY_CHECK(pAt45);

		// Wait for transfer to finish
		while (AT45_IsBusy(pAt45))
			SPID_Handler(pAt45->pSpid);
	}

	AT45D_WaitReady(pAt45);

	error = AT45_SendCommand(pAt45, cmd2, 4, pBuff, uiSize, uiAddress, 0, 0);
	ASSERT(!error, "-F- AT45_Write: Could not issue command.\n\r");

	// Wait until the command is sent
	while (AT45_IsBusy(pAt45)) {
		SANITY_CHECK(pAt45);

		// Wait for transfer to finish
		while (AT45_IsBusy(pAt45))
			SPID_Handler(pAt45->pSpid);
	}

	return error;
}

__inline void AT45_WaitReady(At45 *pAt45) {
	register unsigned char ready = 0;

	// Poll device until it is ready
	while (!ready) {
		ready = AT45_STATUS_READY(AT45D_GetStatus(pAt45));
	}
}

unsigned char AT45_EraseBlock(At45* pAt45, unsigned int uiAddress) {
	SANITY_CHECK(pAt45);
	unsigned char opcode = AT45_BLOCK_ERASE;
	unsigned char error = AT45_SendCommand(pAt45, opcode, 4, 0, 0, uiAddress,
			0, 0);

	ASSERT(!error, "-F- AT45_Erase: Could not issue command.\n\r");

	// Wait until the command is sent
	while (AT45_IsBusy(pAt45)) {
		SANITY_CHECK(pAt45);

		// Wait for transfer to finish
		while (AT45_IsBusy(pAt45))
			SPID_Handler(pAt45->pSpid);
	}

	AT45D_WaitReady(pAt45);

	return error;
}

void SPI_It_Hanlder() {
	SPID_Handler(sAt45.pSpid);
}

void AT45DB321D_ClearChip() {
	TRACE_DEBUG("  in AT45DV321D_ClearChip() \r\n");

	TRACE_INFO("Erasing...");
	unsigned int i = 0;
	for (i = 0; i < BLOCKS_AMOUNT + 1; i++) {
		AT45_EraseBlock(&sAt45, i * BLOCK_SIZE);
	}

	TRACE_DEBUG("  out AT45DV321D_ClearChip() \r\n");
}

void AT45DB321D_Initalize() {
	TRACE_DEBUG("  in AT45DB321D_Initalize() \r\n");
	SPID_Configure(&sSpid, AT91C_BASE_SPI, AT91C_ID_SPI);

	//	AIC_ConfigureIT(AT91C_ID_SPI, AT91C_AIC_PRIOR_LOWEST, SPI_It_Hanlder);
	SPID_ConfigureCS(&sSpid, 1, AT45_CSR(BOARD_MCK, 20000000));
	AIC_DisableIT(AT91C_ID_SPI);

	AT45_Configure(&sAt45, &sSpid, 1);
	unsigned char status = AT45D_GetStatus(&sAt45);
	AT45_FindDevice(&sAt45, status);

	TRACE_INFO("Connected to device %s", AT45_FindDevice(&sAt45, status)->name);

	TRACE_DEBUG("  out AT45DB321D_Initalize() \r\n");
}

/// Write PMC register
#define WRITE_PMC1(pPmc, regName, value) pPmc->regName = (value)

/// Write SPI register
#define WRITE_SPI1(pSpi, regName, value) pSpi->regName = (value)

void AT45DB321D_SelfTest() {
	int i = 0;
	int j = 0;

	memset(mem, 0x0A, MAX_BUFF_IDX2 * PAGE_SIZE);

	unsigned char rdbuff[PAGE_SIZE * 3];
	memset(rdbuff, 0x00, PAGE_SIZE * 3);
	char bOK = 1;

	AT45DB321D_ClearChip();

	register int wsk = 0;
	register int row = 0;
	register int bufSel = 0;
	register int curr_idx = 0;
	register int last_idx_written = 0;

	At45 *pAt45 = AT45DB321D_GetPointer();

	TRACE_DEBUG("\n Writing test pattern \r\n");

	for (i = 0; i < 480; i++) {
		for (j = 0; j < 1280; j++) {
			mem[curr_idx * PAGE_SIZE + wsk++] = j % 256;

			if (wsk & 0x200) {
				wsk = 0;
				waitms(2);
				if ((pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF)) {

					WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
					WRITE_PMC1(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
					WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);
					pAt45->pSpid->semaphore++;

					while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
						;

					unsigned char error = AT45_SendCommand(pAt45, (!bufSel)
							== 0 ? AT45_BUF1_MEM_NOERASE
							: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9, 0, 0);

					ASSERT(!error, "\r\n AT45_BUF_MEM_NOERASE command failed \r\n")

					while (!pAt45->pSpid->semaphore) {
						while (!pAt45->pSpid->semaphore)
							SPID_Handler(pAt45->pSpid);
					}

					AT45_SendCommand(pAt45, (bufSel == 0) ? AT45_BUF1_WRITE
							: AT45_BUF2_WRITE, 4, mem + (last_idx_written
							* PAGE_SIZE), PAGE_SIZE, 0, 0, 0);

					bufSel = !bufSel;
					SHIFT_BUFF_IDX2(last_idx_written);
					SHIFT_BUFF_IDX2(curr_idx);

				} else {
					TRACE_DEBUG("\n else %d %d\r\n", curr_idx, last_idx_written);
					SHIFT_BUFF_IDX2(curr_idx);
					ASSERT(curr_idx != last_idx_written, "\r\ni: %d, curr: %d, last: %d\r\n", i, curr_idx, last_idx_written);
				};
			}
		}

		wsk = 0;
		waitms(2);
		if ((pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF)) {

			WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
			WRITE_PMC1(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
			WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);
			pAt45->pSpid->semaphore++;

			while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
				;

			unsigned char error = AT45_SendCommand(pAt45,
					(!bufSel) == 0 ? AT45_BUF1_MEM_NOERASE
							: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9, 0, 0);

			ASSERT(!error, "\r\n AT45_BUF_MEM_NOERASE command failed \r\n")

			while (!pAt45->pSpid->semaphore) {
				while (!pAt45->pSpid->semaphore)
					SPID_Handler(pAt45->pSpid);
			}

			AT45_SendCommand(pAt45, (bufSel == 0) ? AT45_BUF1_WRITE
					: AT45_BUF2_WRITE, 4, mem + (last_idx_written * PAGE_SIZE),
					512, 0, 0, 0);

			bufSel = !bufSel;
			//			if (last_idx_written != curr_idx) {
			SHIFT_BUFF_IDX2(last_idx_written);
			SHIFT_BUFF_IDX2(curr_idx);
			//			} else {
			//				SHIFT_BUFF_IDX2(curr_idx);
			//			}

		} else {
			TRACE_DEBUG("\n else %d %d\r\n", curr_idx, last_idx_written);
			SHIFT_BUFF_IDX2(curr_idx);
			ASSERT(curr_idx != last_idx_written, "\r\ni: %d, curr: %d, last: %d\r\n", i, curr_idx, last_idx_written);
		};
	}

	while (curr_idx != last_idx_written) {
		if ((pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF)) {
			WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
			WRITE_PMC1(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
			WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);
			pAt45->pSpid->semaphore++;

			while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
				;
			unsigned char error = AT45_SendCommand(pAt45,
					(!bufSel) == 0 ? AT45_BUF1_MEM_NOERASE
							: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row++ << 9, 0, 0);

			ASSERT(!error, "\r\n AT45_BUF_MEM_NOERASE command failed \r\n")

			while (!pAt45->pSpid->semaphore) {
				while (!pAt45->pSpid->semaphore)
					SPID_Handler(pAt45->pSpid);
			}

			AT45_SendCommand(pAt45, (bufSel == 0) ? AT45_BUF1_WRITE
					: AT45_BUF2_WRITE, 4, mem + (last_idx_written * PAGE_SIZE),
					512, 0, 0, 0);

			bufSel = !bufSel;

			SHIFT_BUFF_IDX2(last_idx_written);
		}
	}

	//	for (i = 0; i < PAGES_AMOUNT + 1 && bOK; i++) {
	//		memset(wrbuff, row % 256, PAGE_SIZE);
	//
	//		memcpy(mem + (curr_idx * PAGE_SIZE), wrbuff, PAGE_SIZE);
	//
	//		//
	//		waitms(2);
	//
	//		if ((pAt45->pSpid->pSpiHw->SPI_SR & AT91C_SPI_RXBUFF)) {
	//
	//			//			TRACE_DEBUG("\n not else %d %d\r\n", curr_idx, last_idx_written);
	//			// Disable transmitter and receiver
	//			WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_PTCR, AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
	//
	//			// Disabl1e the SPI clock
	//			WRITE_PMC1(AT91C_BASE_PMC, PMC_PCDR, (1 << pAt45->pSpid->spiId));
	//
	//			// Disable buffer complete interrupt
	//			WRITE_SPI1(pAt45->pSpid->pSpiHw, SPI_IDR, AT91C_SPI_RXBUFF);
	//
	//			// Release the dataflash semaphore
	//			pAt45->pSpid->semaphore++;
	//
	//			while (!AT45_STATUS_READY(AT45_GetStatus(pAt45)))
	//				;
	//
	//			unsigned char error = AT45_SendCommand(pAt45,
	//					(!bufSel) == 0 ? AT45_BUF1_MEM_NOERASE
	//							: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row << 9, 0, 0);
	//
	//			ASSERT(!error, "\r\n AT45_BUF_MEM_NOERASE command failed \r\n")
	//
	//			while (!pAt45->pSpid->semaphore) {
	//				while (!pAt45->pSpid->semaphore)
	//					SPID_Handler(pAt45->pSpid);
	//			}
	//
	//			AT45_SendCommand(pAt45, (bufSel == 0) ? AT45_BUF1_WRITE
	//					: AT45_BUF2_WRITE, 4, mem + (last_idx_written * PAGE_SIZE),
	//					PAGE_SIZE, 0, 0, 0);
	//
	//			bufSel = !bufSel;
	//			if (last_idx_written != curr_idx) {
	//				SHIFT_BUFF_IDX2(last_idx_written);
	//				SHIFT_BUFF_IDX2(curr_idx);
	//				row++;
	//			} else {
	//				SHIFT_BUFF_IDX2(curr_idx);
	//			}
	//
	//		} else {
	//			TRACE_DEBUG("\n else %d %d\r\n", curr_idx, last_idx_written);
	//			SHIFT_BUFF_IDX2(curr_idx);
	//			ASSERT(curr_idx != last_idx_written, "\r\ni: %d, curr: %d, last: %d\r\n", i, curr_idx, last_idx_written);
	//		};
	//	}

	while (!pAt45->pSpid->semaphore) {
		while (!pAt45->pSpid->semaphore)
			SPID_Handler(pAt45->pSpid);
	}

	AT45D_WaitReady(&sAt45);

	TRACE_DEBUG("\n Written pattern check %d %d\r\n", curr_idx, last_idx_written);

	for (i = 0; i < 480; i++) {
		memset(rdbuff, 0x00, PAGE_SIZE * 3);
		AT45D_Read(&sAt45, rdbuff, PAGE_SIZE * 3, PAGE_SIZE * 3 * i + 512);

		unsigned int c1 = 0;
		unsigned int c2 = 0;
		unsigned int c3 = 0;
		//		int d = -1;

		for (j = 0; j < PAGE_SIZE * 3; j++) {

			//			if (!(j % PAGE_SIZE))
			//				d++;

			//			TRACE_DEBUG("\r\n written: %d, read: %d, i: %d, j: %d\r\n", (d), rdbuff[j], i, j);

			if (rdbuff[j] != (j % 256)) {
				if (j < 512)
					c1++;
				else if (j >= 512 && j < 1024)
					c2++;
				else if (j >= 1024)
					c3++;
			}
		}

		TRACE_DEBUG("row: %d, c1: %d, c2: %d, c3: %d\r\n", i, c1, c2, c3);
	}

	//	for (i = 1; i < (PAGES_AMOUNT) && bOK; i++) {
	//		memset(wrbuff, (i - 2) % 256, PAGE_SIZE);
	//		memset(rdbuff, 0x00, PAGE_SIZE);
	//
	//		AT45D_Read(&sAt45, rdbuff, PAGE_SIZE, PAGE_SIZE * i);
	//
	//		for (j = 0; j < PAGE_SIZE; j++) {
	//			if (!bOK || wrbuff[j] != rdbuff[j]) {
	//				bOK = 1;
	//				TRACE_DEBUG("\r\n written: %d, read: %d, i: %d, j: %d\r\n", wrbuff[j], rdbuff[j], i, j);
	//				break;
	//			}
	//		}
	//	}

	if (bOK) {
		TRACE_INFO("Test completed successfully");
	} else {
		ASSERT(0, "Pattern mismatch found");
	}

	//	AT45_EraseChip(&sAt45);
	AT45DB321D_ClearChip();
}
