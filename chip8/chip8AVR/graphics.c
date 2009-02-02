/*
 *  untitled.h
 *  Chip8SDL
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "SDL.h"
#include "graphics.h"

void display_pixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
	Uint32 color = SDL_MapRGB(screen->format, R, G, B);
	switch (screen->format->BytesPerPixel) {
        case 1: {
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			*bufp = color;
			}
			break;
        case 2: {
			Uint16 *bufp;
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
		    }
			break;
        case 3: {
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
				bufp[0] = color;
				bufp[1] = color >> 8;
				bufp[2] = color >> 16;
			} else {
				bufp[2] = color;
				bufp[1] = color >> 8;
				bufp[0] = color >> 16;
			}
		}
			break;

		case 4: { /* Probably 32-bpp */
			Uint32 *bufp;
			bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			*bufp = color;
		}
			break;
		}
}

void display_scene( SDL_Surface *screen, int x_pos, int y_pos, int R, int G, int B)
{ 
	int x, y;
	SDL_LockSurface(screen);
	for( x=x_pos*8;x<(8*x_pos+8);x++)
        for( y=y_pos*8;y<(8*y_pos+8);y++)
			display_pixel(screen, x, y, R, G, B);
	SDL_UnlockSurface(screen);
}

void display_screen(char *framebuffer01, SDL_Surface *screen)
{
	int x, y;
	for(y=0; y<32; y++)
		for(x=0; x<64; x++)
		if( framebuffer01[x + (y *64)])			
			display_scene( screen, x, y, 255, 255, 255 );
		 else if ( framebuffer01[x + (y *64)] ==0)
			display_scene( screen, x, y, 0, 0, 0 );
		
		
	SDL_Flip(screen);
}
