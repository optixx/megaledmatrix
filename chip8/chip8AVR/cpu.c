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

#include "cpu.h"
#include "graphics.h"
#include "memory.h"


uint8_t  V[16];
uint16_t subPC[16];
uint16_t PC, I;

uint8_t framebuffer01[FRAMEBUFER_SIZE];
uint8_t subcounter;

uint8_t keyval = 1;
uint8_t font[80] PROGMEM ={ 0xF0, 0x90, 0x90, 0x90, 0xF0,// 0
		0x20, 0x60, 0x20, 0x20, 0x70,// 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0,// 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0,// 3
		0x90, 0x90, 0xF0, 0x10, 0x10,// 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0,// 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0,// 6
		0xF0, 0x10, 0x20, 0x40, 0x40,// 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0,// 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0,// 9
		0xF0, 0x90, 0xF0, 0x90, 0x90,// A
		0xE0, 0x90, 0xE0, 0x90, 0xE0,// B
		0xF0, 0x80, 0x80, 0x80, 0xF0,// C
		0xE0, 0x90, 0x90, 0x90, 0xE0,// D
		0xF0, 0x80, 0xF0, 0x80, 0xF0,// E
		0xF0, 0x80, 0xF0, 0x80, 0x80 // F
		};

/*
1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F
*/
void  readkeys(uint8_t * value){
	uint8_t keyval = 0;
	*value = keyval;
}

void cpu_int()
{
	memset(framebuffer01,0,FRAMEBUFER_SIZE);
	memcpy_P(memory_area,font,80* sizeof(uint8_t));
	memset(V,0,16 * sizeof(uint8_t));
	memset(subPC,0,16 * sizeof(uint16_t));
	PC = MEMORY_START;//sets pc to starting memory address
	subcounter = 0;
	runable = 1;

}

void cpu_halt()
{
	runable = 0;
	
}


uint8_t cpu()
{
	
	uint8_t i;
	uint8_t vx, yline, data, vxval, vyval;

	uint16_t opcode;
	readkeys(&keyval);
	
	if (!runable){
		return 1;
	}
	
	opcode = ((memory_area[PC]<<8) + memory_area[PC+1]);
	#ifdef DEBUG
	printf("opcode: 0x%04X PC: 0x%03X I: 0x%03X dtime: %i\n", opcode, PC, I, dtime);
	printf("V[0]: V[1}: %i V[2]: %i V[A]: %i V[B]: %i V[E]: %i\n", V[1], V[2], V[0xA], V[0xB], V[0xE] );
	#endif
	switch (opcode&0xF000){
		case 0x0000:
			switch (opcode&0x00FF){
				case 0x00E0://0x00E0
					memset(framebuffer01,0,FRAMEBUFER_SIZE); 
					PC+=2;
					break;

				case 0X00EE:
					subcounter -= 1;
					PC = subPC[subcounter];
					PC+=2;
					break;

				case 0X00FF:  //super instruction enable extended framebuffer01 mode (128 x 64)
					PC+=2;
					break;
		
				default:
					printf("\n0x%04X: opcode not implemented.\n", opcode);
					exit(0);
					break;
			}
			break;
  
		case 0x1000:
			PC = (opcode&0x0FFF);
			break;
  
		case 0x2000:
			subPC[subcounter] = PC;
			subcounter += 1;
			PC = (opcode&0x0FFF);
			break;
		case 0x3000:
			if (V[((opcode&0x0F00)>>8)]==(opcode&0x00FF))
				PC+=4;
			else 
				PC+=2;
			break;

		case 0x4000: //seems good
			if (V[((opcode&0x0F00)>>8)]!=(opcode&0x00FF))
				PC+=4;
			else 
				PC+=2;
			break;

		case 0x5000:
			if (V[((opcode&0x0F00)>>8)]==V[((opcode&0x00F0)>>4)])
				PC+=4;
			else 
				PC+=2;;
			break;

		case 0x6000:
			V[((opcode&0x0F00)>>8)]=(opcode&0x00FF);
			PC+=2;
			break;
			
		case 0x7000:
			V[((opcode&0x0F00)>>8)]=(V[((opcode&0x0F00)>>8)]+(opcode&0x00FF));
			PC+=2;
			break;

		case 0x8000:
			switch (opcode&0x000F){
				case 0x0:
					V[((opcode&0x0F00)>>8)]=V[((opcode&0x00F0)>>4)];
					PC+=2;
					break;

				case 0x1:
					V[((opcode&0x0F00)>>8)]=(V[((opcode&0x0F00)>>8)]|V[((opcode&0x00F0)>>4)]);
					PC+=2;
					break;

				case 0x2:
					V[((opcode&0x0F00)>>8)]=(V[((opcode&0x0F00)>>8)]&V[((opcode&0x00F0)>>4)]);
					PC+=2;
					break;

				case 0x3:
					V[((opcode&0x0F00)>>8)]=(V[((opcode&0x0F00)>>8)]^V[((opcode&0x00F0)>>4)]);
					PC+=2;
					break;

				case 0x4:
					if ((V[((opcode&0x00F0)>>4)]+V[((opcode&0x0F00)>>8)]) > 0xFF) 
						V[0xF]=1;
					else 
						V[0xF]=0;
					V[((opcode&0x0F00)>>8)]+=V[((opcode&0x00F0)>>4)];
					PC+=2;
					break;

				case 0x5:  //seems good
					if (V[((opcode&0x00F0)>>4)]>(V[((opcode&0x0F00)>>8)])) 
						V[0xF]=00;
					else 
						V[0xF]=1;
					V[((opcode&0x0F00)>>8)]-=V[((opcode&0x00F0)>>4)];
					PC+=2;
					break;

				case 0x6:  //seems good
					V[0xF]=V[((opcode&0x0F00)>>8)]&0x01;
					V[((opcode&0x0F00)>>8)]>>=1;
					PC+=2;
					break;

				case 0x7: //seems good
					if (V[((opcode&0x00F0)>>4)]<(V[((opcode&0x0F00)>>8)])) 
						V[0xF]=0;
					else 
						V[0xF]=1;
					V[((opcode&0x0F00)>>8)]=V[((opcode&0x00F0)>>4)]-V[((opcode&0x0F00)>>8)];
					PC+=2;
					break;

				case 0xE:  //seems good
					V[0xF]=((V[((opcode&0x0F00)>>8)]&0x80)>>7);
					V[((opcode&0x0F00)>>8)]<<=1;
					PC+=2;
					break;
			}
			break;
		case 0x9000:
			if (V[((opcode&0x0F00)>>8)]!=V[((opcode&0x00F0)>>4)])
				PC+=4;
			else
				PC+=2;
			break;

		case 0xA000:
			I=(opcode&0x0FFF);
			PC+=2;
			break;
		case 0xB000:
			PC=(opcode&0x0FFF)+V[0];
			break;
		case 0xC000:
			V[((opcode&0x0F00)>>8)]=(rand()&(opcode&0x00FF));
			PC+=2;
			break;
		case 0xD000:
			vxval = V[(opcode&0x0F00)>>8];//put register value in variable to create more optimised code as it is called often
			vyval = V[(opcode&0x00F0)>>4];
			if ((opcode&0x000F)==0)//code for 16x16 sprite, superchip only
				for(yline=0; yline<16; yline++ ){
					data = memory_area[I+yline];//this retrieves the char for a give line of pixels
					for(vx=0; vx<16; vx++){
						if((data&(0x80>>vx))!=0){
							if(framebuffer01[ vx + vxval + ((vyval+yline) * 64)] == 1)
								V[15] = 1;//there has been a collision
							framebuffer01[ vx + vxval + ((vyval+yline) * 64)] ^= 1;//note: coordinate registers from opcode
						}
					}
				}
			else{ //code for 8xn sprite where max of n is 8
				for (yline=0;yline<(opcode&0x000F);yline++){
					data = (memory_area[I+yline]); //this retrieves the char for a give line of pixels
					for(vx=0;vx<8;vx++){
						if ((data&(0x80>>vx))!=0){
							if (  framebuffer01[ vx + vxval + (( vyval + yline) * 64)]==1)
								V[15]=1; //there has been a collision
							framebuffer01[ vx + vxval + ( (vyval + yline) * 64)]^=1; //note: coordinate registers from opcode
		
						}
					}
				}
			}
			PC+=2;
			display_screen( framebuffer01 );
			break;
		case 0xE000:
			switch (opcode&0x00FF){
				case 0x9E:
					#ifdef DEBUG					
					printf("read key 0x9e want %i have %i keyval\n",V[((opcode&0x0F00)>>8)],keyval);
					#endif
					if (keyval == V[((opcode&0x0F00)>>8)]){ 
						PC+=4;
						keyval = 0;
					}else{ 
						PC+=2;
					}
					break;
				case 0xA1:
					#ifdef DEBUG
					printf("read key, want %i have %i keyval\n",V[((opcode&0x0F00)>>8)],keyval);
					#endif
					if (keyval == V[((opcode&0x0F00)>>8)]){ 
						keyval = 0;
						PC+=4;
					}else{ 
						PC+=2;
					}
				break;
			}
			break;
		case 0xF000:
			switch (opcode&0x00FF){
				case 0x07:
					V[((opcode&0x0F00)>>8)]=dtime;
					PC+=2;
					break;
				case 0x0A:
					printf("poll key 0x0a\n");
					//keyval = readkeys();
					V[((opcode&0x0F00)>>8)]=keyval;
					PC+=2;
					break;
				case 0x15:
					dtime=V[((opcode&0x0F00)>>8)];
					PC+=2;
					break;
				case 0x18:
					s_time=V[((opcode&0x0F00)>>8)];
					PC+=2;
					break;
				case 0x1E:
					I +=V[((opcode&0x0F00)>>8)];
					PC+=2;
					break;
				case 0x29:
					I = (V[((opcode&0x0F00)>>8)]*5);//I is set the address for the hexadecimal character sprite referred to by the register 5 chars high
					PC+=2;
					break;
				case 0x30:
					I = (V[((opcode&0x0F00)>>8)]*10);//I is set the address for the hexadecimal character sprite referred to by the register 10 bytes high (super)
					PC+=2;
					break;

				case 0x33: //seems good
					memory_area[I] = (V[((opcode&0x0F00)>>8)]/100);
					memory_area[I+1] = ((V[((opcode&0x0F00)>>8)]/10)%10);
					memory_area[I+2] = ((V[((opcode&0x0F00)>>8)]%100)%10);
					PC+=2;
					break;
				case 0x55: //seems good
					for( i=0; i<=((opcode&0x0F00)>>8); i++ ){
						memory_area[I+i] = V[i];
					}
					PC+=2;
					break;
				case 0x65:
					for(i=0; i<=((opcode&0x0F00)>>8); i++ ){
						V[i] = memory_area[I+i];
					}
					PC+=2;
					break;
				default:
					printf("\n0x%04X: opcode is not implemented.\n", opcode );
					exit(0);
					break;
			}
			break;
		default:
			printf("\n0x%04X: opcode is not implemented.\n", opcode );
			exit(0);
			return 0;
	}
	return 0;
}
