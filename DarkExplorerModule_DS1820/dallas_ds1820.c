#include "AT91SAM7S256.h"
#include "lib_AT91SAM7S256.h"
#include "dallas_ds1820.h"
#include "delay.h"

#define WIRE(bit) if (bit) AT91F_PIO_SetOutput(AT91C_BASE_PIOA, PIO_PA8); else AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, PIO_PA8);

void wire_init(void)
{
    // PIN as output
    AT91F_PIO_Enable(AT91C_BASE_PIOA, PIO_PA8);
    wire_out();
}

void wire_out(void)
{
    // PIN as output
    AT91F_PIO_OutputEnable(AT91C_BASE_PIOA, PIO_PA8);
    AT91F_PIO_SetOutput(AT91C_BASE_PIOA, PIO_PA8);
}

void wire_in(void)
{
    // PIN as input
    AT91F_PIO_OutputDisable(AT91C_BASE_PIOA, PIO_PA8);
}

unsigned char wire_reset(void)
{
    unsigned short int present;

    wire_out();
    WIRE(0);
    delay(700);
    WIRE(1);
    wire_in();
    delay(80);
    present = AT91F_PIO_IsInputSet(AT91C_BASE_PIOA, PIO_PA8);
    delay(400);
    wire_out();

    return !present;
}

void wire_tx(unsigned char bit)
{
    wire_out();
    WIRE(0);

    if (bit&1)
    {
        delay(10);
        WIRE(1);
        delay(80);
    }
    else
    {
        delay(80);
        WIRE(1);
        delay(10);
    }
}

void wire_send(unsigned char data)
{
    unsigned char l;
    l = 8;

    do
    {
        wire_tx(data&1);
        data>>=1;
    }
    while (--l);
}

unsigned char wire_rx(void)
{
    unsigned short int bit;

    wire_out();
    delay(50);
    WIRE(0);
    delay(10);
    WIRE(1);
    wire_in();
    delay(10);

    bit = AT91F_PIO_IsInputSet(AT91C_BASE_PIOA, PIO_PA8);
    if (!bit)
    {
        delay(100);
    }
    wire_out();
    return (bit)?1:0;
}
