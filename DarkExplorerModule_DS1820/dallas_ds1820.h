#define PIO_PA8 0x1 << 8

/*
  Function : enable 1-wire pin
  Arguments : none
  Returns : none
*/
void wire_init(void);

/*
  Function : sets 1-wire pin as output
  Arguments : none
  Returns : none
*/
void wire_out(void);

/*
  Function : sets 1-wire pin as input
  Arguments : none
  Returns : none
*/
void wire_in(void);

/*
  Function : 1-wire bus reset
  Arguments : none
  Returns : 
    '0' - there is no 1-wire device on the bus
    '1' - there is at least one 1-wire device on the bus
*/
unsigned char wire_reset(void);

/*
  Function : transfer 1 bit of data
  Arguments : 
    bit - data to be sent
  Returns : none
*/
void wire_tx(unsigned char bit);

/*
  Function : send 8bit set of data
  Arguments :
    data - data to be sent
  Returns : none
*/
void wire_send(unsigned char data);

/*
  Function : receives 1 bit of data
  Arguments : none
  Returns :
    received one bit of data
*/
unsigned char wire_rx(void);
