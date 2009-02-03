/*
 *  untitled.h
 *  Chip8AVR
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include <avr/io.h>  
#include "graphics.h"

void display_pixel(uint8_t x, uint8_t y,uint8_t c)
{
 //
}


void display_screen(uint8_t *framebuffer01)
{
	uint8_t x, y;
	for(y=0; y<32; y++)
		for(x=0; x<64; x++)
		if( framebuffer01[x + (y *64)])			
			display_pixel( x, y, 1 );
		 else if ( framebuffer01[x + (y *64)] ==0)
			display_pixel( x, y, 0);
		
		
}
