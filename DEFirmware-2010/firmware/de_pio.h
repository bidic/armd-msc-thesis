#ifndef DE_PIO_H
#define DE_PIO_H

/* wyjscia procesora */
#define PIO_PA0     0x1 << 0
#define PIO_PA1     0x1 << 1
#define PIO_PA2     0x1 << 2
#define PIO_PA3     0x1 << 3
#define PIO_PA4     0x1 << 4
#define PIO_PA5     0x1 << 5
#define PIO_PA6     0x1 << 6
#define PIO_PA7     0x1 << 7
#define PIO_PA8     0x1 << 8
#define PIO_PA9     0x1 << 9
#define PIO_PA10    0x1 << 10
#define PIO_PA11    0x1 << 11
#define PIO_PA12    0x1 << 12
#define PIO_PA13    0x1 << 13
#define PIO_PA14    0x1 << 14
#define PIO_PA15    0x1 << 15
#define PIO_PA16    0x1 << 16
#define PIO_PA17    0x1 << 17
#define PIO_PA18    0x1 << 18
#define PIO_PA19    0x1 << 19
#define PIO_PA20    0x1 << 20
#define PIO_PA21    0x1 << 21
#define PIO_PA22    0x1 << 22
#define PIO_PA23    0x1 << 23
#define PIO_PA24    0x1 << 24
#define PIO_PA25    0x1 << 25
#define PIO_PA26    0x1 << 26
#define PIO_PA27    0x1 << 27
#define PIO_PA28    0x1 << 28
#define PIO_PA29    0x1 << 29
#define PIO_PA30    0x1 << 30
#define PIO_PA31    0x1 << 31

/* funkcje wyj�c - plytka */
#define SERWO_PWM   PIO_PA0
#define LED_POWER   PIO_PA1
#define SERWO_POWER PIO_PA2
#define I2C_DATA    PIO_PA3
#define I2C_CLOCK   PIO_PA4
#define UART_RX     PIO_PA5
#define UART_TX     PIO_PA6
#define IN1         PIO_PA7
#define IN2         PIO_PA8
#define IN3         PIO_PA9
#define IN4         PIO_PA10
#define PWM1        PIO_PA11
#define PWM2        PIO_PA12
#define PWM3        PIO_PA13
#define PWM4        PIO_PA14
#define VSYNC       PIO_PA25
#define HSYNC       PIO_PA26
#define MCLK        PIO_PA21
#define CAM_D7      PIO_PA27
#define CAM_D6      PIO_PA20
#define CAM_D5      PIO_PA19
#define CAM_D4      PIO_PA18
#define CAM_D3      PIO_PA17
#define CAM_D2      PIO_PA16
#define CAM_D1      PIO_PA22
#define CAM_D0      PIO_PA24
#define CAM_RESET   PIO_PA23

/* wolne wyjscia PIO */
//2 diodki sygnalizacyjne
#define DIODA1      PIO_PA28
#define DIODA2      PIO_PA29
//nieuzywane
//PIO_PA15 PIO_PA30 PIO_PA31

#endif /* DE_PIO_H */
