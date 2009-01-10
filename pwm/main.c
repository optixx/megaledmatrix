//NOTES: #RS485-transceiver SN75176

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "ledm.h"

//PINOUT
#define RED PD0
#define GRN PD1
#define CLK PD2
#define BRT PD3
#define RST PD4
#define SEL PD5

//FUNCTION "main"
int main (void) 
{

	//RED and GREEN framebuffer, 2x 16*16byte
	//uint8_t fb_r[16][16];
	//uint8_t fb_g[16][16];
	uint8_t fb_r[16][16];	
	uint8_t fb_g[16][16];	

	uint8_t x,y;
	
	uint8_t brt_loop=0;
	
	//flash LED to show the start of init process
	DDRB = 0xff;
	PORTB = 0x00;
	_delay_ms(500);
	PORTB = 0xff;
	
	//configure I/O
	//0b0011 1111
	DDRD  = 0x3f;
	PORTD = 0x00;

	_delay_us(5);
	
	//clear framebuffer
	clear_fb(fb_r);
	clear_fb(fb_g);

	for(x=3;x<13;x++){
	set_px(fb_g,3,x,1);
	set_px(fb_g,4,x,2);
	set_px(fb_g,5,x,3);

	set_px(fb_g,7,x,2);
	set_px(fb_g,8,x,2);	
	set_px(fb_r,7,x,2);
	set_px(fb_r,8,x,2);

	set_px(fb_g,6,x,3);
	set_px(fb_g,9,x,2);	
	set_px(fb_r,6,x,2);
	set_px(fb_r,9,x,3);

	set_px(fb_r,10,x,3);
	set_px(fb_r,11,x,2);
	set_px(fb_r,12,x,1);

	}



	//TOGGLE BRIGHT AND RESET LINE
  	PORTD |= (1<<BRT) | (1<<RST);
	_delay_us(1);

	PORTD &= ~ ( (1<<BRT) | (1<<RST) );
	_delay_us(1);
	
	//SELECT DATA INPUT, requires continous dataflow..
	//We should implement "write frame" and "keep alive" functions. 
	//When Select ist low, matrix shows data from internal buffer. matrix then only needs clock signal, nothing else.
	PORTD |= (1<<SEL);
	_delay_us(1);
	
	//as in datasheet: BRIGHT HIGH
	PORTD |= (1<<BRT);
	_delay_us(1);

  	//MAIN LOOP
	//code implemented as described in datasheet timing diagram, page 295

  	while(1)
	{

//		scroll_h(fb_r,'l');
//		scroll_v(fb_g,'u');

		for(brt_loop=0; brt_loop<16;brt_loop++)
		{
	
			for(y=0; y<16; y++)
			{	
				//PULL BRIGHTNESS UP, AS IN DATASHEET
				PORTD |= (1<<BRT);
				_delay_us(7);
				for(x=0; x<16; x++)
				{
					//IF RED PX IS SET IN FB:
					PORTD &= ~(1<<RED);
		
					if( gamma_table[fb_r[x][y]] & (1<<brt_loop) )
					{
						//SET RED DATA LINE LOW
						PORTD |= (1<<RED);
					}
		
		
					//IF GREEN PX IS SET IN FB:
		                      	PORTD &= ~(1<<GRN);
	                        
					if( gamma_table[fb_g[x][y]] & (1<<brt_loop) )
		                        {
						//SET GREEN DATA LINE LOW
		                                PORTD |= (1<<GRN);
		                        }
		
					//TOGGLE CLOCK LINE
					PORTD |= (1<<CLK);
					
					PORTD &= ~(1<<CLK);
				}
	
				//PULL BRIGHTNESS LOW, AS IN DATASHEET
				_delay_us(3);
				PORTD &= ~(1<<BRT);
				_delay_us(6);
		
				
			}
			PORTD |= (1<<BRT);
		}


  	}

	return 0;

}
