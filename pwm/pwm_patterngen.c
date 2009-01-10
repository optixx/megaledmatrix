#include <stdio.h>
#include <inttypes.h>

//Euklidscher verteilalgo.

void teste(char test[2][2])
{
	test[1][1] = 'e';
}


void main()
{

char i,cnt;

char len=16;
char pulses;


uint16_t pattern=0;


for (pulses=1; pulses<=len; pulses++)

{

	char test[2][2];
	teste(test);
	printf("c",test[1][1]);


	cnt=len;
	for (i=0; i<len; i++)
	{
   		pattern <<= 1;
		if(cnt >= len)
		{
			pattern = (pattern | 1);
			cnt = cnt-len;
		}
		cnt = cnt+pulses;
	}

	printf("\n %2i pulse: 0x%x|  ",pulses, pattern);

        for (i=len-1; i>=0; i--)
	{
		if(pattern & (1<<i))
			printf("1");
		else
			printf("0");
	}

}

	printf("\n\n");
}
