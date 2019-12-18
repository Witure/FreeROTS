#include "delay.h"
void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //????
      while(i--) ;    
   }
}
//??????
void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //????
      while(i--) ;    
   }
}

void delay_s(u16 time)
{
		u16 i;
		for(i=0;i<time;i++)
			delay_ms(1000);
}
