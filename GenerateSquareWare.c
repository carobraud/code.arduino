#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED      PB7

#define delay(Nbig,Nsmall)  {			\
int k; \
for(k=0;k<Nbig;++k) {_delay_loop_2(65535);}				\
_delay_loop_2(Nsmall);							\
}				

int main(void)
{
  //
  int i;
  LED_DDR|=_BV(LED);//LED output
  //
  while(1)
  {
    for(i=0;i<5;++i)
      {//LED on during Th seconds
	LED_PORT|=_BV(LED);
	delay(NBigDelayTh,NSmallDelayTh);
	//delay(3,3395);
       	//_delay_ms(50);

	//LED off during Tb seconds
	LED_PORT&=~_BV(LED);
	//delay(3,3395);
	delay(NBigDelayTb,NSmallDelayTb);
	//_delay_ms(50);

      }
        _delay_ms(1000);
  }
  return (0);
}
