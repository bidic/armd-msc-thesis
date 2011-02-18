/**
 * Projekt    : Dark Explorer firmware
 * Plik       : main.c
 * Zawartosc  : Inicjalizacje pocz�tkowe, obs�uga przerwa� systemowych,
 : interpretacja komend, p�tla g��wna
 * Autor      : Pawe� Kmak
 * Data       : 1.11.2009
 **/

// Include
#include <de_board.h>
#include <utils.h>
#include <rozpoznawanie.h>

#include <math.h>

#include <de_pio.h>
#include <pio/pio.h>
#include <aic/aic.h>
#include <pmc/pmc.h>
#include <pit/pit.h>
#include <dbgu/dbgu.h>
#include <twi/twi.h>
#include <utility/math.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <drivers/async/async.h>
#include <drivers/twi/twid.h>
#include <spi-flash/at45d.h>

#include <string.h>

#include "modules/sharp_gp2d12.h"
#include "modules/mmc212xm.h"

#include "algorithms/obstacle_avoidance.h"

//adres kamery do zapisu przez TWI
#define PO6030K_DEVICE_ID 0x6E

//flaga dla timera PIT
int PitState = 0;

/// TWI clock frequency in Hz.
#define TWCK            100000

/// Pio pins to configure.
static const Pin pins[] = { PINS_TWI };

/// TWI driver instance.
Twid twid;

//Ustawienie polozenia serwa
//1000 - 0, 1500-45, 2000 - 90
//2850 - wieza zlozona
//1750 - postawiona pionowo
//900 - maksymalny wyhyl do przodu
volatile int SerwoAngle = 1750;
volatile int SerwoAngleBuffer = 1750;

//zmienne do sterowania transmisj� UART
char RX_Buffer[2]; //odebrane dane, 0-komenda, 1-dana
int RX_Counter = 0; //wskazuje czy byla odebrana komenda
int timeout; //timeout dla komendy

//zmienne konfiguracyjne modulu bluetooth
char bluetooth[] = "ATI?\r";

//Rozmiar ramki do odebrania
volatile char FrameSizeToGet = 0;

//Wlacznik trybu autonomicznego
char AutoMode = 0, AutoPreview = 0;

//pamiec obrazu
char mem[62000];

//tryb auto
//Stos dla SmithFill - mem od 16000
int StackIndex = 16000;
char MaxIndex = 0;

float wzorzec1 = 0, wzorzec2 = 0, wzorzec3 = 0, wzorzec4 = 0, wzorzec5 = 0,
		wzorzec6 = 0, wzorzec7 = 0, wzorzec8 = 0, wzorzec9 = 0;
int xwzorzec = 0, ywzorzec = 0, WzorzecCount = 0, RozpoznanyX, RozpoznanyY;

char SilnikiEnable = 0;

volatile int xmin, xmax, ymin, ymax;

////////////////////////////////////////////////////////////////////////////////
// Przerwanie od timera PIT
////////////////////////////////////////////////////////////////////////////////
__ramfunc void SYSTEM_INTERRUPT_irq_handler(void) {
	//impuls od timera PIT pojawia si� co 18-20ms

	unsigned int dummy;

	//odczyt rejestru PIVR (powoduje wyzerowanie flagi ��dania przerwania)
	dummy = AT91C_BASE_PITC->PITC_PIVR;

	//obliczenie pozycji serwa
	if (SerwoAngleBuffer > SerwoAngle) {
		SerwoAngle += 5;
	}
	if (SerwoAngleBuffer < SerwoAngle) {
		SerwoAngle -= 5;
	}

	//////////// STEROWANIE SERWEM ///////////////////////////////////////////////
	if (PitState == 0) {
		//zmiana interwa�u na czas 1-2 ms
		//do wygenerowania impulsu dla serwa
		//dlugosc impulsu = wychylenie serwa
		PIT_ChangeInterval(SerwoAngle);
		PitState = 1;

		//wystawnienie jedynki
		AT91F_PIO_SetOutput(AT91C_BASE_PIOA, PIO_PA0);
	} else {
		//zmiana interwa�u na czas 18 ms
		//do wygenerowania odst�pu mi�dzy impulsami
		PIT_ChangeInterval(18000); //18ms
		PitState = 0;

		//wystawnienie zera
		AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA0);

		//timeout ok 200ms - interpretacja komend
		++timeout;
		if (timeout >= 10) {
			timeout = 10;
			RX_Counter = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////////
}

volatile unsigned int data_ready = 0;
volatile unsigned int ls_data = 0;
volatile unsigned int rs_data = 0;

void onLSDataReady(unsigned int output) {
	ls_data = output;
	data_ready = 1;
}

void onRSDataReady(unsigned int output) {
	rs_data = output;
	data_ready = 1;
}

void obstacle_avoid_TestCase() {
	while (1) {
		data_ready = 0;
		GP2D12_MeasureDistance(ADC_CHANNEL_5, onRSDataReady);
		while (!data_ready) {
			//			printf(" --- wait %d ---\r\n", ADC_GetStatus(AT91C_BASE_ADC));
		}

		data_ready = 0;
		GP2D12_MeasureDistance(ADC_CHANNEL_6, onLSDataReady);
		while (!data_ready) {
			//			printf(" --- wait %d ---\r\n", ADC_GetStatus(AT91C_BASE_ADC));
		}

		use_default_configuration();

		unsigned int level_mask = create_level_mask(rs_data, ls_data);

		OAA_OUTPUT output = avoid_obstacles(level_mask);

		//		printf("speed: %d", output.speed_right);
		PWM_Set(0, output.speed_right);
		Kierunek(1, output.gear_left);
		Kierunek(2, output.gear_right);

		waitms(1000);

		//		delay(1000000);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Przerwanie od kanalu odbiorczego UART0
////////////////////////////////////////////////////////////////////////////////
__ramfunc void UART0_DMA_irq_handler(void) {
	RX_Buffer[RX_Counter] = USART_Read(AT91C_BASE_US0, 10);//UART0_Read();
	//USART_Write(AT91C_BASE_US0,RX_Buffer[RX_Counter],10);
	//USART_WriteBuffer(AT91C_BASE_US0, "\n\r", 2);
	RX_Counter++;
	timeout = 0;
	if (RX_Counter == 2) {
		RX_Counter = 0;
	} //2 bajty na ramke

	//Dekodowanie komend
	if (RX_Counter == 0) //po odebraniu komendy i danych
	{
		//Sterowanie serwem
		if (RX_Buffer[0] == 's') {
			SerwoAngleBuffer = 1065 + ((255 - RX_Buffer[1]) * 7);
		}

		//Sterowanie kanalem pwm 0, 1
		if (RX_Buffer[0] == 'a') {
			//			printf("PWM01 - RX_BUffer[1] = %d", RX_Buffer[1]);
			//			PWM_Set(0, RX_Buffer[1]);
			//			PWM_Set(1, RX_Buffer[1]);

			FrameSizeToGet = 6;
		}

		//Sterowanie kanalem pwm 2, 3
		if (RX_Buffer[0] == 'b') {
			//			printf("PWM23 - RX_BUffer[1] = %d", RX_Buffer[1]);
			//			PWM_Set(2, RX_Buffer[1]);
			//			PWM_Set(3, RX_Buffer[1]);
			FrameSizeToGet = 6;
		}

		//silniki 1-2 kierunek (lewe)
		if (RX_Buffer[0] == 'l') {
			Kierunek(1, RX_Buffer[1]);
		}

		//silniki 3-4 kierunek (prawe)
		if (RX_Buffer[0] == 'r') {
			Kierunek(2, RX_Buffer[1]);
		}

		//pomiar napiecia beterii
		if (RX_Buffer[0] == 't') {
			//			ADC_StartConversion(AT91C_ADC_CH4);
			//			RX_Buffer[0] = ADC_Read_2bit(AT91C_ADC_CH4); //2 starsze bity
			//			RX_Buffer[1] = ADC_Read_8bit(AT91C_ADC_CH4); //mlodszy bajt
			//			UART0_DMA_Write(&RX_Buffer[0], 2);
		}

		//wyslanie klatki obrazu 320 x 200
		if (RX_Buffer[0] == 'f') {
			if (RX_Buffer[1] == 0) {
				FrameSizeToGet = 2;
			}

			if (RX_Buffer[1] > 0 && RX_Buffer[1] < 20) {
				UART0_DMA_Write(&mem[3200 * RX_Buffer[1]], 3200);
			}
		}

		//wyslanie klatki obrazu 160 x 100
		if (RX_Buffer[0] == 'p') {
			if (RX_Buffer[1] == 0) {
				FrameSizeToGet = 1;
			}

			if (RX_Buffer[1] > 0 && RX_Buffer[1] < 5) {
				UART0_DMA_Write(&mem[3200 * RX_Buffer[1]], 3200);
			}
		}

		//wyslanie klatki obrazu 160 x 100 kolorowego
		if (RX_Buffer[0] == 'x') {
			if (RX_Buffer[1] == 0) {
				FrameSizeToGet = 3;
			}

			if (RX_Buffer[1] > 0 && RX_Buffer[1] < 10) {
				UART0_DMA_Write(&mem[3200 * RX_Buffer[1]], 3200);
			}
		}

		if (RX_Buffer[0] == 'z') {
			if (RX_Buffer[1] == 0) {
				FrameSizeToGet = 5;
			}
		}

		//tryb autonomiczny komenda i
		if (RX_Buffer[0] == 'i') {
			if (RX_Buffer[1] == 10) {
				AutoMode = 1;
			}

			if (RX_Buffer[1] == 11) {
				AutoMode = 0;
			}

			if (RX_Buffer[1] == 12) {
				AutoPreview = 1;
			}

			if (RX_Buffer[1] == 13) {
				AutoPreview = 0;
			}

			//podglad bufora obrazu - automatyczny
			if (RX_Buffer[1] < 5) {
				UART0_DMA_Write(&mem[(3200 * RX_Buffer[1]) + 48000], 3200);
			}
		}

		//tryb autonomiczny komenda j
		if (RX_Buffer[0] == 'j') {
			//podglad bufora obrazu - manualny
			if (RX_Buffer[1] < 5) {
				UART0_DMA_Write(&mem[(3200 * RX_Buffer[1])], 3200);
			}
		}

		//zapamietanie wskazanego wzorca - wspolrzedna x
		if (RX_Buffer[0] == 'm') {
			xwzorzec = RX_Buffer[1];
			xwzorzec = xwzorzec * 1.3;
			SilnikiEnable = 1;
		}

		//zapamietanie wskazanego wzorca - wspolrzedna y
		if (RX_Buffer[0] == 'n') {
			ywzorzec = RX_Buffer[1];
		}

		//dioda led
		if (RX_Buffer[0] == 'd') {
			//wylaczenie diody
			if (RX_Buffer[1] == 0) {
				AT91F_PIO_SetOutput(AT91C_BASE_PIOA, LED_POWER);
			}

			//wlaczenie diody
			if (RX_Buffer[1] == 1) {
				AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, LED_POWER);
			}
		}

		//serwo zasilanie
		if (RX_Buffer[0] == 'w') {
			//wylaczenie
			if (RX_Buffer[1] == 0) {
				AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SERWO_POWER);
			}

			//wlaczenie
			if (RX_Buffer[1] == 1) {
				AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SERWO_POWER);
			}
		}
	}
}

#define CamR(data) { register unsigned int buff= (AT91C_BASE_PIOA->PIO_PDSR & 0x95F0000) >> 14;\
                        data = (( buff & 0x7C ) )\
                               | ((buff & 0x2000) >> 6)\
                               | ((buff & 0x400) >> 10)\
                               | ((buff & 0x100) >> 7);\
                      }

__inline void AT45_WriteBuffer(At45* pAt45, short int siBuffNr,
		unsigned char* pBuff, unsigned int uiSize) {
	while (AT45_SendCommand(pAt45, (siBuffNr == 0) ? AT45_BUF1_WRITE
			: AT45_BUF2_WRITE, 4, pBuff, uiSize, 0, 0, 0)) {
		while (!pAt45->pSpid->semaphore) {
			while (!pAt45->pSpid->semaphore)
				SPID_Handler(pAt45->pSpid);
		}
	}
}

unsigned char AT45_EraseChip(At45* pAt45) {
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

__inline unsigned char AT45_GetStatus(At45 *pAt45) {
	unsigned char status;

	// Issue a status register read command
	AT45_SendCommand(pAt45, AT45_STATUS_READ, 1, &status, 1, 0, 0, 0);

	while (!pAt45->pSpid->semaphore) {
		while (!pAt45->pSpid->semaphore)
			SPID_Handler(pAt45->pSpid);
	}

	return status;
}

__inline void AT45_BufferToMem(At45* pAt45, short int siBuffNr,
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

////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////
int main(void) {
	// Inicjalizacje


		TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);
		printf("-- Dark Explorer with AT91LIB v. %s --\n\r", SOFTPACK_VERSION);
	//	memset(mem, 0x00, 39000);
		printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

	// Enable User Reset and set its minimal assertion to 960 us
	//  AT91C_BASE_RSTC->RSTC_RMR = AT91C_RSTC_URSTEN | (0x4<<8) | (unsigned int)(0xA5<<24);

	// mt: added reset enable to make the board reset-button "useful"
	//  AT91F_RSTSetMode( AT91C_BASE_RSTC , AT91C_RSTC_URST
	//  LcdInit();
	// Enable the clock of the PIOA
	PMC_EnablePeripheral(AT91C_ID_PIOA);
	//  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA );

	// W��czenie timera PIT
	//  PIT_Init(100,MCK);
	PIT_Configure(100); //pierwsze przerwanie za 100us

	// Wlaczenie PWM
	PWM_Configure();

	PIO_Configure(pins, PIO_LISTSIZE(pins));

	// Configure TWI
	// In IRQ mode: to avoid problems, the priority of the TWI IRQ must be max.
	// In polling mode: try to disable all IRQs if possible.
	// (in this example it does not matter, there is only the TWI IRQ active)
	AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TWI;
	TWI_ConfigureMaster(AT91C_BASE_TWI, TWCK, BOARD_MCK);
	TWID_Initialize(&twid, AT91C_BASE_TWI);

	//wlaczenie UART0
	//  UART0_DMA_Configure(9600); //115.2kbit/s
	//  UART0_DMA_Configure(230400); //230.4kbit/s
	UART0_DMA_Configure(460800); //460.8kbit/s

	//wlaczenie ADC
	GP2D12_InitializeADC();
	//	ADC_Configure(1000000); //1MHz


	//	PMC_DisablePeripheral(AT91C_ID_SPI);
	//	//  AT91C_BASE_PMC->PMC_PLLR = 1 | 1 << 16;
	//
	//	// KONFIGURACJA MCLK KAMERY!!!!!!!!!!!!
	AT91F_PMC_EnablePCK(AT91C_BASE_PMC, 1, AT91C_PMC_CSS_MAIN_CLK
			| AT91C_PMC_PRES_CLK_64); //MAIN_CLK podzielony przez 64
	//
	//	// KONFIGURACJA pinow SPI dla pamieci flash oraz PA21 dla MCK kamery
	AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, AT91C_PA12_MISO | AT91C_PA13_MOSI
			| AT91C_PA14_SPCK | AT91C_PA31_NPCS1, AT91C_PA21_PCK1
			| AT91C_PA11_PWM0);
	//
	//	//przygotowywanie pamieci
	//	At45 sAt45;
	//	Spid sSpid;
	//	printf("przed spid conf ");
	//	SPID_Configure(&sSpid, AT91C_BASE_SPI, AT91C_ID_SPI);
	//	printf("przed spid conf cs ");
	//	//((0x1 <<  1) | ((((((250) / 32 * ((48000000) / 1000000)) / 1000) + 1) << 24) & (0xFF << 24)) \
//       | ((((((250) * ((48000000) / 1000000)) / 1000) + 1)  << 16) & (0xFF << 16)) | ((((48000000) / (20000000)) << 8) & (0xFF <<  8)))
	//	//AT45_CSR(BOARD_MCK,20000000)
	//	//AT91C_SPI_NCPHA | ((0x2 << 8) & AT91C_SPI_SCBR)
	//
	//	SPID_ConfigureCS(&sSpid, 1, AT45_CSR(BOARD_MCK,20000000));
	//	printf("przed at45 conf ");
	//	AT45_Configure(&sAt45, &sSpid, 1);
	//	printf("przed get status ");
	//	unsigned char status = AT45D_GetStatus(&sAt45);
	//	printf("przed find dev %x", status);
	//	At45Desc * pDesc = AT45_FindDevice(&sAt45, status);
	//
	//	char txt[100];
	//
	//	if (pDesc == 0)
	//		printf("Nic");
	//	else {
	//		sprintf(txt, "Test: %s page size %d", pDesc->name,
	//				AT45_PageSize(&sAt45));
	//		printf(txt);
	//	}
	//	printf("poszlo");
	//	int i = 0;
	//	int j = 0;
	//	int iPageSize = AT45_PageSize(&sAt45);
	//	//  char wrbuff[iPageSize];
	//	//  memset(wrbuff,0x00,iPageSize);
	//	//  char rdbuff[iPageSize];
	//	//  memset(rdbuff,0x00,iPageSize);
	//	//  char bOK = 1;
	//	//
	//	int iAmount = 1200;
	//
	//  printf("Usuwanie...");
	//  for(i = 0; i < (iAmount/8)+1; i++)
	//    AT45_EraseBlock(&sAt45,i*512*8);
	//
	//  printf(" Zapis... ");
	//  for(i=0 ; i<iAmount && bOK; i++)
	//  {
	//    memset(wrbuff,i%256,iPageSize);
	////    AT45_FastWrite(&sAt45,i%2,wrbuff,iPageSize,iPageSize*i);
	//    AT45_WriteBuffer(&sAt45,i%2,wrbuff,iPageSize);
	//    while (AT45_IsBusy(&sAt45))
	//    {
	//      SANITY_CHECK(&sAt45);
	//
	//      // Wait for transfer to finish
	//      while (AT45_IsBusy(&sAt45)) {
	//
	//          SPID_Handler(sAt45.pSpid);
	//      }
	//    }
	//    AT45D_WaitReady(&sAt45);
	//    AT45_BufferToMem(&sAt45,i%2,iPageSize*i);
	//  }
	//  AT45D_WaitReady(&sAt45);
	//
	//  printf(" Odczyt... ");
	//  for(i=0 ; i<iAmount && bOK; i++)
	//  {
	//    memset(wrbuff,i%256,iPageSize);
	//    memset(rdbuff,0x00,iPageSize);
	//
	//    AT45D_Read(&sAt45,rdbuff,iPageSize,iPageSize*i);
	//
	//    for(j=0 ; j<iPageSize; j++)
	//    {
	//      if(!bOK || wrbuff[j] != rdbuff[j])
	//      {
	//        sprintf(txt,"i=%d wrbuff[%d]=%d \r\n",i,j,wrbuff[j]);
	//        printf(txt);
	//        sprintf(txt,"i=%d rdbuff[%d]=%d \r\n",i,j,rdbuff[j]);
	//        printf(txt);
	//        bOK = 0;
	//        break;
	//      }
	//    }
	//  }
	//  AT45D_Erase(&sAt45,0);
	//  AT45_FastWrite(&sAt45,1,"AllOK",6,0);
	//  AT45D_WaitReady(&sAt45);
	//  AT45D_Read(&sAt45,rdbuff,6,0);
	//  printf("\n\r");
	//  printf(rdbuff);
	//	printf("Usuwanie...");
	//	for (i = 0; i < (iAmount / 8) + 1; i++)
	//		AT45_EraseBlock(&sAt45, i * 512 * 8);
	//	AT45_EraseChip(&sAt45);
	//	printf(" KONIEC ");

	//  LcdClear();
	//  LcdPrint("Gotowy");

	//wlaczenie TWI
	//TWI_Configure(400000); //400KHz

	//konfiguracja wyj�cia serwa
	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, PIO_PA0);
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~PIO_PA0);

	//konfiguracja zasilania serwa
	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, SERWO_POWER);
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~SERWO_POWER);
	//wylaczenie zasilania serwa
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SERWO_POWER);

	//konfiguracja wyj�cia diody led mocy
	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, LED_POWER);
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~LED_POWER);
	//zgaszenie diody led mocy
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, LED_POWER);
	//konfiguracja wyjsc kierunkowych silnikow (in1-in4)
	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, PIO_PA7); //in1
	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, PIO_PA8); //in2
//	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, PIO_PA9); //in3
//	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, PIO_PA10); //in4

	//konfiguracja linii kamery cam po6030k
	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, CAM_RESET); //reset
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, MCLK);
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, PIO_PA30);
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, AT91C_PA21_PCK1);
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, VSYNC);
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, HSYNC);
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D0); //D0
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D1); //D1
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D2); //D2
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D3); //D3
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D4); //D4
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D5); //D5
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D6); //D6
	AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, CAM_D7); //D7

	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, PIO_PA30); //PCLK
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~PIO_PA30);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, VSYNC); //vsync
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~VSYNC);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, HSYNC); //hsync
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~HSYNC);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D0); //D0
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D0);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D1); //D1
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D1);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D2); //D2
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D2);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D3); //D3
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D3);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D4); //D4
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D4);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D5); //D5
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D5);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D6); //D6
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D6);
	AT91F_PIO_CfgInput(AT91C_BASE_PIOA, CAM_D7); //D7
	AT91F_PIO_CfgPullup(AT91C_BASE_PIOA, ~CAM_D7);

	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, DIODA1); //dioda vsync
	AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, DIODA2); //dioda drop frame

	//  AT91F_PIO_CfgInputFilter( AT91C_BASE_PIOA, 0x4F5F0000);
	AT91F_PIO_InputFilterDisable(AT91C_BASE_PIOA, 0x4F5F0000);
	//zgaszenie diod
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA1);
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA2);

	// INICJALIZACJA KAMERY
	// kamera w stanie reset
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, CAM_RESET);

	// 100 okresow zegara w stanie resetu
	int cam_clk;
	for (cam_clk = 0; cam_clk < 100; cam_clk++) {
		volatile int a = AT91F_PIO_GetInput(AT91C_BASE_PIOA);
		a++;
		a = a + 54;
	}

	// wyjscie ze stanu reset
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, CAM_RESET);

	//0x03 set register to A,B or C, 0x40 turn on test image(0x1a), 0x30 turn of color correction
	unsigned int reg[] = { 0x03, 0x91, 0x03, 0x40 };
	unsigned char comm[] = { 0x00, 0xB0, 0x01, 0x1a };
	//
	////  unsigned int reg[] = {0x03,0x51,0x53,0x54,0x55,0x56,0x57,0x61,0x63,0x80,0x81,0x82,0x03,0x11,0x13,0x14,0x15,0x16,0x17,0x19,0x1B,0x1D,0x1F};
	////  unsigned char comm[] = {0x01,0x04,0x04,0x01,0x43,0x00,0xf3,0x0c,0xec,0x20,0x20,0x01,0x02,0x12,0x0e,0x01,0x30,0x00,0xdf,0x72,0x43,0x50,0x50};
	int twiit = 0;
	unsigned int it = 0;
	//	printf("\r\nSetting camera registers... \r\n");
	//  for(twiit = 0 ; twiit < 4; twiit++) //wykomentuj ta petle zeby nie wyswietlac obrazu testowego
	//  {
	//    TWID_Write(&twid, PO6030K_DEVICE_ID, reg[twiit], 1, comm + twiit, 1, 0);
	//    waitms(200);
	//  }
	//	printf("\r\nCamera registers has been set. \r\n");
	//Konfiguracja modulu bluetooth

	mag_info mg_i = MMC212xM_GetMagneticFieldInfo(&twid);

	printf("MMC212xM response: x = %d y = %d\r\n", mg_i.x, mg_i.y);

	//  AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, DIODA1 );
	//  AT45_EraseChip(&sAt45);

	//zrob 2 razy zdjecie kamera zeby AE przystosowalo sie do swiatla
	//	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA1);
	//	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, DIODA1);
	//	for (cam_clk = 0; cam_clk < 2; cam_clk++) {
	//		AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA2);
	//		while (!(AT91F_PIO_GetInput(AT91C_BASE_PIOA) & VSYNC))
	//			;
	//		AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, DIODA2);
	//		while (AT91F_PIO_GetInput(AT91C_BASE_PIOA) & VSYNC)
	//			;
	//	}
	//	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA1);
	//	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA2);

	//AT91C_BASE_PIOA->PIO_PDSR -- odczytaj wszystkie 32 piny
	//AT91C_BASE_PIOA->PIO_PDSR & 0x2000000 - wyciagnij informacje tylko o 25 pinie (VSYNC)
	//AT91C_BASE_PIOA->PIO_PDSR & 0x4000000 - wyciagnij informacje tylko o 26 pinie (HSYNC)
	//AT91C_BASE_PIOA->PIO_PDSR & 0x40000000 - wyciagnij informacje tylko o 30 pinie (PCLK)
	//	register int row = 0;
	//	register int wsk = 0;
	//	register int iter = 0;
	//	register int bufSel = 0;
	//	register int pix = 0;
	//kod ponizej ma za zadanie umiescic w pamieci obraz o rozmiarze 160x120 kolor
	//musi dzialac bardzo szybko tak zeby nadazyc za clockiem kamery MCLK
	//kiedy VSYNC, HSYNC, PCLK sie zmieniaja i jak to sie ma do danych sciaganych z kamery masz tutaj:
	//http://www.pixelplus.com/config/ZIPdownLoad.asp?filepath=/pdf/vga/PO6030_D.pdf

	//	while (!(AT91C_BASE_PIOA->PIO_PDSR & 0x2000000)) {
	//		while (!(AT91C_BASE_PIOA->PIO_PDSR & 0x2000000))
	//			; //czekaj gdy VSYNC == 0
	//	}
	//
	//	while (AT91C_BASE_PIOA->PIO_PDSR & 0x2000000) //podczas gdy VSYNC == 1
	//	{
	//		while (!(AT91C_BASE_PIOA->PIO_PDSR & 0x4000000)
	//				&& (AT91C_BASE_PIOA->PIO_PDSR & 0x2000000))
	//			;
	//
	//		while (AT91C_BASE_PIOA->PIO_PDSR & 0x4000000) //podczas gdy HSYNC == 1
	//		{
	//			while (!(AT91C_BASE_PIOA->PIO_PDSR & 0x40000000))
	//				; //czekaj gdy PCLK == 0
	//
	//			pix++;
	//			CamR(mem[wsk++]); //odczytaj dane z kamery na narastającym zboczu PCLK
	//			if (wsk & 0x200) {
	//				wsk = 0;
	//
	//				AT45_WriteBuffer(&sAt45, bufSel, mem, 512);
	//
	//				while (!sAt45.pSpid->semaphore) {
	//					while (!sAt45.pSpid->semaphore)
	//						SPID_Handler(sAt45.pSpid);
	//				}
	//
	//				while (!AT45_STATUS_READY(AT45_GetStatus(&sAt45)))
	//					;
	//
	//				AT45_SendCommand(&sAt45, !bufSel ? AT45_BUF1_MEM_NOERASE
	//						: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row << 9, 0, 0);
	//				bufSel = !bufSel;
	//				while (!sAt45.pSpid->semaphore) {
	//					while (!sAt45.pSpid->semaphore)
	//						SPID_Handler(sAt45.pSpid);
	//				}
	//				row++;
	//			}
	//			while (AT91C_BASE_PIOA->PIO_PDSR & 0x40000000)
	//				; //czekaj gdy PCLK == 1
	//		}
	//		wsk = 0;
	//		AT45_WriteBuffer(&sAt45, bufSel, mem, 256);
	//
	//		while (!sAt45.pSpid->semaphore) {
	//			while (!sAt45.pSpid->semaphore)
	//				SPID_Handler(sAt45.pSpid);
	//		}
	//		if (!sAt45.pSpid->semaphore)
	//			SPID_Handler(sAt45.pSpid);
	//
	//		while (!AT45_STATUS_READY(AT45_GetStatus(&sAt45)))
	//			;
	//
	//		AT45_SendCommand(&sAt45, !bufSel ? AT45_BUF1_MEM_NOERASE
	//				: AT45_BUF2_MEM_NOERASE, 4, 0, 0, row << 9, 0, 0);
	//		bufSel = !bufSel;
	//		while (!sAt45.pSpid->semaphore) {
	//			while (!sAt45.pSpid->semaphore)
	//				SPID_Handler(sAt45.pSpid);
	//		}
	//		row++;
	//	}

	//zeby odebrac obraz z kamery musisz wyslac komende: 'z' a potem 0
	//robot zacznie wysylac obraz 160x120 w paczkach po 320. Nie musisz mu nic potwierdzac, tylko odbieraj.
	//Lacznie powinienes dostac 2*160*120 bajtow.
	//jakby udalo Ci sie odebrac czysty obraz z kamery w takim rozmiarze to sprobuj wyslac cos wiekszego.
	//do tego przyda Ci sie pamiec DataFlash czyli funkcje ktore zaczynaja sie od AT45******
	//nazwa pamieci to AT45DB321D tutaj info: http://www.atmel.com/dyn/resources/prod_documents/doc3597.pdf
	//Przeczytaj sobie tam jak zapisywac i odczytywac dane z tej pamieci, najwazniejsze to to ze ma 2 bufory i mozesz
	//pisac po jednym buforze podczas gdy drugi jest zapisywany do pamieci flash.
	//pamietaj zeby sprawdzic czy porzednia operacja na pamieci zostala skonczona: while (!AT45_STATUS_READY(AT45_GetStatus(&sAt45)));

	//Good Luck & High Frag :)


	//      AT45_WriteBuffer(&sAt45,0,mem,320);
	//
	//      while (!AT45_STATUS_READY(AT45_GetStatus(&sAt45)));
	//
	//      AT45_SendCommand(&sAt45, AT45_BUF1_MEM_NOERASE, 4, 0, 0, 512*row, 0, 0);
	//      while (!sAt45.pSpid->semaphore)
	//      {
	//        while (!sAt45.pSpid->semaphore)
	//            SPID_Handler(sAt45.pSpid);
	//      }

	//      while(AT91C_BASE_PIOA->PIO_PDSR & 0x4000000)
	//      {
	//        if(!(AT91C_BASE_PIOA->PIO_PDSR & 0x2000000)) goto end;
	//      }
	//      while(! (AT91C_BASE_PIOA->PIO_PDSR & 0x4000000) )
	//      {
	//        if(!(AT91C_BASE_PIOA->PIO_PDSR & 0x2000000)) goto end;
	//      }
	//      for(pixel=320; pixel<1280; pixel++)
	//      {
	//        while(!(AT91C_BASE_PIOA->PIO_PDSR & 0x40000000));
	//          mem[pixel]=0;
	//        while(AT91C_BASE_PIOA->PIO_PDSR & 0x40000000);      }

	//      AT45_WriteBuffer(&sAt45,1,mem+512,512);

	//      for(pixel=0; pixel<128; pixel++)
	//      {
	//        while(!(AT91C_BASE_PIOA->PIO_PDSR & 0x40000000));
	//          mem[pixel]=0;
	//        while(AT91C_BASE_PIOA->PIO_PDSR & 0x40000000);
	//      }

	//      while (!AT45_STATUS_READY(AT45_GetStatus(&sAt45)));
	//
	//      AT45_SendCommand(&sAt45, AT45_BUF2_MEM_NOERASE, 4, 0, 0, 512*row, 0, 0);
	//      while (!sAt45.pSpid->semaphore)
	//      {
	//        while (!sAt45.pSpid->semaphore)
	//            SPID_Handler(sAt45.pSpid);
	//      }
	//      row++;
	//    }

	//	printf("rows %u | pixels %u |", row, pix);
	//	row = 0;
	// Wait until the command is sent
	//	while (AT45_IsBusy(&sAt45)) {
	//		// Wait for transfer to finish
	//		while (AT45_IsBusy(&sAt45))
	//			SPID_Handler(sAt45.pSpid);
	//	}
	//	AT45_WaitReady(&sAt45);
	it = 0;
	unsigned int it2 = 0;
	for (;;) {
		switch (FrameSizeToGet) {
		case 1://160x100 mono preview
			//			GetFrame(4, 4, 1);
			FrameSizeToGet = 0;
			break;
		case 2: //320x200
			//			GetFrame(2, 2, 1);
			FrameSizeToGet = 0;
			break;
		case 3://160x100 color
			//			GetFrame(16, 4, 1);
			FrameSizeToGet = 0;
			break;
		case 5: {
			//			printf("send");
			//			it = 0;
			//			while (it < 480) {
			//				int pixel = 0;
			//				printf("%d ", it);
			//				AT45D_Read(&sAt45, mem, 1280, 512 * 3 * it);
			//				waitms(175);
			//				it++;
			//				if (!USART_WriteBuffer(AT91C_BASE_US0, mem, 1280)) {
			//					it--;
			//					printf("it = %d\n\r", it);
			//				}
			//			}
			//			FrameSizeToGet = 0;
			break;
		}
		case 6: {
			obstacle_avoid_TestCase();
			break;
		}
		default:
			break;
		}

		if (AutoMode) {
			MaxIndex = Rozpoznaj();
		}
	} //end for
} //end main
