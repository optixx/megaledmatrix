#ifndef _CHIP8_H_
#define _CHIP8_H_

// Standard Libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "opcode.h"

// Emulation Constants
#define MEMORYSIZE	0xFFF
#define REGISTERSIZE	0x10
#define STACKSIZE	0x10
#define ROMENTRY	0x200
#define MAXNAME		1024
#define FONTCOUNT	0x50

// Timer Mechanism
#define TIMER 1
#define FRAMERATE 60

#endif
