/*
 *  untitled.h
 *  Chip8SDL
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _MEMORY_H_
#define _MEMORY_H_


#define MEMORY_SIZE	0x1000
#define MEMORY_START	0x200

void memory_load_rom(const char * filename);
void memory_init();

unsigned char *memory_area;

#endif
