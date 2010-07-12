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

#include <string.h>

//adres kamery do zapisu przez TWI
#define PO6030K_DEVICE_ID 0x6E

//flaga dla timera PIT
int PitState=0;

//Ustawienie polozenia serwa
//1000 - 0, 1500-45, 2000 - 90
//2850 - wieza zlozona
//1750 - postawiona pionowo
//900 - maksymalny wyhyl do przodu
volatile int SerwoAngle=1750;
volatile int SerwoAngleBuffer=1750;

//zmienne do sterowania transmisj� UART
char RX_Buffer[2]; //odebrane dane, 0-komenda, 1-dana
int RX_Counter=0; //wskazuje czy byla odebrana komenda
int timeout; //timeout dla komendy

//zmienne konfiguracyjne modulu bluetooth
char bluetooth[] = "ATI?\r";

//Rozmiar ramki do odebrania
volatile char FrameSizeToGet=0;

//Wlacznik trybu autonomicznego
char AutoMode=0, AutoPreview=0;

//pamiec obrazu
char mem[64000];

//tryb auto
//Stos dla SmithFill - mem od 16000
int StackIndex=16000;
char MaxIndex=0;

float wzorzec1=0, wzorzec2=0, wzorzec3=0, wzorzec4=0, wzorzec5=0, wzorzec6=0, wzorzec7=0, wzorzec8=0, wzorzec9=0;
int xwzorzec=0, ywzorzec=0, WzorzecCount=0, RozpoznanyX, RozpoznanyY;

char SilnikiEnable=0;

volatile int xmin, xmax, ymin, ymax;

////////////////////////////////////////////////////////////////////////////////
// Przerwanie od timera PIT
////////////////////////////////////////////////////////////////////////////////
__ramfunc void SYSTEM_INTERRUPT_irq_handler(void)
{
  //impuls od timera PIT pojawia si� co 18-20ms
  
  unsigned int dummy;
  
  //odczyt rejestru PIVR (powoduje wyzerowanie flagi ��dania przerwania)
  dummy = AT91C_BASE_PITC->PITC_PIVR;
  
  //obliczenie pozycji serwa
  if (SerwoAngleBuffer>SerwoAngle) {SerwoAngle+=5;}
  if (SerwoAngleBuffer<SerwoAngle) {SerwoAngle-=5;}
  
  //////////// STEROWANIE SERWEM ///////////////////////////////////////////////
  if(PitState==0)
  {
    //zmiana interwa�u na czas 1-2 ms
    //do wygenerowania impulsu dla serwa
    //dlugosc impulsu = wychylenie serwa
    PIT_ChangeInterval(SerwoAngle);
    PitState=1;

    //wystawnienie jedynki
    AT91F_PIO_SetOutput( AT91C_BASE_PIOA, PIO_PA0 );
  }
  else {
          //zmiana interwa�u na czas 18 ms
          //do wygenerowania odst�pu mi�dzy impulsami
          PIT_ChangeInterval(18000); //18ms
          PitState=0;
          
          //wystawnienie zera
          AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA0 );
		  
		      //timeout ok 200ms - interpretacja komend
		      ++timeout;
		      if (timeout>=10) {timeout=10; RX_Counter=0;}
        }
  //////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
// Przerwanie od kanalu odbiorczego UART0
////////////////////////////////////////////////////////////////////////////////
__ramfunc void UART0_DMA_irq_handler(void)
{
RX_Buffer[RX_Counter] = USART_Read(AT91C_BASE_US0,10);//UART0_Read();
//USART_Write(AT91C_BASE_US0,RX_Buffer[RX_Counter],10);
//USART_WriteBuffer(AT91C_BASE_US0, "\n\r", 2);
RX_Counter++; timeout=0;
if (RX_Counter==2) { RX_Counter=0; } //2 bajty na ramke

//Dekodowanie komend
if (RX_Counter==0) //po odebraniu komendy i danych
{
  //Sterowanie serwem
  if(RX_Buffer[0] == 's')
  {
    SerwoAngleBuffer=1065 + ((255-RX_Buffer[1])*7);
  }

  //Sterowanie kanalem pwm 0, 1
  if(RX_Buffer[0] == 'a')
  {
    PWM_Set(0, RX_Buffer[1]);
    PWM_Set(1, RX_Buffer[1]);
  }
  
  //Sterowanie kanalem pwm 2, 3
  if(RX_Buffer[0] == 'b')
  {
    PWM_Set(2, RX_Buffer[1]);
    PWM_Set(3, RX_Buffer[1]);
  }

  //silniki 1-2 kierunek (lewe)
  if(RX_Buffer[0] == 'l')
  {
    Kierunek(1, RX_Buffer[1]);
  }

  //silniki 3-4 kierunek (prawe)
  if(RX_Buffer[0] == 'r')
  {
    Kierunek(2, RX_Buffer[1]);
  }

  //pomiar napiecia beterii
  if(RX_Buffer[0] == 't')
  {
    ADC_StartConversion(AT91C_ADC_CH4);
     RX_Buffer[0]=ADC_Read_2bit(AT91C_ADC_CH4); //2 starsze bity
    RX_Buffer[1]=ADC_Read_8bit(AT91C_ADC_CH4); //mlodszy bajt
     UART0_DMA_Write(&RX_Buffer[0], 2);
  }

  //wyslanie klatki obrazu 320 x 200
  if(RX_Buffer[0] == 'f')
  {
    if (RX_Buffer[1]==0)
    {
      FrameSizeToGet=2;
    }

    if (RX_Buffer[1]>0 && RX_Buffer[1]<20)
    {
      UART0_DMA_Write(&mem[3200*RX_Buffer[1]], 3200);
    }
  }

  //wyslanie klatki obrazu 160 x 100
  if(RX_Buffer[0] == 'p')
  {
    if (RX_Buffer[1]==0)
    {
      FrameSizeToGet=1;
    }

    if (RX_Buffer[1]>0 && RX_Buffer[1]<5)
    {
      UART0_DMA_Write(&mem[3200*RX_Buffer[1]], 3200);
    }
  }

  //wyslanie klatki obrazu 160 x 100 kolorowego
  if(RX_Buffer[0] == 'x')
  {
    if (RX_Buffer[1]==0)
    {
      FrameSizeToGet=3;
    }

    if (RX_Buffer[1]>0 && RX_Buffer[1]<10)
    {
      UART0_DMA_Write(&mem[3200*RX_Buffer[1]], 3200);
    }
  }

  //tryb autonomiczny komenda i
  if(RX_Buffer[0] == 'i')
  {
    if (RX_Buffer[1]==10)
    {
      AutoMode=1;
    }

    if (RX_Buffer[1]==11)
    {
      AutoMode=0;
    }

    if (RX_Buffer[1]==12)
    {
      AutoPreview=1;
    }

    if (RX_Buffer[1]==13)
    {
      AutoPreview=0;
    }

    //podglad bufora obrazu - automatyczny
    if (RX_Buffer[1]<5)
    {
      UART0_DMA_Write(&mem[(3200*RX_Buffer[1])+48000], 3200);
    }
  }

  //tryb autonomiczny komenda j
  if(RX_Buffer[0] == 'j')
  {
    //podglad bufora obrazu - manualny
    if (RX_Buffer[1]<5)
    {
      UART0_DMA_Write(&mem[(3200*RX_Buffer[1])], 3200);
    }
  }

  //zapamietanie wskazanego wzorca - wspolrzedna x
  if(RX_Buffer[0] == 'm')
  {
    xwzorzec = RX_Buffer[1];
	xwzorzec=xwzorzec*1.3;
    SilnikiEnable=1;
  }

  //zapamietanie wskazanego wzorca - wspolrzedna y
  if(RX_Buffer[0] == 'n')
  {
    ywzorzec = RX_Buffer[1];
  }

  //dioda led
  if(RX_Buffer[0] == 'd')
  {
    //wylaczenie diody
    if (RX_Buffer[1]==0)
    {
      AT91F_PIO_SetOutput( AT91C_BASE_PIOA, LED_POWER );
    }

    //wlaczenie diody
    if (RX_Buffer[1]==1)
    {
      AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, LED_POWER );
    }
  }

  //serwo zasilanie
  if(RX_Buffer[0] == 'w')
  {
    //wylaczenie
    if (RX_Buffer[1]==0)
    {
      AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, SERWO_POWER );
    }

    //wlaczenie
    if (RX_Buffer[1]==1)
    {
      AT91F_PIO_SetOutput( AT91C_BASE_PIOA, SERWO_POWER );
    }
  }
}
}

////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////
int main(void)
{
  // Inicjalizacje


  TRACE_CONFIGURE(DBGU_STANDARD, 9600, BOARD_MCK);
  printf("-- Dark Explorer with AT91LIB v. %s --\n\r", SOFTPACK_VERSION);
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

  // Enable User Reset and set its minimal assertion to 960 us
//  AT91C_BASE_RSTC->RSTC_RMR = AT91C_RSTC_URSTEN | (0x4<<8) | (unsigned int)(0xA5<<24);

  // mt: added reset enable to make the board reset-button "useful"
//  AT91F_RSTSetMode( AT91C_BASE_RSTC , AT91C_RSTC_URST
  LcdInit();
  // Enable the clock of the PIOA
  PMC_EnablePeripheral(AT91C_ID_PIOA);
//  AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA );

  // W��czenie timera PIT
//  PIT_Init(100,MCK);
  PIT_Configure(100); //pierwsze przerwanie za 100us

  // Wlaczenie PWM
  PWM_Configure();

  //wlaczenie UART0
//  UART0_DMA_Configure(9600); //115.2kbit/s
//  UART0_DMA_Configure(230400); //230.4kbit/s
  UART0_DMA_Configure(460800); //460.8kbit/s

  //wlaczenie ADC
  ADC_Configure(1000000); //1MHz


  LcdClear();
  LcdPrint("Gotowy");

  //wlaczenie TWI
  //TWI_Configure(400000); //400KHz

  //konfiguracja wyj�cia serwa
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA0 );
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~PIO_PA0 );

  //konfiguracja zasilania serwa
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, SERWO_POWER );
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~SERWO_POWER );
  //wylaczenie zasilania serwa
  AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, SERWO_POWER );

  //konfiguracja wyj�cia diody led mocy
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, LED_POWER );
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~LED_POWER );
  //zgaszenie diody led mocy
  AT91F_PIO_SetOutput( AT91C_BASE_PIOA, LED_POWER );
  //konfiguracja wyjsc kierunkowych silnikow (in1-in4)
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA7 );  //in1
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA8 );  //in2
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA9 );  //in3
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA10 ); //in4

  //konfiguracja linii kamery cam po6030k
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, CAM_RESET ); //reset
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, MCLK ); //mclk
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, VSYNC ); //vsync
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~VSYNC );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, HSYNC ); //hsync
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~HSYNC );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D0 ); //D0
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D0 );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D1 ); //D1
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D1 );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D2 ); //D2
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D2 );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D3 ); //D3
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D3 );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D4 ); //D4
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D4 );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D5 ); //D5
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D5 );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D6 ); //D6
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D6 );
  AT91F_PIO_CfgInput( AT91C_BASE_PIOA, CAM_D7 ); //D7
  AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, ~CAM_D7 );

  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, DIODA1 ); //dioda vsync
  AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, DIODA2 ); //dioda drop frame
  //zgaszenie diod
  AT91F_PIO_SetOutput( AT91C_BASE_PIOA, DIODA1 );
  AT91F_PIO_SetOutput( AT91C_BASE_PIOA, DIODA2 );

  // INICJALIZACJA KAMERY
  // kamera w stanie reset
  AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, CAM_RESET );

  // 100 okresow zegara w stanie resetu
  int cam_clk;
  for(cam_clk=0; cam_clk<100; cam_clk++)
  {
   AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, MCLK );
   AT91F_PIO_SetOutput( AT91C_BASE_PIOA, MCLK );
  }

  // wyjscie ze stanu reset
  AT91F_PIO_SetOutput( AT91C_BASE_PIOA, CAM_RESET );

  // pojedyncze tykniecie zegara
  AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, MCLK );
  AT91F_PIO_SetOutput( AT91C_BASE_PIOA, MCLK );

  //Konfiguracja modulu bluetooth

//  waitms(500);
////  UART0_DMA_Write(&bluetooth[0], 1);
//  USART_Write(AT91C_BASE_US0,&bluetooth[0],10);
//  waitms(500);
////  UART0_DMA_Write(&bluetooth[1], 1);
//  USART_Write(AT91C_BASE_US0,&bluetooth[1],10);
//  waitms(500);
////  UART0_DMA_Write(&bluetooth[2], 1);
//  USART_Write(AT91C_BASE_US0,&bluetooth[2],10);
//  waitms(500);
////  UART0_DMA_Write(&bluetooth[3], 1);
//  USART_Write(AT91C_BASE_US0,&bluetooth[3],10);
//  waitms(500);
////  UART0_DMA_Write(&bluetooth[4], 1);
//  USART_Write(AT91C_BASE_US0,&bluetooth[4],10);

  waitms(50);
//  UART0_DMA_Write(&bluetooth[0], 1);
  USART_Write(AT91C_BASE_US0,bluetooth[0],10);
  waitms(50);
//  UART0_DMA_Write(&bluetooth[1], 1);
  USART_Write(AT91C_BASE_US0,bluetooth[1],10);
  waitms(50);
//  UART0_DMA_Write(&bluetooth[2], 1);
  USART_Write(AT91C_BASE_US0,bluetooth[2],10);
  waitms(50);
//  UART0_DMA_Write(&bluetooth[3], 1);
  USART_Write(AT91C_BASE_US0,bluetooth[3],10);
  waitms(50);
//  UART0_DMA_Write(&bluetooth[4], 1);
  USART_Write(AT91C_BASE_US0,bluetooth[4],10);

  for (;;)
  {
    switch (FrameSizeToGet) {
      case 1://160x100 mono preview
        GetFrame( 4, 4, 1 );
        FrameSizeToGet=0;
        break;
      case 2: //320x200
        GetFrame( 2, 2, 1 );
        FrameSizeToGet=0;
        break;
      case 3://160x100 color
        GetFrame( 16, 4, 1 );
        FrameSizeToGet=0;
        break;
      }

    if(AutoMode)
    {
      MaxIndex = Rozpoznaj();
    }
  } //end for
} //end main
