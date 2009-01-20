/*This is a port of a chip8 emulator originally coded by TJA (of the emutalk forums) for the pc
This is a port of that emu to the psp and has being named "Dreamchip".
The authors permission was granted before i released this.
Feel free to look and learn from this code.
I do NOT permit any unoficial builds to be released of this emu without my consent. 
to get in touch with me send me a email or msn me at  : Zion______@hotmail.com.
Thank you*/

#include "include/graphics.h"

void Slock(SDL_Surface *screen2)
{
		if ( SDL_MUSTLOCK(screen2) )
		{
        if ( SDL_LockSurface(screen2) < 0 )
        {
        return;
        }
		}
}

void Sulock(SDL_Surface *screen2)
{     
		if ( SDL_MUSTLOCK(screen2) )
		{
        SDL_UnlockSurface(screen2);
		}
}

void DrawPixel(SDL_Surface *screen2, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
		Uint32 color = SDL_MapRGB(screen2->format, R, G, B);
		switch (screen2->format->BytesPerPixel) {
        case 1: { /* Assuming 8-bpp */
        Uint8 *bufp;

        bufp = (Uint8 *)screen2->pixels + y*screen2->pitch + x;
        *bufp = color;
        }
		
    break;

        case 2: { /* Probably 15-bpp or 16-bpp */
        Uint16 *bufp;

        bufp = (Uint16 *)screen2->pixels + y*screen2->pitch/2 + x;
        *bufp = color;
        }
		
    break;

        case 3: { /* Slow 24-bpp mode, usually not used */
        Uint8 *bufp;

        bufp = (Uint8 *)screen2->pixels + y*screen2->pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
        bufp[0] = color;
        bufp[1] = color >> 8;
        bufp[2] = color >> 16;
		} else {
        bufp[2] = color;
        bufp[1] = color >> 8;
        bufp[0] = color >> 16;
        }
        }
    break;

        case 4: { /* Probably 32-bpp */
        Uint32 *bufp;
        bufp = (Uint32 *)screen2->pixels + y*screen2->pitch/4 + x;
        *bufp = color;
        }
    break;
		}
		}

void DrawScene( SDL_Surface *screen2, int x_pos, int y_pos, int R, int G, int B)
{ 
	int x, y;

		Slock(screen2);
		for( x=x_pos*7;x<(7*x_pos+7);x++)
		{
        for( y=y_pos*7;y<(7*y_pos+7);y++)
        {
             
        DrawPixel(screen2, x, y, R, G, B);
                    
        }
		}  
		Sulock(screen2);
}

/*Code for display*/
void display_screen( byte *screen, byte *old_screen, SDL_Surface *screen2)
{
	int x, y;

		for(y=0; y<32; y++){
		for(x=0; x<64; x++){
		if( screen[x + (y *64)]) /* != old_screen[x + (y *64)] && screen[x + (y *64)] == 1)  */
        DrawScene( screen2, x, y, 255, 255, 255 );
        else if ( screen[x + (y *64)] ==0)  /*!= old_screen[x + (y *64)] && screen[x + (y *64)] == 0 ) */
        DrawScene( screen2, x, y, 0, 0, 0 );
		}
		}
		SDL_Flip(screen2);
}
