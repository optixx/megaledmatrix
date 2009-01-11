 
#include <avr/io.h>  
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"

extern FILE uart_stdout;

uint16_t ee_data01 __attribute__((section(".eeprom"))) = 0x01;
uint16_t ee_data02 __attribute__((section(".eeprom"))) = 0x02;
uint16_t ee_data03 __attribute__((section(".eeprom"))) = 0x03;
uint16_t ee_data04 __attribute__((section(".eeprom"))) = 0x04;


const uint8_t text[] PROGMEM = "TEST ME";
const uint8_t font[47][5]   = {

{0xfc,0x12,0x12,0x12,0xfc},
{0xfe,0x92,0x92,0x92,0x6c},
{0x7c,0x82,0x82,0x82,0x44},
{0xfe,0x82,0x82,0x44,0x38},
{0xfe,0x92,0x92,0x92,0x82},
{0xfe,0x12,0x12,0x12,0x02},
{0x7c,0x82,0x92,0x92,0x74},
{0xfe,0x10,0x10,0x10,0xfe},
{0x00,0x82,0xfe,0x82,0x00},
{0x60,0x80,0x80,0x80,0x7e},
{0xfe,0x10,0x28,0x44,0x82},
{0xfe,0x80,0x80,0x80,0x80},
{0xfe,0x04,0x08,0x04,0xfe},
{0xfe,0x08,0x10,0x20,0xfe},
{0x7c,0x82,0x82,0x82,0x7c},
{0xfe,0x12,0x12,0x12,0x0c},
{0x7c,0x82,0xa2,0x42,0xbc},
{0xfe,0x12,0x32,0x52,0x8c},
{0x8c,0x92,0x92,0x92,0x62},
{0x02,0x02,0xfe,0x02,0x02},
{0x7e,0x80,0x80,0x80,0x7e},
{0x3e,0x40,0x80,0x40,0x3e},
{0xfe,0x40,0x20,0x40,0xfe},
{0xc6,0x28,0x10,0x28,0xc6},
{0x06,0x08,0xf0,0x08,0x06},
{0xc2,0xa2,0x92,0x8a,0x86},
{0x7c,0xa2,0x92,0x8a,0x7c},
{0x00,0x08,0x04,0xfe,0x00},
{0xe2,0x92,0x92,0x92,0x8c},
{0x42,0x92,0x9a,0x96,0x62},
{0x10,0x18,0x14,0xfe,0x10},
{0x8e,0x92,0x92,0x92,0x62},
{0x7c,0x92,0x92,0x92,0x60},
{0x02,0xe2,0x12,0x0a,0x06},
{0x6c,0x92,0x92,0x92,0x6c},
{0x0c,0x92,0x92,0x92,0x7c},
{0xfe,0x82,0x82,0x82,0xfe},
{0x00,0x00,0x00,0x00,0xfe},
{0xf2,0x92,0x92,0x92,0x9e},
{0x92,0x92,0x92,0x92,0xfe},
{0x1e,0x10,0x10,0x10,0xfe},
{0x9e,0x92,0x92,0x92,0xf2},
{0xfe,0x92,0x92,0x92,0xf2},
{0x02,0x02,0x02,0x02,0xfe},
{0xfe,0x92,0x92,0x92,0xfe},
{0x9e,0x92,0x92,0x92,0xfe},
{0x00,0x00,0x00,0x00,0x00},
};


#define ABS(A) ((A)<0?-(A):A)
#define SGN(A) (A < 0 ? -1 : (A > 0 ? 1 : 0))  


#define DISPLAY_MAX_X 4
#define DISPLAY_MAX_Y 3

#define DATADDR DDRB
#define DATAPORT PORTB 

#define SELECTDDR DDRA
#define SELECTPORT PORTA 



//PINOUT
#define RED PB0
#define GRN PB1
#define CLK PB2
#define BRT PB3
#define RST PB4


typedef struct panel_t{
    uint16_t row[16];
} panel_t;

typedef struct framebuffer_t{
    panel_t data[DISPLAY_MAX_Y][DISPLAY_MAX_X];
    uint8_t panel_sel;
} framebuffer_t;


volatile framebuffer_t fb;

volatile uint8_t cnt_timer;   

SIGNAL(SIG_OVERFLOW2)
{
    cnt_timer++;
    TCNT2 = 183;
}

inline void _sleep (uint8_t t)
{
    cnt_timer=0;
    while (cnt_timer < t);
}
        
 
inline void sleep(uint16_t msec)
{
    uint16_t i;
    for(i=0; i<msec; i++) {
        _sleep(100);
    }
}

void timer_init(void)
{
    TCCR2 = (1<<CS22);          
    TCNT2  = 183;           
    TIMSK |= (1<<TOIE2);     
    sei();
}

uint8_t * dump_eeprom(uint8_t *addr)
{
    uint16_t c = 0; 
    printf("eeprom:\n");
    for (c=0;c<1024;c++){
        eeprom_busy_wait(); 
        if (c % 16 ==0)
            printf("\n 0x%04x: ",c);
        printf("0x%02x ",eeprom_read_byte(addr));
        addr++;
   }
    printf("\n");
    return addr;
}

uint8_t * dump_progmem(uint8_t *addr)
{    
    uint16_t c = 0; 
    for (c=0;c<(4*1024);c++){
        eeprom_busy_wait(); 
        if (c % 16 ==0)
            printf("\n 0x%04x: ",c);
        printf("0x%02x ",  pgm_read_byte_near(addr));
        addr++;
    }
    printf("\n");
    return addr;
}

inline void panel_set(panel_t *p,uint8_t x,uint8_t y)                                                                   
{                                                                                                         
    p->row[y&15] |= (1<<(x&15));                                                                       
}   

void fb_clear(void)
{
    memset((void*)fb.data,0,DISPLAY_MAX_Y * DISPLAY_MAX_X * 16 * 2);
}

void fb_init(void)
{
    fb_clear();
    fb.panel_sel = 0;
}
                                                                                                           
inline void fb_set(uint8_t x,uint8_t y)                                                                   
{                                                                                                         
    uint8_t fx,fy,px,py;
    panel_t *p;
    
    fx = x / 16;
    px = x % 16;
    fy = y / 16;
    py = y % 16;
  
    if (fy>3 || fx>4)
        return;
    p = &fb.data[fy][fx];
    panel_set(p,px,py);
    
    //printf("%p %i %i -> %i %i %i %i  0x%04x\n ",p,x,y,fx,fy,px,py,fb.data[fy][fx].row[py]); 
    
}   

void fb_write(uint8_t *msg,uint8_t x,uint8_t y)
{
    uint8_t *p = msg;
    while(*p){
        fb_putc(*p,x,y);
        x+=6;
        p++;
    }
}
void fb_putc(uint8_t c,uint8_t x,uint8_t y)
{
    uint8_t cx,cy,i;
    uint8_t d; 
    
    if (c!=32)
        c = c - 65;
    else 
        c = 46;
    
    for (cx=0;cx<5;cx++){
        d = font[c][cx];
        for (cy=0;cy<8;cy++){
            if ( ( d & ( 1 << cy )))
                fb_set(x+cx,y+cy);
            }
    }
}


inline void panel_prepare(void)
{
	//TOGGLE BRIGHT AND RESET LINE
  	DATAPORT |= (1<<BRT) | (1<<RST);
	_delay_us(1);

	DATAPORT &= ~ ( (1<<BRT) | (1<<RST) );
	_delay_us(1);
	
   
    SELECTPORT = 1; 
    _delay_us(1);
	
    DATAPORT |= (1<<BRT);
	_delay_us(1);

}

inline void panel_tick_pixel(void)
{
    DATAPORT |= (1<<CLK);
    //_delay_us(1);
    DATAPORT &= ~(1<<CLK);
    //_delay_us(1);
}

inline void panel_tick_row(void)
{
    //PULL BRIGHTNESS LOW, AS IN DATASHEET
    DATAPORT &= ~(1<<BRT);
    //THIS DELAY IS RESPONSIBLE FOR BRIGHTNESS-wait longer for brighter display-
    //wait too long for flicker.. :)   >350us
    _delay_us(10);
    DATAPORT |= (1<<BRT);
    //_delay_us(1);
 
}


void fb_line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
    
    int8_t d, x, y, ax, ay, sx, sy, dx, dy;

    dx = x2-x1;  ax = ABS(dx)<<1;  sx = SGN(dx);
    dy = y2-y1;  ay = ABS(dy)<<1;  sy = SGN(dy);

    x = x1;
    y = y1;
    if (ax>ay) {		
        d = ay-(ax>>1);
        for (;;) {
            fb_set(x, y);
            if (x==x2) return;
            if (d>=0) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    }
    else {			
        d = ax-(ay>>1);
        for (;;) {
            fb_set(x, y);
            if (y==y2) return;
            if (d>=0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}

inline void fb_rect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
    fb_line(x1,y1,x2,y1);
    fb_line(x2,y1,x2,y2);
    fb_line(x2,y2,x1,y2);
    fb_line(x1,y2,x1,y1);
}

void fb_circle(uint8_t x, uint8_t  y, uint8_t  radius)
{
    int8_t  balance, xoff, yoff;
    
    xoff = 0;
    yoff = radius;
    balance = -radius;
    
    do {
        fb_set(x+xoff, y+yoff);
        fb_set(x-xoff, y+yoff);
        fb_set(x-xoff, y-yoff);
        fb_set(x+xoff, y-yoff);
        fb_set(x+yoff, y+xoff);
        fb_set(x-yoff, y+xoff);
        fb_set(x-yoff, y-xoff);
        fb_set(x+yoff, y-xoff);
    
        if ((balance += (xoff++) + xoff) >= 0)
            balance -= (--yoff) + yoff;
    
    } while (xoff <= yoff);
} 


void dump_row(uint16_t row)
{

    uint8_t x;
    for (x=0;x<16;x++) 
        if (row & ( 1 << x))
            printf("1");
        else
            printf("0");
 
}

void fb_render(void)
{
    uint8_t fx,fy,x,y;
    fb.panel_sel = 0;
    
    panel_prepare();

    for (fy=0;fy<DISPLAY_MAX_Y;fy++) {
        for (fx=0;fx<DISPLAY_MAX_X;fx++) {
            fb.panel_sel++;
            SELECTPORT =  fb.panel_sel; 
            //printf("--------------------\n");
            for (y=0;y<16;y++) {
                for (x=0;x<16;x++) {
                   if ((fb.data[fy][fx].row[y] & ( 1 << x))){
                        DATAPORT |= (1 << GRN);
                        //DATAPORT &= ~(1 <<  RED);
                   } else {
                        DATAPORT &= ~(1 << GRN);
                        //DATAPORT |= (1 << RED);
                   } 
                    panel_tick_pixel();
                }
                panel_tick_row();
                //printf("%p %02i %02i %02i %02i p%02i  ",&fb.data[fy][fx].row,fx,fy,x,y,fb.panel_sel);
                //dump_row(fb.data[fy][fx].row[y]); 
                //printf("\n");
            }
        }
     } 
}
                    


            
int main (void) 
{            
   
    
    DATADDR = 0xff;
    DATAPORT = 0x00;
    SELECTDDR = 0xff;
    SELECTPORT = 0x00;

    uint8_t x,y,d;
    uint16_t t; 

    t=0;
    x=0;
    y=0;
    d=0;

    timer_init();
    uart_init();
    stdout = &uart_stdout;
    
    printf("Basic init...done\n");
    fb_init();
    printf("FB init...done\n");
    
    fb_line(0,0,63,47);
    //fb_set(10,10);
    while(1) {              
        //printf("call fb render...\n");
        fb_clear();
       
        fb_line(0,0,63,y);
        fb_line(63,0,0,y);

        fb_line(0,48,63,48-y);
        fb_line(63,48,0,48-y);
 

        fb_circle(32,24,y);
 
        fb_circle(32,24,y+3);

        fb_circle(32,24,y+6);

        fb_write("DISCO",18,y);
       

        fb_render();
        if (d==0){
            y = y + 1;
            if (y==48) {
                d=1;
            }
        } else {
            y = y - 1;
            if (y==0) {
                d=0;
            }

        }
                
                /* 
        for (t=0;t<8;t++){
            DATAPORT = (1 << t);
            printf("%i\n",t);
            sleep(50);
        }
        */
        /*  
        for (t=1;t<13;t++){
            SELECTPORT = t;
            printf("%i\n",t);
            sleep(250);
        }
        */
    }
    return 0;               
}

