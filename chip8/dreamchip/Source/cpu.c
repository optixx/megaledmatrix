/*This is a port of a chip8 emulator originally coded by TJA (of the emutalk forums) for the pc
This is a port of that emu to the psp and has being named "Dreamchip".
The authors permission was granted before i released this.
Feel free to look and learn from this code.
I do NOT permit any unoficial builds to be released of this emu without my consent. 
to get in touch with me send me a email or msn me at  : Zion______@hotmail.com.
Thank you*/

#include "include/cpu.h"
#include "include/graphics.h"

unsigned char V[16];
unsigned int subPC[16];
byte subcounter;
static unsigned int PC, I;
byte screen[2049];
byte old_screen[2049];
int test;

byte font[80]={ 0xF0, 0x90, 0x90, 0x90, 0xF0,// 0
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

/*Main code for emulation of Chip8*/
void cpu( word *keys, SDL_Surface *screen2, SDL_Event *event )
	{
		byte keyval;
		int print, i;
		byte vx, yline, data, vxval, vyval;

		word opcode;
		opcode = ((RAM[PC]<<8) + RAM[PC+1]);
		KeyMap_PollEvent(event);

  /*if ( event->key.keysym.sym == SDLK_F1 )
      print = 1; */

  //printf("opcode: 0x%04X PC: 0x%03X I: 0x%03X dtime: %i\n", opcode, PC, I, dtime);
  //printf("V[0]: V[1}: %i V[2]: %i V[A]: %i V[B]: %i V[E]: %i\n", V[1], V[2], V[0xA], V[0xB], V[0xE] );

	switch (opcode&0xF000){

		case 0x0000:
		switch (opcode&0x00FF){
		case 0x00E0://0x00E0
			for(i=0; i <2047; i++ ){
				screen[i] = 0; //clears the screen
			}
			PC+=2;
	break;

		case 0X00EE:
			subcounter -= 1;
			PC = subPC[subcounter];
			PC+=2;
	break;

        case 0X00FF:  //super instruction enable extended screen mode (128 x 64)
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
		if (V[((opcode&0x0F00)>>8)]==(opcode&0x00FF))PC+=4;
		else PC+=2;
	break;

		case 0x4000: //seems good
		if (V[((opcode&0x0F00)>>8)]!=(opcode&0x00FF))PC+=4;
		else PC+=2;
	break;

		case 0x5000:
		if (V[((opcode&0x0F00)>>8)]==V[((opcode&0x00F0)>>4)])PC+=4;
		else PC+=2;;
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
        if ((V[((opcode&0x00F0)>>4)]+V[((opcode&0x0F00)>>8)]) > 0xFF) V[0xF]=1;
        else V[0xF]=0;
		V[((opcode&0x0F00)>>8)]+=V[((opcode&0x00F0)>>4)];
		PC+=2;

    break;

		case 0x5:  //seems good
        if (V[((opcode&0x00F0)>>4)]>(V[((opcode&0x0F00)>>8)])) V[0xF]=00;
        else V[0xF]=1;
        V[((opcode&0x0F00)>>8)]-=V[((opcode&0x00F0)>>4)];
        PC+=2;
    break;

		case 0x6:  //seems good
        V[0xF]=V[((opcode&0x0F00)>>8)]&0x01;
        V[((opcode&0x0F00)>>8)]>>=1;
		PC+=2;
    break;

		case 0x7: //seems good
        if (V[((opcode&0x00F0)>>4)]<(V[((opcode&0x0F00)>>8)])) V[0xF]=0;
        else V[0xF]=1;
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
		data = RAM[I+yline];//this retrieves the byte for a give line of pixels
		for(vx=0; vx<16; vx++){
		if((data&(0x80>>vx))!=0){
		if(screen[ vx + vxval + ((vyval+yline) * 64)] == 1)
		V[15] = 1;//there has been a collision
		screen[ vx + vxval + ((vyval+yline) * 64)] ^= 1;//note: coordinate registers from opcode
		}
		}
		}
		else{ //code for 8xn sprite where max of n is 8
		for (yline=0;yline<(opcode&0x000F);yline++){
		data = (RAM[I+yline]); //this retrieves the byte for a give line of pixels
		for(vx=0;vx<8;vx++){
		if ((data&(0x80>>vx))!=0){
		if (  screen[ vx + vxval + (( vyval + yline) * 64)]==1)
		V[15]=1; //there has been a collision
		screen[ vx + vxval + ( (vyval + yline) * 64)]^=1; //note: coordinate registers from opcode
		
		}
		}
		}
		}
		
		PC+=2;
        display_screen( screen, old_screen, screen2 );
        for(yline=0; yline<32; yline++)
		for(vx=0; vx<64; vx++)
		old_screen[vx + (yline *64)] = screen[vx + (yline *64)];
	break;

		case 0xE000:
	switch (opcode&0x00FF){
		case 0x9E:
        if (comp_keys(keys[V[((opcode&0x0F00)>>8)]], event )) PC+=4;
        else PC+=2;
    break;

		case 0xA1:
        if (comp_keys(keys[V[((opcode&0x0F00)>>8)]], event )==0) PC+=4;
        else PC+=2;
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
	    keyval=check_keys( );	// keep scanning through keys until one is pressed
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
	   	I = (V[((opcode&0x0F00)>>8)]*5);//I is set the address for the hexadecimal character sprite referred to by the register 5 bytes high
		PC+=2;
		
	break;
	
        case 0x30:
	   	I = (V[((opcode&0x0F00)>>8)]*10);//I is set the address for the hexadecimal character sprite referred to by the register 10 bytes high (super)
		PC+=2;
	break;

		case 0x33: //seems good
        RAM[I] = (V[((opcode&0x0F00)>>8)]/100);
		RAM[I+1] = ((V[((opcode&0x0F00)>>8)]/10)%10);
		RAM[I+2] = ((V[((opcode&0x0F00)>>8)]%100)%10);
		PC+=2;
	break;
	   
		case 0x55: //seems good
	   	for( i=0; i<=((opcode&0x0F00)>>8); i++ ){
			RAM[I+i] = V[i];
		}
		PC+=2;
	break;

		case 0x65:
		for(i=0; i<=((opcode&0x0F00)>>8); i++ ){
			 V[i] = RAM[I+i];
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
	return;
	}
	}

/*Sets cpu to starting values*/
void cpu_int()
{
int x, y;
		test =0;
		for(y=0; y<32; y++)
		for(x=0; x<64; x++)
		old_screen[x + (y *64)] = 0;

//	createGLWindow("DreamChip",480, 272, 32, False );
		for(subcounter=0; subcounter < 80; subcounter++ ){
		RAM[0x000 + subcounter ] = font[subcounter];//loads fonts into RAM[]
		}
		subcounter = 0;
		PC=0x200;//sets pc to starting memory address
		for(x=0; x<16; x++)
        V[x] = 0; //initalises registers to 0

}
