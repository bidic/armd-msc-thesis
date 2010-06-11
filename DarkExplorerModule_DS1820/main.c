// Include files

#include "dallas_ds1820.h"
#include "Board.h"
#include "pio.h"
#include "lcd.c"

unsigned char l, remain, temp_txt[]="  zxx.xxxx\xdf\x43";
unsigned short int temp;
float t;
unsigned int diode_state = 0;

unsigned int setDiodeState(unsigned int state)
{
    if(state)
        AT91F_PIO_SetOutput( AT91C_BASE_PIOA, PIO_PA7 ); //ustawienie stanu wysokiego
    else
        AT91F_PIO_ClearOutput( AT91C_BASE_PIOA, PIO_PA7 ); //ustawienie stanu niskiego

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
    AT91F_PIO_CfgOutput( AT91C_BASE_PIOA, PIO_PA7 );

    // LCD Init & Clear
    LcdInit();
    LcdClear();

    // Digital Thermometer Init
    wire_init();

    LcdPrint("DarkExplorer 2.0");

    while(1)
    {
        if (!wire_reset())
        {

            LcdSetCursor(0x40);
            LcdPrint(" No DS1820 device");
            delay(1000000);
            LcdSetCursor(0x40);
            LcdPrint("                ");
        }
        else
        {
            LcdSetCursor(0x40);
            LcdPrint(" Dallas DS1820  ");
            delay(1000000);
            LcdSetCursor(0x40);
            LcdPrint("                ");
            while (1)
            {
                wire_reset();
                wire_send(0xCC); //Master issues Skip ROM command.
                wire_send(0x44); //Master issues Convert T command.
                delay(750000); //750ms
                if (wire_reset())
                {
                    wire_send(0xCC); //Master issues Skip ROM command.
                    wire_send(0xBE); //Master issues Read Scratchpad command.

                    /* Read Scratchpad command response :
                    byte 0 Temperature LSB (AAh)
                    byte 1 Temperature MSB (00h)
                    byte 2 TH Register or User Byte 1* TH Register or User Byte 1
                    byte 3 TL Register or User Byte 2* TL Register or User Byte 2
                    byte 4 Reserved (FFh)
                    byte 5 Reserved (FFh)
                    byte 6 COUNT REMAIN (0Ch)
                    byte 7 COUNT PER °C (10h)
                    byte 8 CRC*
                    */

                    l = 16; //read temperature
                    temp = 0x0;
                    do
                    {
                        temp |= wire_rx()<<(16-l);
                    }
                    while (--l);

                    l = 4*8; //skip some data
                    do
                    {
                        wire_rx();
                    }
                    while (--l);

                    l = 8; //read count remain
                    remain = 0x0;
                    do
                    {
                        remain |= wire_rx()<<(8-l);
                    }
                    while (--l);

                    if (temp&0x8000) //if temp < 0
                    {
                        temp = ~temp+1;
                        temp_txt[2]='-';
                    }
                    else // temp > 0
                        temp_txt[2]='+';


                    //High Resolution Temperature Measurement
                    t=(temp>>1) - 0.25 + (16-remain)/(float)16;

                    //convert temperature to string
                    l = t/10;
                    temp_txt[3]='0'+l;
                    l = (int)t%10;
                    temp_txt[4]='0'+l;
                    l = (int)(t*10)%10;
                    temp_txt[6]='0'+l;
                    l = (int)(t*100)%10;
                    temp_txt[7]='0'+l;
                    l = (int)(t*1000)%10;
                    temp_txt[8]='0'+l;
                    l = (int)(t*10000)%10;
                    temp_txt[9]='0'+l;

                    LcdSetCursor(0x41);
                    LcdPrint(temp_txt);
                    diode_state = setDiodeState(!diode_state);
                }
                else
                {
                    LcdSetCursor(0x40);
                    LcdPrint(" No DS1820 device");
                    delay(1000000);
                    LcdSetCursor(0x40);
                    LcdPrint("                ");
                }
            }
        }
    }
    return 0;
}
