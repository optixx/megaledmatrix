/*
 *  untitled.h
 *  Chip8SDL
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL.h>

#include "chip8.h"
#include "cpu.h"
#include "memory.h"

int main( int argc, char** argv ){
	
	SDL_Surface *screen;
	SDL_Event event;
	
	if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_AUDIO) <0 ){
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
	}
    
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL ){
        printf("Unable to set %ix%i video: %s\n",SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
	}
	atexit(SDL_Quit);
	
	memory_init();
	memory_load_builtin();
	//memory_load_rom("/Users/david/Devel/arch/chip8/code/Chip8SDL/Roms/ROM");
	cpu_int();
 	
    while(1){
		if (cpu(screen, &event ))
			SDL_Delay(100);
		if (dtime>0)dtime-=1;
			if (s_time>0)s_time-=1;
	}
	SDL_Quit();
	return 0;
}
