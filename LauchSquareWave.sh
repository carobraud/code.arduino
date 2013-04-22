#!/bin/bash


F_CPU=16000000.
#F_CPU=$1
#definition square wave signal:
DC=0.5 #50% of Duty Cycle
DC=$1
f=10.   #10 Hz
f=$2

#Compute the biggest delay of the square wave signal
Th=`echo "scale=14; $DC/$f" |bc`
#Compute the smallest delay of the square wave signal
Tb=`echo "scale=14; (1.0/$f)-$Th" |bc`

# Compute Min/Max delay available with _delay_loop_2(uint16_t) function (uint16_t => from 1 to 65535) 
LoopMinDelay=`echo "scale=14; 4./$F_CPU" | bc` # _delay_loop_2(1)=LoopMinDelay = 4 microsecondes if F_CPU=16000000. 
LoopMaxDelay=`echo "scale=14;65535.*($LoopMinDelay)" | bc`  # _delay_loop_2(65535)=LoopMaxDelay = 0.01638 secondes if F_CPU=16000000. 

## COMPUTE DELAY FOR Th
# Compute number of _delay_loop_2(65535) needed to get BigDelay
NBigDelayTh=`echo "$Th/$LoopMaxDelay" | bc`    
# Compute remaining delay 
RemainDelay=`echo "scale=14;$Th-$NBigDelayTh*$LoopMaxDelay" | bc`
# Compute number of _delay_loop_2(1) needed to get RemainDelay
NSmallDelayTh=`echo "$RemainDelay/$LoopMinDelay" | bc`    

## COMPUTE DELAY FOR Tb
# Compute number of _delay_loop_2(65535) needed to get BigDelay
NBigDelayTb=`echo "$Tb/$LoopMaxDelay" | bc`    
# Compute remaining delay 
RemainDelay=`echo "scale=14;$Tb-$NBigDelayTb*$LoopMaxDelay" | bc`
# Compute number of _delay_loop_2(1) needed to get RemainDelay
NSmallDelayTb=`echo "$RemainDelay/$LoopMinDelay" | bc`    


#Clean
make clean;

#Compil
avr-gcc -c -mmcu=atmega1280 -Os -g -Wall -I. -DF_CPU=$F_CPU GenerateSquareWare.c -o GenerateSquareWare.o -D NBigDelayTh=$NBigDelayTh -D NSmallDelayTh=$NSmallDelayTh -D NBigDelayTb=$NBigDelayTb -D NSmallDelayTb=$NSmallDelayTb;

#Upload and size
make upload ; make size;



