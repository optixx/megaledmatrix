/*This is a port of a chip8 emulator originally coded by TJA (of the emutalk forums) for the pc
This is a port of that emu to the psp and has being named "Dreamchip".
The authors permission was granted before i released this.
Feel free to look and learn from this code.
I do NOT permit any unoficial builds to be released of this emu without my consent. 
to get in touch with me send me a email or msn me at  : Zion______@hotmail.com.
Thank you*/

#ifndef _graphics_h_
#define _graphics_h_

#include "driver.h"

void display_screen( byte *screen, byte *old_screen, SDL_Surface *screen2 );

#endif

