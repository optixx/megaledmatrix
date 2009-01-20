/*
 *  Chip8SDL
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _CPU_H_
#define _CPU_H_

#define FRAMEBUFER_SIZE 2048

#include <SDL.h>

unsigned char dtime, s_time;
unsigned char runable;

void cpu_int();
void cpu_halt();
int cpu(SDL_Surface *screen, SDL_Event *event );
  
#endif
