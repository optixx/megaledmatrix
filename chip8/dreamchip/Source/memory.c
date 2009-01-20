/*This is a port of a chip8 emulator originally coded by TJA (of the emutalk forums) for the pc
This is a port of that emu to the psp and has being named "Dreamchip".
The authors permission was granted before i released this.
Feel free to look and learn from this code.
I do NOT permit any unoficial builds to be released of this emu without my consent. 
to get in touch with me send me a email or msn me at  : Zion______@hotmail.com.
Thank you*/

#include "include/memory.h"

static int load_error;

/*
| allocating memory for roms's
*/
void GAME_INFO( int TOTAL_SIZE )
{

	RAM = malloc( TOTAL_SIZE );

}

/*
|Loading games into virtual memory
*/
void LOAD_ROM(char *ROM, word POS, word SIZE_OF_ROM )
{


		if (RAM == NULL){
		printf("There was a problem allocating memory space for ROM\n");
		load_error = 1;
}
		else {

		FILE *in;

		if ((in = fopen( ROM, "rb")) == NULL){
        printf("Cannot open %s\n", ROM );
        load_error = 1;
        }
        else {
        fread( &RAM[POS], SIZE_OF_ROM, 1, in);
        fclose(in);
		}
		}

		if (load_error == 1)
		type.miss_rom = 1;

}

/*
| Check to see if load was sucessful
*/

void CHECK_LOAD( void )
{
		if ( type.miss_rom == 1){
		printf("Error");
		exit(0);
		}
		else {
		printf("ROM's loaded begining emulation........\n");
		}
}


