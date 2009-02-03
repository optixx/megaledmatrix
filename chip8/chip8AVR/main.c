/*
 *  untitled.h
 *  Chip8AVR
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <avr/io.h>  
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "chip8.h"
#include "cpu.h"
#include "memory.h"

extern FILE uart_stdout;

uint16_t ee_data01 __attribute__((section(".eeprom"))) = 0x01;
uint16_t ee_data02 __attribute__((section(".eeprom"))) = 0x02;
uint16_t ee_data03 __attribute__((section(".eeprom"))) = 0x03;
uint16_t ee_data04 __attribute__((section(".eeprom"))) = 0x04;
volatile uint8_t cnt_timer;   

SIGNAL(SIG_OVERFLOW2)
{
    cnt_timer++;
    TCNT2 = 183;
}

inline void _sleep (uint8_t t)
{
    cnt_timer=0;
    while (cnt_timer < t);
}
        
 
inline void sleep(uint16_t msec)
{
    uint16_t i;
    for(i=0; i<msec; i++) {
        _sleep(100);
    }
}

void timer_init(void)
{
    TCCR2 = (1<<CS22);          
    TCNT2  = 183;           
    TIMSK |= (1<<TOIE2);     
    sei();
}




uint8_t main( int argc, char** argv ){
	
    timer_init();
    uart_init();
    stdout = &uart_stdout;
    
    printf("Basic init...done\n");
    printf("FB init...done\n");
	
	memory_init();
    printf("Memory init ...done\n");
	memory_load_builtin();
    printf("Load Rom...done\n");
	cpu_int();
    printf("Memory init...done\n");
	cpu_int();
    printf("Cpu init...done\n");
 	
    while(1){
		if (cpu())
			sleep(50);
		if (dtime>0)dtime-=1;
			if (s_time>0)s_time-=1;
	}
	return 0;
}
