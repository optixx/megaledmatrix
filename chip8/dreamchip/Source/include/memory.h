/*This is a port of a chip8 emulator originally coded by TJA (of the emutalk forums) for the pc
This is a port of that emu to the psp and has being named "Dreamchip".
The authors permission was granted before i released this.
Feel free to look and learn from this code.
I do NOT permit any unoficial builds to be released of this emu without my consent. 
to get in touch with me send me a email or msn me at  : Zion______@hotmail.com.
Thank you*/

#ifndef _memory_h_
#define _memory_h_

#include "driver.h"

  struct error_check
  {
    int miss_rom;
  } type;
  
  void CHECK_LOAD( void );
  void START_ROM( char *romname );
  void GAME_INFO( int TOTAL_SIZE );
  void LOAD_ROM(char *ROM_NAME, word POS, word SIZE_OF_ROM );
  
#endif
