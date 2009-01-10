/*
Support routine for the 32x16 led-board as sold by baco army goods.
(C) 2008 Jeroen Domburg (jeroen AT spritesmods.com)

This program is free software: you can redistribute it and/or modify
t under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
	    
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
			    
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <avr/pgmspace.h>
#include "leds.h"
#include "pic.h"
#include "uart.h"
//#include "exp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>




int main(void) {
    int x,y,n;
    unsigned char c;
    //init led board
    initleds();
    uart_setup();
    
    //copy picture from pic.h to display memory
    for (x=0; x<512; x++) dispmem[x]=pgm_read_byte(picture+x);
    sei(); //let the ledboard-routine do it's job
    while(1) do_leds();
    return 0;
}

