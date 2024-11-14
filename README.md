# Embedded Systems Project: Home Monitoring System with Temperature and Fan Control

## Overview
This project involves the design and implementation of a home monitoring system using an ATmega32 microcontroller. The system controls an LED light, a fan, and monitors the temperature using a potentiometer and an LM35 temperature sensor. The LCD is used to display the status of the system and allows interaction through buttons for controlling various aspects.

### Features:
- LED light control via a potentiometer.
- Fan speed control with three levels: slow, medium, fast.
- Temperature control with adjustable levels.
- LCD display for user interaction and information display.

## System Components
- **ATmega32 Microcontroller**
- **LCD (Alphanumeric 16x2)** for displaying information
- **Potentiometer** for adjusting LED brightness
- **LM35 Temperature Sensor** for reading the room temperature
- **Fan Control** via PWM for speed adjustment

## Pin Configuration
- **Port A**: Inputs from buttons and potentiometer
- **Port B**: Controls the fan speed via PWM
- **Port C**: Controls the LCD and interacts with buttons
- **Port D**: Input for temperature sensor

## Code Explanation

```c
#include<stdio.h>
#include<stdlib.h>
#include <mega32.h>
#include <delay.h>
#include <alcd.h>

// Declare global variables
char txt[32];

// ADC Configuration
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

// ADC reading function
unsigned int read_adc(unsigned char adc_input) {
    ADMUX = adc_input | ADC_VREF_TYPE;
    delay_us(10); // Delay for ADC stabilization
    ADCSRA |= (1<<ADSC); // Start ADC conversion
    while ((ADCSRA & (1<<ADIF)) == 0); // Wait for conversion to finish
    ADCSRA |= (1<<ADIF); // Clear the interrupt flag
    return ADCW; // Return ADC result
}

// PWM LED initialization
void PwmLed_init() {
    TCCR2 = (1<<WGM20) | (1<<COM21) | (1<<WGM21) | (1<<CS20); // Configure Timer2
    DDRD |= (1<<DDD7); // Set PD7 as output for LED
}

// PWM Fan initialization
void PwmFAN_init() {
    TCCR0 = (1<<WGM00) | (1<<COM01) | (1<<WGM01) | (1<<CS00) | (1<<CS01); // Configure Timer0
    DDRB |= (1<<DDB3); // Set PB3 as output for FAN
}

// Menu functions for controlling LED, Fan, and Temperature

// LCD update functions for LED, FAN, and TEMP
int LCD_LED_LEVEL(int LedOn, int LAYER_OF_LCD, int PotentiometerInput) {
    if (PINA.4) {
        LAYER_OF_LCD = 0;
    }
    if (LedOn) {
        sprintf(txt, "LED light  %2d                 ", PotentiometerInput);
    } else {
        sprintf(txt, "LED light  %2d                 ", PotentiometerInput);
    }
    lcd_gotoxy(0,0);
    lcd_puts(txt);
    return LAYER_OF_LCD;
}

// Counting pressed buttons for changes
int COUNT_Pressed(int pressedUP) {
    if (PINA.1) {
        pressedUP++;
    } else if (PINA.2) {
        pressedUP--;
    }
    delay_ms(125);
    return abs(pressedUP);
}

// Main function controlling system logic
void main(void) {
    int PotentiometerInput;
    int LedOn;
    int pressedUP = 0;
    int LAYER_OF_LCD = 0;
    int tempreture;
    int TEMP_LEVEL = 30;
    int FAN_LEVEL = 0;

    // I/O Port initializations
    DDRA = 0x00;  // Port A as input
    PORTA = 0x00; // Set default state for Port A

    DDRB = 0x10;  // Set PB3 as output (Fan control)
    PORTB = 0x00; // Set default state for Port B

    DDRC = 0x0F;  // Set Port C as output for LCD
    PORTC = 0x00; // Set default state for Port C

    DDRD = 0x80;  // Set PD7 as output (LED control)
    PORTD = 0x00; // Set default state for Port D

    // Timer and interrupt setups
    TCCR0 = 0x00; // Timer 0 settings
    TIMSK = 0x00; // Disable Timer interrupts
    MCUCR = 0x00; // External interrupt control

    // Main loop to control the system
    while (1) {
        // Monitor and update LCD with system status
        LAYER_OF_LCD = LCD_MENU(pressedUP, LAYER_OF_LCD);
        LAYER_OF_LCD = LCD_LED_LEVEL(LedOn, LAYER_OF_LCD, PotentiometerInput);
        LAYER_OF_LCD = LCD_FAN_SPEED(LAYER_OF_LCD, pressedUP, FAN_LEVEL);
        LAYER_OF_LCD = LCD_CHANGE_TEMP(LAYER_OF_LCD, pressedUP, TEMP_LEVEL, tempreture);
    }
}
```
## Usage
- LED Light Control: Adjust the potentiometer to control the brightness of the LED.
- Fan Speed Control: Press the buttons to select the fan speed (Slow, Medium, or Fast).
- Temperature Control: Use the buttons to set the temperature threshold. The system will show the temperature readings on the LCD.
## Hardware Requirements
- ATmega32 Microcontroller
- 16x2 LCD Display
- LM35 Temperature Sensor
- Potentiometer (for LED control)
- PWM Fan
- Push Buttons for user input
## Wiring
Connect the LM35 sensor to Port A.
Use Port B for fan PWM control.
The LCD is connected to Port C.
Use Port D to control the LED.
