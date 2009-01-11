//NOTES: #RS485-transceiver SN75176

#ifndef F_CPU
#define F_CPU 20000000L
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "ledm.h"

//PINOUT
#define RED1 PD0
#define GRN1 PD1
#define CLK1 PD2
#define BRT1 PD3
#define RST1 PD4

#define RED2 PC0
#define GRN2 PC1
#define CLK2 PC2
#define BRT2 PC3
#define RST2 PC4


#include "data.h"


//FUNCTION "main"
int main (void) 
{
	uint8_t x,y,data1,data2;
	uint16_t brt_mask;
	
	uint8_t brt_loop=0;
	
	//flash LED to show the start of init process
	DDRB = 0xff;
	PORTB = 0x00;
	_delay_ms(500);
	PORTB = 0xff;
	
	//configure I/O
	//0b0001 1111
	DDRD  = 0x1f;
	PORTD = 0x00;

	DDRC  = 0x1f;
	PORTC = 0x00;
	

	//TOGGLE BRIGHT AND RESET LINE
  	PORTD |= (1<<BRT1) | (1<<RST1);
  	PORTC |= (1<<BRT2) | (1<<RST2);
	_delay_us(1);

	PORTD &= ~ ( (1<<BRT1) | (1<<RST1) );
	PORTC &= ~ ( (1<<BRT2) | (1<<RST2) );
	_delay_us(1);
	
	//as in datasheet: BRIGHT HIGH
	PORTD |= (1<<BRT1);
	PORTC |= (1<<BRT2);
	_delay_us(1);

	uint8_t ysh;

  	//MAIN LOOP
	//code implemented as described in datasheet timing diagram, page 295
  	while(1)
	{
		for(brt_loop=0; brt_loop<16;brt_loop++)
		{
	
			for(y=0; y<16; y++)
			{
				ysh=y<<4;
	
				//PULL BRIGHTNESS UP, AS IN DATASHEET
				PORTD |= (1<<BRT1);
				PORTC |= (1<<BRT2);
				_delay_us(7);

				for(x=0; x<16; x++)
				{
					brt_mask=(1<<brt_loop);

					//##################
					//    PANEL 1
					//##################
					
					data1 = fb1[x+ysh];
					data2 = fb2[x+ysh];
					//IF RED PX IS SET IN FB:
					PORTD &= ~(1<<RED1);		
					if( pwm_table[ (data1 & 0xf0) >> 4 ] & brt_mask )
					{
						//SET RED DATA LINE HIGH
						PORTD |= (1<<RED1);
					}
		
					//IF GREEN PX IS SET IN FB:
		               PORTD &= ~(1<<GRN1);	                        
					if( pwm_table[ (data1 & 0x0f) ]  & brt_mask )
                        	{
						//SET GREEN DATA LINE HIGH
		                    PORTD |= (1<<GRN1);
  		             	}

					//##################
					//    PANEL 2
					//##################

					PORTC &= ~(1<<RED2);
					if( pwm_table[ (data2 & 0xf0)>>4 ] & brt_mask )
					{
						//SET RED DATA LINE HIGH
						PORTC |= (1<<RED2);
					}

		               PORTC &= ~(1<<GRN2);
					if( pwm_table[ (data2 & 0x0f) ] & brt_mask )
		                        {
						//SET GREEN DATA LINE HIGH
		                    PORTC |= (1<<GRN2);
		             	}

					//TOGGLE CLOCK LINE
					PORTD |= (1<<CLK1);
					PORTC |= (1<<CLK2);										
					PORTD &= ~(1<<CLK1);
					PORTC &= ~(1<<CLK2);
				}
	
				//PULL BRIGHTNESS LOW, AS IN DATASHEET
				_delay_us(3);
				PORTD &= ~(1<<BRT1);
				PORTC &= ~(1<<BRT2);
				_delay_us(15);
			}
			PORTD |= (1<<BRT1);
			PORTC |= (1<<BRT2);
		}


  	}
	return 0;
}
