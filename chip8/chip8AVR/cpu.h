/*
 *  Chip8AVR
 *
 *  Created by david on 1/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _CPU_H_
#define _CPU_H_

#define FRAMEBUFER_SIZE ((64*32)/8)


uint8_t dtime, s_time;
uint8_t runable;

void cpu_int(void);
void cpu_halt(void);
uint8_t  cpu(void);
  
#endif

