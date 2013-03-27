#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define LED_DDR  DDRB
#define LED_PORT PORTB
#define TTL_DDR  DDRD
#define TTL_PORT PORTD
#define LED      PB5
#define TTL      PD7

int main(void)
{
  //
  int i;
  LED_DDR|=_BV(LED);//LED output
  TTL_DDR&=~_BV(TTL);//TTL input
  //
  while(1)
  {
    loop_until_bit_is_set(PINB,TTL); //wait for PIV synchronization
    for(i=0;i<5;++i)
      {//burst
	LED_PORT|=_BV(LED);//LED on
	_delay_ms(500);
	LED_PORT&=~_BV(LED);//LED off
	_delay_ms(100);
      }
    //    _delay_ms(1000);
  }
  return (0);
}

