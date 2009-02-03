/*
 *  untitled.h
 *  Chip8AVR
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _MEMORY_H_
#define _MEMORY_H_


//#define MEMORY_SIZE		0x1000
#define MEMORY_SIZE		0x0400
#define MEMORY_START	0x200

void memory_init (void);
void memory_load_builtin (void);

unsigned char *memory_area;

#endif
