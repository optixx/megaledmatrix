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
#include <avr/pgmspace.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"

unsigned char game_pong[] PROGMEM = {
0x6a, 0x02, 0x6b, 0x0c, 0x6c, 0x3f, 0x6d, 0x0c, 0xa2, 0xea, 0xda, 0xb6,
0xdc, 0xd6, 0x6e, 0x00, 0x22, 0xd4, 0x66, 0x03, 0x68, 0x02, 0x60, 0x60,
0xf0, 0x15, 0xf0, 0x07, 0x30, 0x00, 0x12, 0x1a, 0xc7, 0x17, 0x77, 0x08,
0x69, 0xff, 0xa2, 0xf0, 0xd6, 0x71, 0xa2, 0xea, 0xda, 0xb6, 0xdc, 0xd6,
0x60, 0x01, 0xe0, 0xa1, 0x7b, 0xfe, 0x60, 0x04, 0xe0, 0xa1, 0x7b, 0x02,
0x60, 0x1f, 0x8b, 0x02, 0xda, 0xb6, 0x8d, 0x70, 0xc0, 0x0a, 0x7d, 0xfe,
0x40, 0x00, 0x7d, 0x02, 0x60, 0x00, 0x60, 0x1f, 0x8d, 0x02, 0xdc, 0xd6,
0xa2, 0xf0, 0xd6, 0x71, 0x86, 0x84, 0x87, 0x94, 0x60, 0x3f, 0x86, 0x02,
0x61, 0x1f, 0x87, 0x12, 0x46, 0x02, 0x12, 0x78, 0x46, 0x3f, 0x12, 0x82,
0x47, 0x1f, 0x69, 0xff, 0x47, 0x00, 0x69, 0x01, 0xd6, 0x71, 0x12, 0x2a,
0x68, 0x02, 0x63, 0x01, 0x80, 0x70, 0x80, 0xb5, 0x12, 0x8a, 0x68, 0xfe,
0x63, 0x0a, 0x80, 0x70, 0x80, 0xd5, 0x3f, 0x01, 0x12, 0xa2, 0x61, 0x02,
0x80, 0x15, 0x3f, 0x01, 0x12, 0xba, 0x80, 0x15, 0x3f, 0x01, 0x12, 0xc8,
0x80, 0x15, 0x3f, 0x01, 0x12, 0xc2, 0x60, 0x20, 0xf0, 0x18, 0x22, 0xd4,
0x8e, 0x34, 0x22, 0xd4, 0x66, 0x3e, 0x33, 0x01, 0x66, 0x03, 0x68, 0xfe,
0x33, 0x01, 0x68, 0x02, 0x12, 0x16, 0x79, 0xff, 0x49, 0xfe, 0x69, 0xff,
0x12, 0xc8, 0x79, 0x01, 0x49, 0x02, 0x69, 0x01, 0x60, 0x04, 0xf0, 0x18,
0x76, 0x01, 0x46, 0x40, 0x76, 0xfe, 0x12, 0x6c, 0xa2, 0xf2, 0xfe, 0x33,
0xf2, 0x65, 0xf1, 0x29, 0x64, 0x14, 0x65, 0x00, 0xd4, 0x55, 0x74, 0x15,
0xf2, 0x29, 0xd4, 0x55, 0x00, 0xee, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned int game_pong_len= 246;

void memory_init()
{	
	memory_area = (unsigned char *)malloc(MEMORY_SIZE);
	memset(memory_area,0,MEMORY_SIZE);
	printf("Malloc RAM %p %i\n",memory_area,MEMORY_SIZE);
}

void memory_load_builtin ()
{
	memcpy_P(&memory_area[MEMORY_START],game_pong,game_pong_len);
	printf("Loaded Pong to 0x%04x (%i bytes) %x\n",MEMORY_START,game_pong_len,game_pong[0]);
}		


