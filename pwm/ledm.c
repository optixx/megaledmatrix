#include "ledm.h"

// 0, 1, 6, 16

uint16_t gamma_table[4]={0x0000,
					0x8000,
			 		0x8924,
			 		0xffff};
/*
uint8_t gamma_table[4]={0x00,
			0x80,
			0x92,
			0xff};
*/
//0, 2, 4, 7, 11, 19, 35, 64
//0, 1  2  4   6   9  17  32
//0  0  1  2   3   5   9  16


void clear_fb(uint8_t fb[][16])
{
	uint8_t y,x;

        //clear framebuffer
        for(y=0; y<16; y++)
        {
                for(x=0; x<16; x++)
                {
				fb[y][x] = 0;
                }
        }
}


void scroll_h(uint8_t fb[][16], uint8_t dir)
{
	uint8_t temp=0,x,y;

        if(dir=='l')
        {
		for(y=0; y<16; y++)
                {
	                temp=fb[0][y];
	                for(x=0; x<15; x++)
                        {
                                fb[x][y] = fb[x+1][y];
                        }
                        fb[15][y] = temp;
                }
        }

        if(dir=='r')
        {
                for(y=0; y<16; y++)
                {
                        temp=fb[15][y];
                        for(x=15; x>0; x--)
                        {
                                fb[x][y] = fb[x-1][y];
                        }
                        fb[0][y] = temp;
                }
        }

}


void scroll_v(uint8_t fb[][16], uint8_t dir)
{
        uint8_t temp=0,x,y;
        
        if(dir=='u')
        {
                for(x=0; x<16; x++)
                {
                        temp=fb[x][0];
                        for(y=0; y<15; y++)
                        {
                                fb[x][y] = fb[x][y+1];
                        }
                        fb[x][15] = temp;
                }
        }

	   if(dir=='d')
        {
                for(x=0; x<16; x++)
                {
                        temp=fb[x][15];
                        for(y=15; y>0; y--)
                        {
                            	fb[x][y] = fb[x][y-1];
                        }
                        fb[x][0] = temp;
                }
        }
}


//FUNCTION "set_px": 0<=x<=15, 0<=y<=15, color: 'r','g'
//sets pixel in fb_r or fb_g
void set_px(uint8_t fb[][16], uint8_t x, uint8_t y, uint8_t bright)
{
        	fb[x][y] = bright;
}

//FUNCTION "clr_px": 0<=x<=15, 0<=y<=15, color: 'r','g'
//clears pixel in fb_r or fb_g
void clr_px(uint8_t fb[][16], uint8_t x, uint8_t y)
{
        	fb[x][y] = 0;
}
