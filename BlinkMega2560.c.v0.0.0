#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED      PB7

int main(void)
{
  //
  int i;
  LED_DDR|=_BV(LED);//LED output
  //
  while(1)
  {
    for(i=0;i<5;++i)
      {//burst
	LED_PORT|=_BV(LED);//LED on
	_delay_ms(5000);
	LED_PORT&=~_BV(LED);//LED off
	_delay_ms(5000);
      }
        _delay_ms(1000);
  }
  return (0);
}

