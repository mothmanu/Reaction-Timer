/* 
 * File:   main.c
 * Authors: Abraham Tesfazgi, Damola Akinola, Obi, Vanja
 *
 * Created on December 1, 2022, 7:28 PM
 */
#define F_CPU 14745600UL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <time.h>
#include "lcd.h"
#include "hd44780.h"
#include "defines.h"
/*
 * 
 */

FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

float TimerConvert(uint32_t counts);
void Buzz(float num);

volatile uint16_t t1, t2,ovf_count;
volatile uint32_t period_count;
volatile bool restart = true;
volatile float score = -1;

ISR(TIMER1_CAPT_vect, ISR_BLOCK) {
    // Immediately prevent input capture from re-occuring
    TIMSK1 &= ~(1 << ICIE1);
    t2 = ICR1;
    period_count = (ovf_count*65536 + t2) - t1;
    
    // Stop light
    PORTD &= ~((1<<PD1) | (1<<PD2) | (1<<PD3));
  
    float result = TimerConvert(period_count);
    hd44780_outcmd(1); //clear screen
    hd44780_wait_ready(true);
    
    if(result < score || score == -1){
        score = result;
    }
    fprintf(&lcd_str, "%.3fs",result);
    fprintf(&lcd_str, "\x1b\xc0\Best: %.3fs",score);
    Buzz(result);
    _delay_ms(5000);
    
    // Stop light
    PORTD &=~ (1<<PD1);
    PORTD &=~ (1<<PD2);
    
    restart = true;
}

// Increment the overflow count
ISR(TIMER1_OVF_vect,ISR_BLOCK) {
    ovf_count++;
    
    if(ovf_count >= 1575){  // If user waits too long (7s))
        
        //TIMSK1 &= ~(1 << TOIE1);
        ovf_count = 0;
        TIMSK1 &= ~(1 << ICIE1);
        hd44780_outcmd(1); //clear screen
        fprintf(&lcd_str, "\x1b\x80You waited\x1b\xc0too long");
        PORTD |= (1<<PD1);
        
        // Display losing sound
        Buzz(7);

        _delay_ms(3500);
        PORTD &= ~(1<<PD1);
        restart = true;
    }
}


// Functions that use loops to allow for variable delays
void new_delay_us(int delay) {
    for (int i = 0; i < delay / 10; i++) {
        _delay_us(10);
    }
}

void MakeSound(float duration_ms, int delay_us) {
    // Repeat loop as many times as it takes to reach the time (duration_ms)
    for (int i = 0; i < duration_ms / (2 * (double) delay_us / 1000); i++) {
        PORTD |= (1 << PD0); //set to high

        new_delay_us(delay_us);

        PORTD &= ~(1 << PD0); //set to low

        new_delay_us(delay_us);
    }
}

float TimerConvert(uint32_t counts){
    float period_sec = (float)counts / F_CPU; //converts period in cycles to seconds
    return period_sec; 
}

void Buzz(float num){
    
    if (num > 0.3){
        PORTD |= (1<<PD1);
        
        // Game Over Theme
        MakeSound(62.5*2.5, 1012);
        _delay_us(15625*2.5);
        MakeSound(93.75*2.5, 716);
        _delay_us(62500*2.5);
        MakeSound(62.5*2.5, 716);
        MakeSound(125*2.5, 716);
        MakeSound(125*2.5, 758);
        MakeSound(125*2.5, 851);
        MakeSound(62.5*2.5, 956);
        MakeSound(62.5*2.5, 1517);
        MakeSound(62.5*2.5, 2551);
        MakeSound(62.5*2.5, 1517);
        MakeSound(250*2.5, 1911);
    
        /*MakeSound(500, 2000);
        MakeSound(500, 3000);
        MakeSound(500, 4000);
        MakeSound(500, 5000);*/

    } else {
        PORTD |= (1<<PD2);
        
        // Victory Theme
        MakeSound(125, 1517/2);
        MakeSound(125, 1276/2);
        MakeSound(125, 758/2);
        MakeSound(125, 956/2);
        MakeSound(125, 851/2);
        MakeSound(125, 638/2);
        
        /*MakeSound(300, 400);
        MakeSound(300, 200);
        MakeSound(300, 300);
        MakeSound(300, 200);*/
    }    
    
}
int main(int argc, char** argv) {
    lcd_init();
    

    DDRD |= (1 << PD0); //buzzer initialized as output 
    DDRB &= ~(1 << PB0); // button initialized as input
    PORTB |= (1 << PB0); // enable pull-up (connected to positive rail)
    
    DDRD |= (1 << PD1); // LED initialized as output
    DDRD |= (1 << PD2); // green LED
    DDRD |= (1 << PD3); // blue LED
    
    PORTD &=~ (1<<PD1); //set pins low initially
    PORTD &=~ (1<<PD2);
    PORTD &=~ (1<<PD3);
    
    // Enable input capture & interrupt
    TIMSK1 |= (1 << TOIE1);
    TIMSK1 &= ~(1 << ICIE1); // Disable IC interrupt in the beginning
    TCCR1B &= ~(1 << ICES1);
    TCCR1B |= (1 << CS10);

    while(1){
        cli();
        restart = false;

        hd44780_outcmd(1); //clear screen
        hd44780_wait_ready(true);

        fprintf(&lcd_str, "Press To Start"); //prepare user for reaction timer 
        
        if(score == -1){
            fprintf(&lcd_str, "\x1b\xc0\Best: NaN"); //displays nothing if user hasn't played yet
        }else{
            fprintf(&lcd_str, "\x1b\xc0\Best: %.3fs",score); //displays the user's highest score
        }
        
        while(PINB & (1<<PB0));
        while(!(PINB & (1<<PB0)));

        srand(TCNT1); // Use clock time as a random seed and produce random number
        int waitTime = rand() % 5000 + 2000; // Generate delay from 2-7 seconds

        hd44780_outcmd(1); //clear screen
        hd44780_wait_ready(true);
        
        fprintf(&lcd_str, "Get Ready!");
        fprintf(&lcd_str, "\x1b\xc0(Aim for 0.3s)");
        
        
        for (int i = 0; i < waitTime; i++) {
            if(!(PINB & (1<<PB0))){
                break;
            }
            _delay_ms(1);
        }
        if(!(PINB & (1<<PB0))){
            hd44780_outcmd(1); //clear screen
            hd44780_wait_ready(true);
            fprintf(&lcd_str, "\x1b\x80Wait for the LED");
            fprintf(&lcd_str, "\x1b\xc0You Stupid!");
            
            PORTD |= (1<<PD1);
            MakeSound(500, 4000);
            
            _delay_ms(3500);
            PORTD &= ~(1<<PD1);
            continue;
        }
      
       
        TIFR1 |= (1 << ICF1);
        // Re-enable input capture interrupt
        TIMSK1 |= (1 << ICIE1);
        sei();
        
        // Flash
        PORTD |= (1<<PD1) | (1<<PD2) | (1<<PD3);
        t1 = TCNT1;     //initial time
        ovf_count = 0;
        _delay_ms(200);
        PORTD &= ~((1<<PD1) | (1<<PD2) | (1<<PD3));
        
        while(!restart); // This is set to true after the interrupt is finished
    }
    
    return (EXIT_SUCCESS);
}

