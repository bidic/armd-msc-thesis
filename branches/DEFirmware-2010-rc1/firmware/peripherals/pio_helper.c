/*
 * pio_helper.c
 *
 *  Created on: Apr 27, 2011
 *      Author: lhanusiak
 */

#include "pio_helper.h"

static const Pin pin_cam_reset = PIN_CAM_RESET;
static const Pin pin_uart_rx = PIN_UART_RX;
static const Pin pin_uart_tx = PIN_UART_TX;
static const Pin pin_cam_d0 = PIN_CAM_D0;
static const Pin pin_cam_d1 = PIN_CAM_D1;
static const Pin pin_cam_d2 = PIN_CAM_D2;
static const Pin pin_cam_d3 = PIN_CAM_D3;
static const Pin pin_cam_d4 = PIN_CAM_D4;
static const Pin pin_cam_d5 = PIN_CAM_D5;
static const Pin pin_cam_d6 = PIN_CAM_D6;
static const Pin pin_cam_d7 = PIN_CAM_D7;
static const Pin pin_hsync = PIN_HSYNC;
static const Pin pin_vsync = PIN_VSYNC;
static const Pin pin_dioda1 = PIN_DIODA1;
static const Pin pin_dioda2 = PIN_DIODA2;
static const Pin pin_mclk = PIN_MCLK;
static const Pin pin_pwm1 = PIN_PWM1;
static const Pin pin_pwm2 = PIN_PWM2;
static const Pin pin_pwm3 = PIN_PWM3;
static const Pin pin_pwm4 = PIN_PWM4;
static const Pin pin_in1 = PIN_IN1;
static const Pin pin_in2 = PIN_IN2;
static const Pin pin_in3 = PIN_IN3;
static const Pin pin_in4 = PIN_IN4;
static const Pin pin_serwo_pwm = PIN_SERWO_PWM;
static const Pin pin_led_power = PIN_LED_POWER;
static const Pin pin_serwo_power = PIN_SERWO_POWER;
static const Pin pin_i2c_data = PIN_I2C_DATA;
static const Pin pin_i2c_clock = PIN_I2C_CLOCK;
static const Pin pin_cd4053b_ctrl = PIN_CD4053B_CTRL;

static const Pin de_pins[] = { PINS_TWI, PIN_CAM_RESET, PIN_UART_RX, PIN_UART_TX,
		PIN_CAM_D0, PIN_CAM_D1, PIN_CAM_D2, PIN_CAM_D3, PIN_CAM_D4, PIN_CAM_D5,
		PIN_CAM_D6, PIN_CAM_D7, PIN_HSYNC, PIN_VSYNC, PIN_DIODA1, PIN_DIODA2,
		PIN_MCLK, PIN_PWM1, PIN_PWM2, PIN_PWM3, PIN_PWM4, PIN_IN1, PIN_IN2,
		PIN_IN3, PIN_IN4, PIN_SERWO_PWM, PIN_LED_POWER, PIN_SERWO_POWER,
		PIN_I2C_DATA, PIN_I2C_CLOCK, PIN_CD4053B_CTRL };
