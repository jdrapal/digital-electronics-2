/***********************************************************************
 * 
 *
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 * 
 *
 * Define pins for motor 1 ------------------------------------------*/
#define servo1 PB1 

/* Define pins for motor 2 ---------------------------------------------*/
#define servo2 PB2

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for number conversions

/* Define parameters for servo ---------------------------------------------------------*/
# define M_min 124
# define M_max 260
# define DELAY 1000 // Delay in milliseconds

volatile uint32_t M1_position = M_min;
volatile uint32_t M2_position = M_max;

int main(void)
{
    /* ----------------------------- Timer for SERVO 1 ---------------------------------*/
    // Configure 8-bit Timer/Counter0 to control servo motors
    // Set prescaler to 16 ms and enable overflow interrupt
    TIM0_overflow_16ms();
    TIM0_overflow_interrupt_enable();

    /* ------------------------- Set pins with gpio library ----------------------------*/
    GPIO_mode_output(&DDRB, servo1);        // First servo motor
    GPIO_mode_output(&DDRB, servo2);        // Second servo motor

    // Set 10. waveform generation mode (1010)
    TCCR1A |= (1 << WGM11);                  
    TCCR1B |= (1 << WGM13);

    // Set compare output mode
    TCCR1A |= (1 << COM0A1) | (1 << COM0B1); 

     // Set TOP value
    ICR1 = 1250;
                         
    // Set duty cycle
    OCR1A = M1_position;
    OCR1B = M2_position;
    
    // Set prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10 ); 

    PCICR |= (1<<PCIE0);                    // Any change of any enable PCINT[7:0] pins will cause an interrupt
    PCMSK0 |= (1<<PCINT0);                  // Enable PCINT0 change interrupt  

    // Enables interrupts by setting the global interrupt mask
    sei();
    
    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines, ISRs */
    }

    // Will never reach this
    return 0;

}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 33 ms.
 **********************************************************************/

ISR(TIMER0_OVF_vect)
{
    for(M1_position = M_min; M1_position <= M_max; M1_position += 1)
    {
        OCR1A = M1_position;
    }
    _delay_ms(DELAY); // Wait 1 s

    for(M1_position = M_max; M1_position >= M_min; M1_position -= 1)
    {
        OCR1A = M1_position;
    }
    _delay_ms(DELAY); // Wait 1 s

    for(M2_position = M_min; M2_position <= M_max; M2_position += 1)
    {
        OCR1B = M2_position;
    }
    _delay_ms(DELAY); // Wait 1 s

    for(M2_position = M_max; M2_position >= M_min; M2_position -= 1)
    {
        OCR1B = M2_position;
    }
    _delay_ms(DELAY); // Wait 1 s
}
