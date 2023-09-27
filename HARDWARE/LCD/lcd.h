#ifndef _LCD_H
#define _LCD_H
#define uchar unsigned char
#define uint unsigned int
#include "stm32f10x.h"

//彩屏管脚定义
#define GPIO_TFT  GPIOA
#define TFT_RCC_APB RCC_APB2Periph_GPIOA
#define SDA 			GPIO_Pin_6
#define SCL 			GPIO_Pin_7
#define CS 				GPIO_Pin_4   //片选端,低电平有效
#define RESET 		GPIO_Pin_5 //复位端，低电平复位
#define RS 				GPIO_Pin_15    //0:命令  1：数据

#define SDA_H   GPIO_SetBits(GPIO_TFT,SDA)
#define SDA_L   GPIO_ResetBits(GPIO_TFT,SDA)
#define SCL_H   GPIO_SetBits(GPIO_TFT,SCL)
#define SCL_L   GPIO_ResetBits(GPIO_TFT,SCL)
#define CS_H    GPIO_SetBits(GPIO_TFT,CS)
#define CS_L    GPIO_ResetBits(GPIO_TFT,CS)
#define RESET_H GPIO_SetBits(GPIO_TFT,RESET)
#define RESET_L GPIO_ResetBits(GPIO_TFT,RESET)
#define RS_H    GPIO_SetBits(GPIO_TFT,RS)
#define RS_L    GPIO_ResetBits(GPIO_TFT,RS)

//定义常用颜色
#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   
#define GRAY1   0x8410      	
#define GRAY2   0x4208 


void LCD_Init(void);
void LCD_GPIO_Conf(void);
void Font_Test(void);
void Display_Desc(void);
void showimage1(const unsigned char *);
void Display_ASCII8X16(uint ,uint ,uchar *);
void String_show(uint x, uint y, uint fc, uint bc, uchar *s);
void Draw_String(u16 x, u16 y, u8* pFont, u8 size, u32 fColor,u32 bColor);
void dsp_single_colour(int color);
#endif
