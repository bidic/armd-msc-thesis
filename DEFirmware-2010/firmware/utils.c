/**
* Projekt    : Dark Explorer firmware
* Plik       : utils.c
* Zawartosc  : Procedury obliczeniowe i steruj�ce wy�szego poziomu
* Autor      : Pawe� Kmak
* Data       : 1.08.2009
**/

#include <de_board.h>
#include <de_pio.h>
#include <utils.h>
#include <peripherals.h>

////////////////////////////////////////////////////////////////////////////////
// Globalne zmienne zewnetrzne
////////////////////////////////////////////////////////////////////////////////
extern char mem[];
extern int StackIndex;
extern char MaxIndex;
extern volatile int xmin, xmax, ymin, ymax;
#define PO6030K_DEVICE_ID 0x6E

////////////////////////////////////////////////////////////////////////////////
// Sterowanie kierunkiem obrotu silnikow
// silnik = 1 - silniki lewe, 2 - silniki prawe
// kierunek = 0 - stop, 1 - przod, 2 - tyl
////////////////////////////////////////////////////////////////////////////////
inline void Kierunek(int silnik, int kierunek)
{
  if(silnik == 1)
  {
    switch (kierunek)
    {
      case 0: 
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA7 );
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA8 );
        break;

      case 1: 
        AT91F_PIO_SetOutput( AT91C_BASE_PIOA, PIO_PA7 );
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA8 );
        break;

      case 2: 
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA7 );
        AT91F_PIO_SetOutput( AT91C_BASE_PIOA, PIO_PA8 );
        break;
    }
  }
  
  if(silnik == 2)
  {
    switch (kierunek)
    {
      case 0: 
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA9 );
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA10 );
        break;

      case 1: 
        AT91F_PIO_SetOutput( AT91C_BASE_PIOA, PIO_PA9 );
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA10 );
        break;

      case 2: 
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA9 );
        AT91F_PIO_SetOutput( AT91C_BASE_PIOA, PIO_PA10 );
        break;
    }
  }
  switch (kierunek)
  {
    case 0:
      LcdClear();
      LcdPrint("STOP!");
      break;

    case 1:
      LcdClear();
      LcdPrint("Do przodu!");
      break;

    case 2:
      LcdClear();
      LcdPrint("Do tylu!");
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Proste sterowanie silnikami
// kierunek 0 - stop, 1 - w lewo, 2 - prosto, 3 - w prawo
////////////////////////////////////////////////////////////////////////////////
inline void go(char kierunek, char pwm1, char pwm2)
{
  switch (kierunek)
  {
    case 0:
      Kierunek(1, 0);
      Kierunek(2, 0); 
      PWM_Set(0, pwm1);
      PWM_Set(1, pwm1);
      PWM_Set(2, pwm2);
      PWM_Set(3, pwm2);
      break;

    case 1:
      Kierunek(1, 2);
      Kierunek(2, 1); 
      PWM_Set(0, pwm1);
      PWM_Set(1, pwm1);
      PWM_Set(2, pwm2);
      PWM_Set(3, pwm2);
      break;

    case 2:
      Kierunek(1, 1);
      Kierunek(2, 1); 
      PWM_Set(0, pwm1);
      PWM_Set(1, pwm1);
      PWM_Set(2, pwm2);
      PWM_Set(3, pwm2);
      break;

    case 3:
      Kierunek(1, 1);
      Kierunek(2, 2); 
      PWM_Set(0, pwm1);
      PWM_Set(1, pwm1);
      PWM_Set(2, pwm2);
      PWM_Set(3, pwm2);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Odebranie 8 bit�w danych z kamery
////////////////////////////////////////////////////////////////////////////////
inline char CamRead(void)
{
  register char data=0;

  data = ( AT91F_PIO_GetInput(AT91C_BASE_PIOA) & (CAM_D2 | CAM_D3 | CAM_D4 | CAM_D5 | CAM_D6 | CAM_D7) ) >> 14;
  if (AT91F_PIO_GetInput(AT91C_BASE_PIOA) & CAM_D0)  {data += 1;}
  if (AT91F_PIO_GetInput(AT91C_BASE_PIOA) & CAM_D1) {data += 2;}

 return data;
}

////////////////////////////////////////////////////////////////////////////////
// Odebranie ramki obrazu z kamery // po optymalizacji //
// podzielnik = 2 -> klatka 320 x 200
// podzielnik = 4 -> klatka 160 x 100
////////////////////////////////////////////////////////////////////////////////
void GetFrame( register char PodzielnikX, register char PodzielnikY, char transmit )
{
  register unsigned int x=0, y=0, wsk=0;
  register char x2=0, y2=0, CamClockEnable=0;

  // 1-sza klatka do odrzutu
  CamClockEnable=1;
  AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, DIODA2 );
  while ( AT91F_PIO_GetInput(AT91C_BASE_PIOA) & VSYNC )
  {
    AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, MCLK );
    AT91F_PIO_SetOutput( AT91C_BASE_PIOA, MCLK );
  }

  printf("-- Poza petla --\n\r");
  AT91F_PIO_SetOutput( AT91C_BASE_PIOA, DIODA2 );
  CamClockEnable=1;
  wsk=0;


  register int pclkJump;

  register int enter = 0;
  // 2-ga klatka dobra
  AT91C_BASE_PIOA->PIO_CODR = DIODA1;
  while (CamClockEnable)
  {
    for(pclkJump = 0; pclkJump < PodzielnikX ; pclkJump++)
    {
      AT91C_BASE_PIOA->PIO_CODR = MCLK;
      AT91C_BASE_PIOA->PIO_SODR = MCLK;
//      AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, MCLK );
//      AT91F_PIO_SetOutput( AT91C_BASE_PIOA, MCLK );
    }

    // VSYNC = 1 (podczas aktywnej czesci klatki)
    if ( AT91C_BASE_PIOA->PIO_PDSR & VSYNC )
    {
//      AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, DIODA1 );
      // HSYNC = 1 (podczas aktywnej czesci linii)
      if ( AT91C_BASE_PIOA->PIO_PDSR & HSYNC  )
      {
        enter = 1;
        if (PodzielnikX < 8) 
        {
          if (x<(640. / PodzielnikX) && y< (400. / PodzielnikY))
          {
//            printf(".");
            x++;
            mem[wsk] = ( AT91C_BASE_PIOA->PIO_PDSR & (CAM_D2 | CAM_D3 | CAM_D4 | CAM_D5 | CAM_D6 | CAM_D7) ) >> 14;
            if (AT91C_BASE_PIOA->PIO_PDSR & CAM_D0)  {mem[wsk] += 1;}
            if (AT91C_BASE_PIOA->PIO_PDSR & CAM_D1) {mem[wsk] += 2;}
            wsk++;
          }
          else if(y>=400/PodzielnikY)
            CamClockEnable = 0;
          if (CamClockEnable==1) { CamClockEnable=2; }

        } else {
                if (x<1280 && y<800) 
                {
                  if ((x2<3 || x2==9) && y2==0) { mem[wsk] = CamRead(); ++wsk;}
                  ++x; ++x2;
                  if(x2==PodzielnikX) {x2=0;}
                }
                if (CamClockEnable==1) { CamClockEnable=2; }
              }
      }
      else if(enter)
      {
//        printf("(%d, %d)\n\r",x,y);
        y++;
        x=0;
        enter = 0;
      }
    }
    else if((y >= (400./PodzielnikY)) && (x >= (640. / PodzielnikX)) && !(AT91F_PIO_GetInput(AT91C_BASE_PIOA) & VSYNC))
    {
      CamClockEnable = 0;
    }

    //wyslanie pierwszego bloku danych 
    if(wsk==3200 && CamClockEnable==2 && transmit)
    {
      UART0_DMA_Write(mem, wsk);
      printf("Sent: %d\n\r",wsk);
      CamClockEnable = 3;
      wsk = 0;
    }

  } // end while (CamClockEnable)
  AT91F_PIO_SetOutput( AT91C_BASE_PIOA, DIODA1 );
  printf("wsk = %d\n\r",wsk);
} // end GetFrame()

////////////////////////////////////////////////////////////////////////////////
// Funkcje pomocnicze
// - obliczanie pozycji piksela w tablicy
// - konwersja do zapisu w trybie 2bity na piksel
////////////////////////////////////////////////////////////////////////////////

long int ToWsk(int x, int y)
{
  if(x<320 && y<200 && x>0 && y>0) { return(x+(y*320)); }
     else return 0;
}

int GetPx2bit(long int wsk)
{
  long int pos_bajt;
  int pos_bit;

  pos_bajt = wsk>>2; //dzielenie przez 4
  pos_bit = wsk%4;

  return ( mem[pos_bajt] >> (6-pos_bit-pos_bit) ) & 0x03;
}

void SetPx2bit(long int wsk, char kolor)
{
  long int pos_bajt;
  int pos_bit, mask[4] = {192, 48, 12, 3};

  pos_bajt = wsk>>2; //dzielenie przez 4
  pos_bit = wsk%4;

  mem[pos_bajt] &= ~mask[pos_bit]; //zerowanie odpowiednich bitow 
  mem[pos_bajt] |= ( kolor << (6-pos_bit-pos_bit) ); //zapis wyzerowanych bitow
}

////////////////////////////////////////////////////////////////////////////////
// Implementacja stosu na tablicy mem dla SmithFill
////////////////////////////////////////////////////////////////////////////////
void StackPush(int data)
{
  mem[++StackIndex] = (data>>8) & 0xFF;
  mem[++StackIndex] = data & 0xFF;
}

int StackPop()
{
  int val1, val2;
  val1 = mem[StackIndex--];
  val2 = mem[StackIndex--];
  val2 = val2<<8;
  return val1+val2;
}

////////////////////////////////////////////////////////////////////////////////
// SmithFill - indeksacja obszar�w sp�jnych
////////////////////////////////////////////////////////////////////////////////
int SmithFill(int xstart, int ystart, char ChangeColor, char FloodColor)
{
  int x, y, xst, yst, PixelCounter=0;
  char up, down, kolor;

  //prostokat obcinania inicjalizacja
  xmin = 500; xmax = 0;  ymin = 500; ymax = 0;

  //punkt poczatkowy na stos
  StackPush(xstart);
  StackPush(ystart);

  //dopoki dane na stosie
  while (StackIndex>16000)
  {
    //pobranie punktu startowego
    yst = StackPop();
    xst = StackPop();
    x = xst;
    y = yst;

    //znaczniki kierunku
    up=1; down=1;

    //odczyt koloru
    kolor = GetPx2bit(ToWsk(x,y));

    //malowanie w lewo
    while (x>=0 && kolor==ChangeColor)
    {
      //w gore
      kolor = GetPx2bit(ToWsk(x,y+1));
      if (y<199 && kolor==ChangeColor)
      {
        if(up)
        {
          StackPush(x);
          StackPush(y+1);
          up=0;
        }
      }
      else { up=1; }

      //w dol
      kolor = GetPx2bit(ToWsk(x,y-1));
      if (y>0 && kolor==ChangeColor)
      {
        if(down)
        {
          StackPush(x);
          StackPush(y-1);
          down=0;
        }
      }
      else { down=1; }

      //zamalowanie piksela
      SetPx2bit(ToWsk(x,y), FloodColor);
      ++PixelCounter;

      //prostokat obcinania
      if ( x < xmin ) { xmin = x; } //xmin
      if ( x > xmax ) { xmax = x; } //xmax
      if ( y < ymin ) { ymin = y; } //ymin
      if ( y > ymax ) { ymax = y; } //ymax

      //w lewo
      --x;
      kolor = GetPx2bit(ToWsk(x,y));
    }

    //na poczatek
    x = xst + 1;
    y = yst;

    //znaczniki
    up=1;
    down=1;

    //odczyt koloru
    kolor = GetPx2bit(ToWsk(x,y));

    //malowanie w prawo
    while (x<=319 && kolor==ChangeColor)
    {
      //w gore
      kolor = GetPx2bit(ToWsk(x,y+1));
      if (y<199 && kolor==ChangeColor)
      {
        if(up)
        {
          StackPush(x);
          StackPush(y+1);
          up=0;
        }
      }
      else { up=1; }

      //w dol
      kolor = GetPx2bit(ToWsk(x,y-1));
      if (y>0 && kolor==ChangeColor)
      {
        if(down)
        {
          StackPush(x);
          StackPush(y-1);
          down=0;
        }
      }
      else { down=1; }

      //zamalowanie piksela
      SetPx2bit(ToWsk(x,y), FloodColor);
      ++PixelCounter;

      //prostokat obcinania
      if ( x < xmin ) { xmin = x; } //xmin
      if ( x > xmax ) { xmax = x; } //xmax
      if ( y < ymin ) { ymin = y; } //ymin
      if ( y > ymax ) { ymax = y; } //ymax

      //w prawo
      ++x;
      kolor = GetPx2bit(ToWsk(x,y));
    }
  }

  return PixelCounter;
}
