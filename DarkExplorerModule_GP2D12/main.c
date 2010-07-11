// Include files

#include "sharp_gp2d12.h"
#include "Board.h"
#include "pio.h"
#include "lcd.c"

unsigned int diode_state = 0;
unsigned char numbers[10] = {'0','1','2','3','4','5','6','7','8','9'};

unsigned int setDiodeState(unsigned int diode, unsigned int state)
{
    if(state)
        AT91F_PIO_SetOutput( AT91C_BASE_PIOA, diode ); //ustawienie stanu wysokiego
    else
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, diode ); //ustawienie stanu niskiego

    return state;
}

int main(void)
{
    //-------------------------------- MAIN START ----------------------------------

    // Enable User Reset and set its minimal assertion to 960 us
    AT91C_BASE_RSTC->RSTC_RMR = AT91C_RSTC_URSTEN | (0x4<<8) | (unsigned int)(0xA5<<24);

    // mt: added reset enable to make the board reset-button "useful"
    AT91F_RSTSetMode( AT91C_BASE_RSTC , AT91C_RSTC_URSTEN );

    // Enable the clock of the PIOA
    AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA );

    // Output config
    AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA14 );
    AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA13 );

    //wlaczenie ADC
    ADC_Configure(7000000); //7MHz

    // LCD Init & Clear
    LcdInit();
    LcdClear();

    int state = 0;
    while(1)
    {
        ADC_StartConversion(AT91C_ADC_CH4);
        //ADC_Read_2bit(AT91C_ADC_CH4); //2 starsze bity
        //ADC_Read_8bit(AT91C_ADC_CH4); //mlodszy bajt

        int val_rx = (ADC_Read_2bit(AT91C_ADC_CH4) << 8) + ADC_Read_8bit(AT91C_ADC_CH4);
        float output = 3.3f / 1023.0f;
        output *= val_rx * 3.0f;

        char text[7] = "     V\0";
        int val = output * 100;
        text[4] = numbers[val%10];
        val = val / 10;
        text[3] = numbers[val%10];
        text[2] = '.';
        val = val / 10;
        text[1] = numbers[val%10];
        val = val / 10;
        text[0] = numbers[val%10];

        LcdPrint(text);

        setDiodeState(PIO_PA13, output < 2.6);
        diode_state = !diode_state;
        delay(1000000);
        LcdClear();
        setDiodeState(PIO_PA14, diode_state);
    }

    return 0;
}

