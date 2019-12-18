//////////////////////////////////////////////////////////////////////////////////	 
//
//  文 件 名   : oled.c
//  版 本 号   : v2.0
//  作    者   : gflytu
//  生成日期   : 2018-1010
//  E_MAIL     : gflytu@163.com
//  功能描述   : OLED 4接口演示例程
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PE2（SCL）
//              D1   接PE3（SDA）
//              RES  接PE4
//              DC   接PE5              
//              ----------------------------------------------------------------
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	
#include "delay.h"
//OLED模式设置
//0:4线串行模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   
#define OLED_RST_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_4)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOE,GPIO_Pin_4)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_5)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOE,GPIO_Pin_5)
 
//使用4线串行接口时使用 
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_2)//D0
#define OLED_SCLK_Set() GPIO_SetBits(GPIOE,GPIO_Pin_2)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_3)//D1
#define OLED_SDIN_Set() GPIO_SetBits(GPIOE,GPIO_Pin_3)
 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ShowChinese_Row(u8 x,u8 y,char *hanzi);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



