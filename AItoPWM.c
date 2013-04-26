/* Date: 26/04/2013 
 *
 * Author: Caroline Braud
 *
 * AI to PWM or 
 * How to drive DC of a square wave signal from Analog Input Signal 
 * Frequency is fixe to 10 Hz (T=0.1s)
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "util/util.h"

#define delay(Time)  {\               
int k;					\
float LoopMinDelay= 4./F_CPU;\
float LoopMaxDelay= 65535.*LoopMinDelay;\
int NBigLoop=Time/LoopMaxDelay;\
float Remain=Time - NBigLoop*LoopMaxDelay;\
int NSmallLoop=Remain/LoopMinDelay;\
for(k=0;k<NBigLoop;++k) {_delay_loop_2(65535);}\                            
_delay_loop_2(NSmallLoop);\                           
g}

static void set_pin_pb7_for_output (uint8_t initial_value)
{
  DDRB |= _BV (DDB7);
  loop_until_bit_is_set (DDRB, DDB7);

  if ( initial_value == HIGH ) 
    {
      PORTB |= _BV (PORTB7);
      loop_until_bit_is_set (PORTB, PORTB7);
    }
  else 
    {
      PORTB &= ~(_BV (PORTB7));
      loop_until_bit_is_clear (PORTB, PORTB7);
    }
}

static void set_pin_ph3_for_output (uint8_t initial_value)
{
  DDRH |= _BV (DDH3);
  loop_until_bit_is_set (DDRH, DDH3);

  if ( initial_value == HIGH ) {
    PORTH |= _BV (PORTH3);
    loop_until_bit_is_set (PORTH, PORTH3);
  }
  else {
    PORTH &= ~(_BV (PORTH3));
    loop_until_bit_is_clear (PORTH, PORTH3);
  }
}

static void set_pin_pb7 (uint8_t value)
{
  if ( value == HIGH ) {
    PORTB |= _BV (PORTB7);
    loop_until_bit_is_set (PORTB, PORTB7);
  }
  else {
    PORTB &= ~(_BV (PORTB7));
    loop_until_bit_is_clear (PORTB, PORTB7);
  }
}

static void set_pin_ph3 (uint8_t value)
{
  if ( value == HIGH ) {
    PORTH |= _BV (PORTH3);
    loop_until_bit_is_set (PORTH, PORTH3);
  }
  else {
    PORTH &= ~(_BV (PORTH3));
    loop_until_bit_is_clear (PORTH, PORTH3);
  }
}

static void initialize_adc (void)
{
  // Internal pull-ups interfere with the ADC. disable the pull-up on the
  // pin ifit's being used for ADC. either writing 0 to the port register
  // or setting it to output should be enough to disable pull-ups.
  DDRC = 0x00;

  // Unless otherwise configured, arduinos use the internal Vcc reference. MUX
  // 0x0f samples the ground (0.0V) (we'll change this before each actual
  // ADC read).
  ADMUX = _BV (REFS0) | 0x0f;
  
  // Enable the ADC system, use 128 as the clock divider on a 16MHz arduino
  // (ADC needs a 50 - 200kHz clock) and start a sample.  The AVR needs to
  // do some set-up the first time the ADC is used; this first, discarded,
  // sample primes the system for later use.
  ADCSRA |= _BV (ADEN) | _BV (ADPS2) | _BV (ADPS1) | _BV (ADPS0) | _BV (ADSC);
  
  // Wait for the ADC to return a sample.
  loop_until_bit_is_clear (ADCSRA, ADSC);
}

static unsigned short adc_read (unsigned char pin)
{
  unsigned char l, h, r;

  r = (ADMUX & 0xf0) | (pin & 0x0f);

  // Select the input channel.
  ADMUX = r;
  ADCSRA |= _BV (ADSC);
  loop_until_bit_is_clear (ADCSRA, ADSC);

  // It is required to read the low ADC byte before the high byte.
  l = ADCL;
  h = ADCH;

  return ((unsigned short)h << 8) | l;
}


int main(void)
{

  // set pin PB7 for output
  set_pin_pb7_for_output (HIGH);
  // set pin PH3 for output
  set_pin_ph3_for_output (HIGH);

  // Declarations
  short raw;
  const uint16_t a2d_steps = 1024;
  const float supply_voltage = 5.0;

  // Preparation of AI acquisition
  initialize_adc ();


  while(1)
  {
    // Read Analog Input Channel A1
    raw = adc_read (1);
    // Convert integer into voltage (10bit of resolution for AI of Atmega2560)
    float tap_voltage = ((float) raw / a2d_steps) * supply_voltage;
    
    // DC (0-1) is proportionnal to Analog Input Voltage (0-5 Volt)
    float DC=tap_voltage/5.;
    // Fixed period of square wave signal 
    float T=0.1;
    // Computation of delays th and tb (th+tb=T)
    float th=DC*T;
    float tb=T-th;

    // Blinking LED + TTL signal (DC proportionnal to AI)
	set_pin_pb7 (HIGH);
	set_pin_ph3 (HIGH);
	delay(th);
	set_pin_pb7 (LOW);
	set_pin_ph3 (LOW);
	delay(tb);

  }

  return (0);
}
