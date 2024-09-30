#pragma once

/*
MAIN CONFIG AREA

This contains the pin and build configuration for the CC3501 RP2040 LABS
- Pin numbers and I2C details
- What Tasks should be included
- etc.
*/


// Pin Config

#define LED_PIN 14
#define TASK_PIN 15
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Debug Config

/*
Set the Debug Configuration for the build

#define DBG_LOGGING             | Build with logging over UART
*/

#define DBG_LOGGING

// Tasks

/*
Define the tasks that we want to run in our build. We can have the following:

#define ACCEL_TASK              | Build with the acceleration task
#define MICROPHONE_TASK         | Build with the microphone task
#define BLUETOOTH_TASK          | Build with the bluetooth task

*/

// #define LED_TASK
// #define ACCEL_TASK
#define MICROPHONE_TASK
// #define BLUETOOTH_TASK

// Task config

/*
LED TASK

#define NUMBER_OF_LEDS          | Number of LEDs on the board
#define LED_PIO                 | What PIO to use, usually `pio0` or `pio1`
#define DBG_LED                 | Use the LED task in debug mode, logs changes and updates
*/

#define NUMBER_OF_LEDS 12
#define LED_PIO pio0
// #define DBG_LED

/*
ACCELEROMETER TASK


#define DBG_ACCEL               | Log Accelerometer Debug Messages
*/

// #define DBG_ACCEL
#define I2C_ADDRESS 0x19
#define START_ADDR 0x28
#define SPIRIT_LEVEL_THRESHOLD 0.1
#define WHO_AM_I 0x33
#define ACCEL_SCALING 0.004

/*
MICROPHONE TASK
*/

#define DBG_MICROPHONE
#define ADC_CLK_DIV 1087

/*
UART CONFIGURATION

#define UART_ID                 | What UART Interface to use
#define BAUD_RATE               | Baud rate of Serial connection
#define DATA_BITS               | Number of data bits
#define STOP_BITS               | Number of stop bits
#define PARITY                  | UART Parity
*/

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

/*
I2C CONFIGURATION

#define I2C_INSTANCE            | What I2C Instance are we using
#define SDA_PIN                 | Define the Serial Data Pin (SDA)
#define SCL_PIN                 | Define the Serial Clock Pin (SCL)

*/

#define I2C_INSTANCE i2c0
#define SDA_PIN 16
#define SCL_PIN 17