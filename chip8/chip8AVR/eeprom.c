/*
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

uint8_t * dump_eeprom(uint8_t *addr)
{
    uint16_t c = 0; 
    printf("eeprom:\n");
    for (c=0;c<1024;c++){
        eeprom_busy_wait(); 
        if (c % 16 ==0)
            printf("\n 0x%04x: ",c);
        printf("0x%02x ",eeprom_read_byte(addr));
        addr++;
   }
    printf("\n");
    return addr;
}

uint8_t * dump_progmem(uint8_t *addr)
{    
    uint16_t c = 0; 
    for (c=0;c<(4*1024);c++){
        eeprom_busy_wait(); 
        if (c % 16 ==0)
            printf("\n 0x%04x: ",c);
        printf("0x%02x ",  pgm_read_byte_near(addr));
        addr++;
    }
    printf("\n");
    return addr;
}
