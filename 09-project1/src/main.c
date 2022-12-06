/***********************************************************************
 * 
 * Stopwatch by Timer/Counter2 on the Liquid Crystal Display (LCD)
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2017 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * Components:
 *   16x2 character LCD with parallel interface
 *     VSS  - GND (Power supply ground)
 *     VDD  - +5V (Positive power supply)
 *     Vo   - (Contrast)
 *     RS   - PB0 (Register Select: High for Data transfer, Low for Instruction transfer)
 *     RW   - GND (Read/Write signal: High for Read mode, Low for Write mode)
 *     E    - PB1 (Read/Write Enable: High for Read, falling edge writes data to LCD)
 *     D3:0 - NC (Data bits 3..0, Not Connected)
 *     D4   - PD4 (Data bit 4)
 *     D5   - PD5 (Data bit 5)
 *     D6   - PD6 (Data bit 3)
 *     D7   - PD7 (Data bit 2)
 *     A+K  - Back-light enabled/disabled by PB2
 * 
 **********************************************************************/
/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

/* Defines ----------------------------------------------------------*/ 
#define VRX PC0     //PC0 is pin where joystick x axis is connected
#define VRY PC1     //PC1 is pin where joystick y axis is connected
#define CLK PC2     //PC2 is pin where Rotary-encoder CLK is connected
#define EN PD1      //PD1 is pin where rotary-encoder EN is connected
#define DT PD3      //PD3 is pin where rotary-encoder DT is connected


uint16_t value = 0;
int8_t value2 = 0;
int8_t value3 = 0;
uint8_t CSEN, CSCLK, LSCLK, LSEN;
/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{   
    // Initialize display
    lcd_init(LCD_DISP_ON);

    lcd_gotoxy(0, 0);
    lcd_puts("ec");
    lcd_gotoxy(2, 0);
    lcd_puts("0");

    lcd_gotoxy(8, 0);
    lcd_puts("swc");
    lcd_gotoxy(10, 0);
    lcd_puts("0");

    GPIO_mode_input_pullup(&DDRD, EN);
    GPIO_mode_input_nopull(&DDRD, DT);
    GPIO_mode_input_nopull(&DDRC, CLK);

    LSCLK = GPIO_read(&PINC, CLK);
    //LSEN = GPIO_read(&PINB, EN);

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"

    ADMUX |= (1<<REFS0);
    ADMUX &= ~(1<<REFS1);

    // Enable ADC module
    ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);


    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 16 ms and enable overflow interrupt    
    TIM1_overflow_4ms();
    TIM1_overflow_interrupt_enable();

    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    if(value == 0)
    {
        ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3)); 
    }
    else if(value == 1)
    {
        ADMUX &= ~((1<<MUX1) | (1<<MUX2) | (1<<MUX3)); ADMUX |= (1<<MUX0);
    }
}
ISR(TIMER2_OVF_vect)
{
    ADCSRA |= (1<<ADSC);
    // Rotary encoder
    char string[4];

    CSCLK = GPIO_read(&PINC, CLK);

    if(CSCLK != LSCLK)
    {
        if(GPIO_read(&PIND, DT) == CSCLK)
        {
            value3++;
        }
        else
        {
            value3--;
        }
    itoa(value3, string, 10);
    lcd_gotoxy(2, 0);
    lcd_puts("     ");
    lcd_gotoxy(2, 0);
    lcd_puts(string);
    }
    LSCLK = CSCLK;


    CSEN = GPIO_read(&PIND, EN);

    if(CSEN != LSEN)
    {
        if(GPIO_read(&PIND, EN) == CSEN && CSEN == 1)
        {
            if(value2<10)
            {
                value2++;
            }
            else
            {
                value2 = 0;
            }
        }
        itoa(value2, string, 10);
        lcd_gotoxy(10, 0);
        lcd_puts("  ");
        lcd_gotoxy(10, 0);
        lcd_puts(string);
    }
    LSEN = CSEN;

    
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    static uint16_t xValue;
    static uint16_t yValue;
    char string[4];  // String for converted numbers by itoa()

    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    if (value == 0)
    {
        xValue = ADC;
        value = 1;
    }
    else if (value == 1)
    {
        
        yValue = ADC;
        value = 0;
    }

    lcd_gotoxy(0,1);
    lcd_puts("x");
    itoa(xValue, string, 10);
    lcd_gotoxy(1, 1);
    lcd_puts("    ");
    lcd_gotoxy(1, 1);
    lcd_puts(string);

    lcd_gotoxy(6,1);
    lcd_puts("y");
    itoa(yValue, string, 10);
    lcd_gotoxy(7, 1);
    lcd_puts("    ");
    lcd_gotoxy(7, 1);
    lcd_puts(string);

    if (xValue < 100)
    {

        lcd_gotoxy(11, 1);
        lcd_puts("     ");
        lcd_gotoxy(11,1);
        lcd_puts("LEFT ");
    }
    else if (xValue > 900)
    {

        lcd_gotoxy(11, 1);
        lcd_puts("     ");
        lcd_gotoxy(11,1);
        lcd_puts("RIGHT");
    }
    else if (yValue < 100)
    {

        lcd_gotoxy(11, 1);
        lcd_puts("     ");
        lcd_gotoxy(11,1);
        lcd_puts("DOWN ");
    }
    else if (yValue > 900)
    {

        lcd_gotoxy(11, 1);
        lcd_puts("     ");
        lcd_gotoxy(11,1);
        lcd_puts("UP   ");
    }
    else
    {
        lcd_gotoxy(11,1);
        lcd_puts("NONE ");
    }
}