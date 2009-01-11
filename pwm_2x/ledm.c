#include "ledm.h"


/* Farbverlauf
aus		g0,r0

grün		g1,r0
		g2,r0	
		g3,r0
		g3,r2
orange	g3,r3
		g2,r3
		g0,r3
		g0,r2
red		g0,r1
*/


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


void clear_fb(uint8_t fb[][16][16])
{
	uint8_t y,x;

        //clear framebuffer
        for(y=0; y<16; y++)
        {
                for(x=0; x<16; x++)
                {
				fb[0][y][x] = 0;
				fb[1][y][x] = 0;
                }
        }
}


void scroll_h(uint8_t fb[][16][16], uint8_t dir)
{
	uint8_t tempr,tempg,x,y;

        if(dir=='l')
        {
		for(y=0; y<16; y++)
                {
	                tempr=fb[0][0][y];
	                tempg=fb[1][0][y];
				 for(x=0; x<15; x++)
                        {
                                fb[0][x][y] = fb[0][x+1][y];
                                fb[1][x][y] = fb[1][x+1][y];
                        }
                        fb[0][15][y] = tempr;
                        fb[1][15][y] = tempg;
                }
        }

        if(dir=='r')
        {
                for(y=0; y<16; y++)
                {
                        tempr=fb[0][15][y];
                        tempg=fb[1][15][y];
                        for(x=15; x>0; x--)
                        {
                                fb[0][x][y] = fb[0][x-1][y];
                                fb[1][x][y] = fb[1][x-1][y];
                        }
                        fb[0][0][y] = tempr;
                        fb[1][0][y] = tempg;
                }
        }

}


void scroll_v(uint8_t fb[][16][16], uint8_t dir)
{
        uint8_t tempr,tempg,x,y;
        
        if(dir=='u')
        {
                for(x=0; x<16; x++)
                {
                        tempr=fb[0][x][0];
                        tempg=fb[1][x][0];
                        for(y=0; y<15; y++)
                        {
                                fb[0][x][y] = fb[0][x][y+1];
                                fb[1][x][y] = fb[1][x][y+1];
                        }
                        fb[0][x][15] = tempr;
                        fb[1][x][15] = tempg;
                }
        }

	   if(dir=='d')
        {
                for(x=0; x<16; x++)
                {
                        tempr=fb[0][x][15];
                        tempg=fb[1][x][15];
                        for(y=15; y>0; y--)
                        {
                            	fb[0][x][y] = fb[0][x][y-1];
                            	fb[1][x][y] = fb[1][x][y-1];
                        }
                        fb[0][x][0] = tempr;
                        fb[1][x][0] = tempg;
                }
        }
}


//FUNCTION "set_px": 0<=x<=15, 0<=y<=15, color: 'r','g'
//sets pixel in fb_r or fb_g
void set_px(uint8_t fb[][16][16], uint8_t x, uint8_t y, uint8_t color)
{
		uint8_t color_table[10][2]={
						{0,0},
						{1,0},
						{2,0},
						{3,0},
						{3,2},
						{3,3},
						{2,3},
						{0,3},
						{0,2},
						{0,1}};

        	fb[0][x][y] = color_table[color%10][0];
        	fb[1][x][y] = color_table[color%10][1];
}

//FUNCTION "clr_px": 0<=x<=15, 0<=y<=15, color: 'r','g'
//clears pixel in fb_r or fb_g
void clr_px(uint8_t fb[][16][16], uint8_t x, uint8_t y)
{
        	fb[0][x][y] = 0;
        	fb[1][x][y] = 0;
}
