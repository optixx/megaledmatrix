/*This is a port of a chip8 emulator originally coded by TJA (of the emutalk forums) for the pc
This is a port of that emu to the psp and has being named "Dreamchip".
The authors permission was granted before i released this.
Feel free to look and learn from this code.
I do NOT permit any unoficial builds to be released of this emu without my consent. 
to get in touch with me send me a email or msn me at  : Zion______@hotmail.com.
Thank you*/

#include "include/cpu.h"
#include "include/memory.h"
 
	word keys[]=
 
	{
		KEY_7_PAD,KEY_8_PAD,KEY_9_PAD,KEY_SLASH_PAD,
		KEY_4_PAD,KEY_5_PAD,KEY_6_PAD,KEY_ASTERISK,
		KEY_1_PAD,KEY_2_PAD,KEY_3_PAD,KEY_MINUS_PAD,
		KEY_0_PAD,KEY_DEL_PAD,KEY_ENTER_PAD,KEY_PLUS_PAD
	};
 
	byte keyval = 0;
 
void CheckCmdLine(int argc, char **argv)
	{
 
	if (argc < 2)
	{
        printf("i am not working right");
		printf("im being gay lol");
  	}
 
	}
 
/*
|Rom loading infomation
*/
 
void START_ROM( char *ROM )
	{
	char location[256] = "Roms/ROM";
 
	GAME_INFO( 0xFFF );
    LOAD_ROM( location ,0x200,0xFFF);
	CHECK_LOAD();
	}
 
/*Initalises settings for emulation*/
void init( int argc, char **argv )
	{
	CheckCmdLine(argc, argv);
	START_ROM( argv[1] );
	}

int updown[14];

/* wrapper for SDL_PollEvent that records the states of the jbuttons in 
 * an updown array
 */
int KeyMap_PollEvent(SDL_Event *event)
	{
    int result=SDL_PollEvent(event);
    if(result==0) return result;   
	
    if(event->type==SDL_JOYBUTTONUP) {
    if(event->jbutton.button == 7 /* LEFT */) updown[7] = 0;     
	if(event->jbutton.button == 8 /* UP */) updown[8] = 0;  
	if(event->jbutton.button == 9 /* RIGHT */) updown[9] = 0;  
	if(event->jbutton.button == 6 /* DOWN */) updown[6] = 0;  
	if(event->jbutton.button == 2 /* CROSS */) updown[2] = 0;   
	if(event->jbutton.button == 3 /* SQUARE */) updown[3] = 0;
    }
	
    if(event->type==SDL_JOYBUTTONDOWN) {
    if(event->jbutton.button == 7 /* LEFT */) updown[7] = 1;    
	if(event->jbutton.button == 8 /* UP */) updown[8] = 1;  
	if(event->jbutton.button == 9 /* RIGHT */) updown[9] = 1;  
	if(event->jbutton.button == 6 /* DOWN */) updown[6] = 1;  
	if(event->jbutton.button == 2 /* CROSS */) updown[2] = 1;  
	if(event->jbutton.button == 3 /* SQUARE */) updown[3] = 1;
	if(event->jbutton.button == 11 /* START */) exit(0);
    }
	    
    return result;
	}

  
/*sees if a certain key is pressed if so true is returned, else false is returned*/

int
comp_keys( byte key_pressed, SDL_Event *event )
{

 //SDL_Event event;
 
	int keyval = 0;
	int done = 0;
 //
 
	while ( KeyMap_PollEvent(event) )
	{
  
 
		if ( event->type == SDL_QUIT ) { done = 2; }
		{
		
        if( event->type == SDL_KEYDOWN || event->type == SDL_JOYBUTTONDOWN) 
		done = 1;
		}
		  
  
        if ( event->key.keysym.sym == keys[key_pressed] ) 
        keyval = 1;
		
        else
		
        if ( event->key.keysym.sym == SDLK_ESCAPE ) 
        done = 1;
	}
	  
	switch (event->type){
 
		case SDL_JOYBUTTONUP:
		if(updown[7] /* LEFT */ && key_pressed == 4 )  keyval = 0;
		if(updown[8] /* UP */ && key_pressed == 0 )  keyval = 0;
		if(updown[9] /* RIGHT*/ && key_pressed == 6 )  keyval = 0;
		if(updown[6] /* DOWN */ && key_pressed == 10 )  keyval = 0;
		if(updown[2] /*CROSS */ && key_pressed == 2 )  keyval = 0;
		if(updown[3] /*SQUARE*/ && key_pressed == 5) keyval = 0;
 
		break;
 
		case SDL_JOYBUTTONDOWN:
		break;
 
		default:
        done = 0;
        break;
        }
 
		if (done == 2)
        exit(0);
		if(updown[7] /* LEFT */ && key_pressed == 4 )  keyval = 1;
		if(updown[8] /* UP */ && key_pressed == 0 )  keyval = 1;
		if(updown[9] /* RIGHT*/ && key_pressed == 6 )  keyval = 1;
		if(updown[6] /* DOWN */ && key_pressed == 10)  keyval = 1;
		if(updown[2] /*CROSS */ && key_pressed == 2 )  keyval = 1;
		if(updown[3] /*SQUARE*/ && key_pressed == 5) keyval = 1;
		
		return keyval; 
			
		}
 
/* keep scaning through keys till one is pressed then return its value */

	byte
	check_keys( )
{
	SDL_Event event;
	int done = 0;
	byte keyval = 0;
	SDLKey key;
 
	while(done == 0)
    {  
    while ( KeyMap_PollEvent(&event) )
	{
	switch (event.type){
 
		case SDL_JOYBUTTONDOWN:
		if(event.jbutton.button == 7 /* LEFT */) key = SDLK_KP4;
		if(event.jbutton.button == 8 /* UP */) key = SDLK_KP7;
		if(event.jbutton.button == 9 /* RIGHT */) key = SDLK_KP6;
		if(event.jbutton.button == 6 /* DOWN */) key = SDLK_KP3;
		if(event.jbutton.button == 2 /* CROSS */) key = SDLK_KP5;
		if(event.jbutton.button == 3 /* SQUARE */) key = SDLK_KP4;
		if(event.jbutton.button == 11 /* START */) exit(0);
		break;
 
        default:
		done = 0;
        break;
		}   
				
		if ( event.type == SDL_QUIT ) { done = 2; }
		
        if( event.type == SDL_KEYDOWN || event.type == SDL_JOYBUTTONDOWN) {
         done = 1;
		  
        if(  event.type == SDL_KEYDOWN) key = event.key.keysym.sym;
 
	switch (key){
 
        case 256:
        keyval = 12;
        break;  
		
        case SDLK_KP1:
        keyval = 8;
        break;
		
        case SDLK_KP2:
        keyval = 9;
        break;
		
        case SDLK_KP3:
        keyval = 10;
        break;
		
        case SDLK_KP4:
        keyval = 4;
        break;
		
        case SDLK_KP5:
        keyval = 5;
        break;
		
        case SDLK_KP6:
        keyval = 6;
        break;
		
        case SDLK_KP7:
        keyval = 0;
        break;
		
        case SDLK_KP8:
        keyval = 1;
        break;
		
        case SDLK_KP9:
        keyval = 2;
        break;
		
        case SDLK_KP_DIVIDE:
        keyval = 3;
        break;
		
        case SDLK_KP_MULTIPLY:
        keyval = 7;
        break;
		
        case SDLK_KP_MINUS:
        keyval = 11;
        break;
		
        case SDLK_KP_PERIOD:
        keyval = 13;
        break;
		
        case SDLK_KP_ENTER:
        keyval = 14;
        break;
		
        case SDLK_KP_PLUS:
        keyval = 15;
        break;
 
        default:
        done = 0;
        break;
		}   
 
		} 
		}
		}
		
		if ( done ==2 )
		exit(0);
 
		return keyval;
		}
 
int main( int argc, char** argv )

	{
	
	int done=0;
    
	SDL_Surface *screen2;
	SDL_Event event;
	SDL_Joystick *joy;
 
		if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_AUDIO) <0 )
		{
        printf("Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
		}
 
	joy=0;
	if(SDL_NumJoysticks()>0)
	joy=SDL_JoystickOpen(0);
 
    screen2 = SDL_SetVideoMode(64*8, 32*8, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
 
		if ( screen2 == NULL )
		{
        printf("Unable to set 480x272 video: %s\n", SDL_GetError());
        exit(1);
		}
 
    atexit(SDL_Quit);
 
    init( argc, argv );
	cpu_int();
 
    while(done == 0)
	
    {

		//for(x=0;x<5000;x++) //some arbitrary number of loops
		cpu( keys, screen2, &event ); //call cpu to execute instructions, etc
 
 		if (dtime>0)dtime-=1;
 		if (s_time>0)s_time-=1;
     }
  
	return 0;
  
}
