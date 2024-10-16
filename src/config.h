#pragma once

/*
MAIN CONFIG AREA

This contains the pin and build configuration for the CC3501 RP2040 LABS
- Pin numbers and I2C details
- What Tasks should be included
- etc.
*/
/*
Debug Config
Set the Debug Configuration for the build

#define DBG_LOGGING             | Build with logging over UART
*/
#define DBG_LOGGING


#define LED_PIO pio0
#define OUTPUT_LED_PIN 18
#define DEBUG_LED_PIN 19

/*
UART CONFIGURATION

#define UART_ID                 | What UART Interface to use
#define BAUD_RATE               | Baud rate of Serial connection
#define DATA_BITS               | Number of data bits
#define STOP_BITS               | Number of stop bits
#define PARITY                  | UART Parity
*/
#define UART_1_ID uart1
#define UART_1_TX_PIN 24
#define UART_1_RX_PIN 25
#define UART_1_BAUD_RATE 115200
#define UART_1_DATA_BITS 8
#define UART_1_STOP_BITS 1
#define UART_1_PARITY UART_PARITY_NONE

/*
I2C CONFIGURATION

#define I2C_INSTANCE            | What I2C Instance are we using
#define SDA_PIN                 | Define the Serial Data Pin (SDA)
#define SCL_PIN                 | Define the Serial Clock Pin (SCL)
*/
#define I2C_0_INSTANCE i2c0
#define I2C_0_SDA_PIN 12
#define I2C_0_SCL_PIN 13

#define I2C_1_INSTANCE i2c1
#define I2C_1_SDA_PIN 14
#define I2C_1_SCL_PIN 15


#define ADC_0_PIN 26
#define ADC_1_PIN 27
#define ADC_2_PIN 28
#define ADC_3_PIN 29
