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
#include <usart/usart.h>

#include <string.h>

#include "modules/sharp_gp2d12.h"
#include "modules/mmc212xm.h"
#include "modules/at45db321d.h"
#include "modules/po6030k.h"

#include "algorithms/obstacle_avoidance.h"
#include "algorithms/reverse_track_reconstruction.h"

#include "peripherals/adc_helper.h"

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

//tryb auto
//Stos dla SmithFill - mem od 16000
int StackIndex = 16000;
char MaxIndex = 0;

float wzorzec1 = 0, wzorzec2 = 0, wzorzec3 = 0, wzorzec4 = 0, wzorzec5 = 0,
		wzorzec6 = 0, wzorzec7 = 0, wzorzec8 = 0, wzorzec9 = 0;
int xwzorzec = 0, ywzorzec = 0, WzorzecCount = 0, RozpoznanyX, RozpoznanyY;

char SilnikiEnable = 0;

volatile int xmin, xmax, ymin, ymax;

extern char mem[60000];

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
volatile unsigned int obstacle_avoid_enable = 0;

void onDistanceDataReady(unsigned int rs_output, unsigned int ls_output) {
	rs_data = rs_output;
	ls_data = ls_output;

	data_ready = 1;
}

void obstacle_avoid_TestCase() {
	while (obstacle_avoid_enable) {
		data_ready = 0;
		ADC_StartDoubleChannelConversion(ADC_CHANNEL_5, ADC_CHANNEL_6,
				onDistanceDataReady);
		while (!data_ready)
			;

		init_oa_configuration();

		unsigned int level_mask = create_level_mask(rs_data, ls_data);
		OAA_OUTPUT output = avoid_obstacles(level_mask);

		PWM_Set(0, output.speed_right);
		Kierunek(1, output.gear_left);
		Kierunek(2, output.gear_right);

		if (output.gear_left != output.gear_right)
			waitms(1000);
		else
			waitms(500);
	}

	PWM_Set(0, 0);
	Kierunek(1, STOP_GEAR);
	Kierunek(2, STOP_GEAR);

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
			//			PWM_Set(0, 85);
			//			PWM_Set(1, RX_Buffer[1]);
			FrameSizeToGet = 13;
		}
		//TODO
		//Sterowanie kanalem pwm 2, 3
		if (RX_Buffer[0] == 'b') {
			//			PWM_Set(0, 85);
			//			printf("PWM23 - RX_BUffer[1] = %d", RX_Buffer[1]);
			//			PWM_Set(2, RX_Buffer[1]);
			//			PWM_Set(3, RX_Buffer[1]);
			//FrameSizeToGet = 8;
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

		if (RX_Buffer[0] == 'z') {
			if (RX_Buffer[1] == 0) {
				FrameSizeToGet = 1;
			}
		}
    if (RX_Buffer[0] == 'h') {
      if (RX_Buffer[1] == 0) {
        FrameSizeToGet = 2;
      }
    }
    if (RX_Buffer[0] == 'x') {
      if (RX_Buffer[1] == 0) {
        FrameSizeToGet = 3;
      }
    }
    if (RX_Buffer[0] == 'g') {
      if (RX_Buffer[1] == 0) {
        FrameSizeToGet = 4;
      }
    }
    if (RX_Buffer[0] == 'f') {
      if (RX_Buffer[1] == 0) {
        FrameSizeToGet = 5;
      }
    }
    if (RX_Buffer[0] == 'p') {
      if (RX_Buffer[1] == 0) {
        FrameSizeToGet = 6;
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
		//TODO
		if (RX_Buffer[0] == 'd') {
			//wylaczenie diody
			if (RX_Buffer[1] == 0) {
				//				FrameSizeToGet = 8;
				stop_recording_track();
				//AT91F_PIO_SetOutput(AT91C_BASE_PIOA, LED_POWER);
			}

			//wlaczenie diody
			if (RX_Buffer[1] == 1) {
				FrameSizeToGet = 8;
				//AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, LED_POWER);
			}
		}

		//serwo zasilanie
		if (RX_Buffer[0] == 'w') {
			//wylaczenie
			if (RX_Buffer[1] == 0) {
				obstacle_avoid_enable = 0;
				data_ready = 1;
				//				AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SERWO_POWER);
			}

			//wlaczenie
			if (RX_Buffer[1] == 1) {
				obstacle_avoid_enable = 1;
				FrameSizeToGet = 7;
				//AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SERWO_POWER);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////
int main(void) {
	// Inicjalizacje

	//TODO
	TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);
	TRACE_INFO("-- Dark Explorer with AT91LIB v. %s --\n\r", SOFTPACK_VERSION);
	//	memset(mem, 0x00, 39000);
	TRACE_INFO("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

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
	//	PIT_Configure(100); //pierwsze przerwanie za 100us

	// Wlaczenie PWM
	PWM_Configure();

	PIO_Configure(pins, PIO_LISTSIZE(pins));

	// Configure TWI
	// In IRQ mode: to avoid problems, the priority of the TWI IRQ must be max.
	// In polling mode: try to disable all IRQs if possible.
	// (in this example it does not matter, there is only the TWI IRQ active)
	PMC_EnablePeripheral(AT91C_ID_TWI);
	TWI_ConfigureMaster(AT91C_BASE_TWI, TWCK, BOARD_MCK);
	TWID_Initialize(&twid, AT91C_BASE_TWI);

	//wlaczenie UART0
	//  UART0_DMA_Configure(9600); //115.2kbit/s
	//  UART0_DMA_Configure(230400); //230.4kbit/s
	UART0_DMA_Configure(460800); //460.8kbit/s

	//wlaczenie ADC
	ADC_Configure();

	//	// KONFIGURACJA pinow SPI dla pamieci flash oraz PA21 dla MCK kamery
	AT91F_PIO_CfgPeriph(AT91C_BASE_PIOA, AT91C_PA12_MISO | AT91C_PA13_MOSI
			| AT91C_PA14_SPCK | AT91C_PA31_NPCS1, AT91C_PA21_PCK1
			| AT91C_PA11_PWM0);

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
	//TODO

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

	//TODO diódeczka :D :*
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, DIODA2);

	AT45DB321D_Initalize();
	//		AT45DB321D_SelfTest();
	AT45DB321D_ClearChip();
	//	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, DIODA2);
  PO6030K_Initalize();
  PO6030K_InitRegisters(&twid);
	PO6030K_TakePicture();
	int iAmountOfPackets=0;
	//TODO
	for (;;) {
		switch (FrameSizeToGet) {
		case 1://640x480 color
		  iAmountOfPackets = 480;
		case 2://320x240 color
		  iAmountOfPackets = iAmountOfPackets ? iAmountOfPackets : 120;
		case 3://160x120 color
		  iAmountOfPackets = iAmountOfPackets ? iAmountOfPackets : 30;
		case 4://640x480 b&w
		  iAmountOfPackets = iAmountOfPackets ? iAmountOfPackets : 240;
		case 5://320x240 b&w
		  iAmountOfPackets = iAmountOfPackets ? iAmountOfPackets : 60;
		case 6://160x120 b&w
		{
      iAmountOfPackets = iAmountOfPackets ? iAmountOfPackets : 15;
			TRACE_DEBUG("Sending image...");
			int it = 0;
			At45 *pAt45 = AT45DB321D_GetPointer();
			while (!pAt45->pSpid->semaphore)
			{
				while (!pAt45->pSpid->semaphore)
					SPID_Handler(pAt45->pSpid);
			}
			while (it < iAmountOfPackets)
			{
				TRACE_DEBUG("Sent row %d", it);
				AT45DB321D_Read(mem, 1280, 1280 * it + 512);
				waitms(150);
				it++;
				if (!USART_WriteBuffer(AT91C_BASE_US0, mem, 1280)) {
					it--;
				}
			}

			FrameSizeToGet = 0;
			break;
		}
		case 7: {
			obstacle_avoid_TestCase();
			FrameSizeToGet = 0;
			break;
		}
		case 8: {
			start_recording_track();
			FrameSizeToGet = 0;
			break;
		}
		case 9: {
			FrameSizeToGet = 0;
			break;
		}
		case 13: {
			reconstruct_reverse_track();
			//start_recording_track();
			FrameSizeToGet = 0;
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
