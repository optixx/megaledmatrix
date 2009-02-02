#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "exp.h"
#include "leds.h"


void uart_setup(void) {
    //setup uart
    UCSR0B=0x98;
//    UCSR0B=0x18; //no interrupt
    UCSR0C=0x6;

//115200
//    UCSR0A=0; //2=2xspeed
//    UBRR0=10; //115200 baud


    //Halve megabit >:)
    UCSR0A=2; //2=2xspeed
    UBRR0=4; //500000 baud
}


ISR(USART_RX_vect) {
    static short wpos=0;
    static unsigned char *wptr;
    unsigned char b;
    b=UDR0;
    //Pass through all command bytes
    if (b&0x80) {
//	while((UCSR0A&(1<<5))==0) ;
	UDR0=b;
	if (b==0x80) { //reset frame
	    wpos=0;
	    wptr=dispmem;
/*	} else if (b==0xaa) { //reset avr
	    //reset avr using watchdog
	    cli();
	    WDTCSR=(1<<3)|(1<<4);
	    WDTCSR=(1<<3);
	    while(1);
*/	}
	return;
    }
    
    if (wpos==512) { //check display full
	//yes -> pass through
//	while((UCSR0A&(1<<5))==0) ;
	UDR0=b;
    } else {
//	dispmem[wpos++]=pgm_read_byte(exptab+(b>>2));
	*wptr=pgm_read_byte(exptab+(b>>2));
	wptr++;
	wpos++;
    }
}

