#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 战舰STM32F103开发板V3
//2.4寸/2.8寸/3.5寸/4.3寸/7寸 TFT液晶驱动	  
//支持驱动IC型号包括:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963等		    
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/7/4
//版本：V2.9
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved	
//********************************************************************************
//V1.2修改说明
//支持了SPFD5408的驱动,另外把液晶ID直接打印成HEX格式.方便查看LCD驱动IC.
//V1.3
//加入了快速IO的支持
//修改了背光控制的极性（适用于V1.8及以后的开发板版本）
//对于1.8版本之前(不包括1.8)的液晶模块,请修改LCD_Init函数的LCD_LED=1;为LCD_LED=1;
//V1.4
//修改了LCD_ShowChar函数，使用画点功能画字符。
//加入了横竖屏显示的支持
//V1.5 20110730
//1,修改了B505液晶读颜色有误的bug.
//2,修改了快速IO及横竖屏的设置方式.
//V1.6 20111116
//1,加入对LGDP4535液晶的驱动支持
//V1.7 20120713
//1,增加LCD_RD_DATA函数
//2,增加对ILI9341的支持
//3,增加ILI9325的独立驱动代码
//4,增加LCD_Scan_Dir函数(慎重使用)	  
//6,另外修改了部分原来的函数,以适应9341的操作
//V1.8 20120905
//1,加入LCD重要参数设置结构体lcddev
//2,加入LCD_Display_Dir函数,支持在线横竖屏切换
//V1.9 20120911
//1,新增RM68042驱动（ID:6804），但是6804不支持横屏显示！！原因：改变扫描方式，
//导致6804坐标设置失效，试过很多方法都不行，暂时无解。
//V2.0 20120924
//在不硬件复位的情况下,ILI9341的ID读取会被误读成9300,修改LCD_Init,将无法识别
//的情况（读到ID为9300/非法ID）,强制指定驱动IC为ILI9341，执行9341的初始化。
//V2.1 20120930
//修正ILI9325读颜色的bug。
//V2.2 20121007
//修正LCD_Scan_Dir的bug。
//V2.3 20130120
//新增6804支持横屏显示
//V2.4 20131120
//1,新增NT35310（ID:5310）驱动器的支持
//2,新增LCD_Set_Window函数,用于设置窗口,对快速填充,比较有用,但是该函数在横屏时,不支持6804.
//V2.5 20140211
//1,新增NT35510（ID:5510）驱动器的支持
//V2.6 20140504
//1,新增ASCII 24*24字体的支持(更多字体用户可以自行添加)  
//2,修改部分函数参数,以支持MDK -O2优化
//3,针对9341/35310/35510,写时间设置为最快,尽可能的提高速度
//4,去掉了SSD1289的支持,因为1289实在是太慢了,读周期要1us...简直奇葩.不适合F4使用
//5,修正68042及C505等IC的读颜色函数的bug.
//V2.7 20140710
//1,修正LCD_Color_Fill函数的一个bug. 
//2,修正LCD_Scan_Dir函数的一个bug.
//V2.8 20140721
//1,解决MDK使用-O2优化时LCD_ReadPoint函数读点失效的问题.
//2,修正LCD_Scan_Dir横屏时设置的扫描方式显示不全的bug.
//V2.9 20141130
//1,新增对SSD1963 LCD的支持.
//2,新增LCD_SSD_BackLightSet函数
//3,取消ILI93XX的Rxx寄存器定义
//////////////////////////////////////////////////////////////////////////////////	 
/*

在彩屏引脚上都有引脚标识，所以很容易知道各个管脚的用途，TFT彩屏工作电压为3.3V，虽然可能接5V也可以使用，但对液晶不好，所以1引脚最好还是接3.3V。

 2引脚为GND，直接接地就好了。

 3引脚CS为屏模块的SPI片选信号输入引脚，与STM32芯片IO口相接即可，本程序接的是A13；

 4引脚REST(RST)为屏模块复位输入引脚，与STM32芯片IO口相接即可，本程序接的是A14；

 5A0(RS)引脚为屏模块命令 /数据 信号输入选择引脚，与STM32芯片IO口相接即可，本程序接的是A15；

 6引脚SDA（SDI）为屏模块的SPI数据输入引脚，与STM32芯片IO口相接即可，本程序接的是A11；

 7引脚SCL(SCK)为屏模块的SPI时钟信号输入引脚，与STM32芯片IO口相接即可，本程序接的是A12；
 8引脚LED为屏模块的LED背光，注意，LCD没有背光就一片黑，啥也看不到！，本程序接的是VCC3.3；
  
*/

uchar Zk_ASCII8X16[]=
 {
 0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,/*"1",0*/
 0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00,/*"5",1*/
	 
	 
	 
0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,/*"I",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",1*/
0x00,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,/*"l",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,/*"o",3*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x10,0x10,0x00,0x00,/*"v",4*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x7E,0x40,0x40,0x42,0x3C,0x00,0x00,/*"e",5*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",6*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x18,0x10,0x10,0xE0,/*"y",7*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,/*"o",8*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00/*"u",9*/
  };	
//彩屏管脚初始化
void LCD_GPIO_Conf()
{
	GPIO_InitTypeDef GPIO_InitStruct;//定义结构体
	RCC_APB2PeriphClockCmd(TFT_RCC_APB,ENABLE);//是使能GPIOA推挽输出
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=SDA|SCL|CS|RESET|RS;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TFT,&GPIO_InitStruct);
}



//#define USE_LANDSCAPE//横竖屏切换

void  SPI_WriteData(uchar Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
			SDA_H;
		else 
			SDA_L;
			SCL_L;
			SCL_H;
			Data<<=1;
	}
}
//
void  Lcd_WriteIndex(uchar Data)
{
		
		CS_L;
		RS_L;
		SPI_WriteData(Data); 		
		CS_H;
}
//
void  Lcd_WriteData(uchar Data)
{	
		CS_L;
		RS_H;
		SPI_WriteData(Data); 	
		CS_H;
}
//???16???
void  LCD_WriteData_16Bit(unsigned int Data)
{
	CS_L;
	RS_H;
	SPI_WriteData(Data>>8); 	//
	SPI_WriteData(Data); 			//
	CS_H;

}

void Reset()
{
    RESET_L;
    delay_ms(100);
    RESET_H;
    delay_ms(100);
}
//////////////////////////////////////////////////////////////////////////////////////////////
//  for S6D02A1
void LCD_Init()
{	
	Reset();//Reset before LCD Init.
		
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	delay_ms (120);
		
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
#ifdef USE_LANDSCAPE
	Lcd_WriteData(0xA8); //??C8 ??08 A8
#else
	Lcd_WriteData(0xC8); //??C8 ??08 A8
#endif		
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80+3);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	
	Lcd_WriteIndex(0x29);//Display on

}



/*************************************************
???:LCD_Set_Region
??:??lcd,
:xy?
???:?
*************************************************/
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	
#ifdef USE_LANDSCAPE//??
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+3);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+2);

#else//	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+3);	
#endif
	Lcd_WriteIndex(0x2c);
}


void PutPixel(uint x_start,uint y_start,uint color)
{
	Lcd_SetRegion(x_start,y_start,x_start+1,y_start+1);
	LCD_WriteData_16Bit(color);
	
}



void dsp_single_colour(int color)
{
 	uchar i,j;
	Lcd_SetRegion(0,0,128-1,160-1);
 	for (i=0;i<128;i++)
    	for (j=0;j<160;j++)
        	LCD_WriteData_16Bit(color);
}




//
void Display_ASCII8X16(uint x0,uint y0,uchar *s)
{
	int i,j,k,x,y,xx;
	
	unsigned char qm;
	
	long int ulOffset;
	
	char  ywbuf[32],temp[2];

	for(i = 0; i<strlen((char*)s);i++)
	{
		if(((unsigned char)(*(s+i))) >= 161)
		{
			temp[0] = *(s+i);
			temp[1] = '\0';
			return;
		}
		
		else
		{
			qm = *(s+i);

			ulOffset = (long int)(qm) * 16;
			
      for (j = 0; j < 16; j ++)
      {
				ywbuf[j]=Zk_ASCII8X16[ulOffset+j];
      }
             
      for(y = 0;y < 16;y++)
      {
	        for(x=0;x<8;x++) 
	        {
             k=x % 8;
                	
				  	if(ywbuf[y]&(0x80 >> k))
				   	{
				  		xx=x0+x+i*8;
				    	PutPixel(xx,y+y0,RED);
				  	}
							
			   	}
      }

		}
	}     	
}

void Display_Desc()
{ 
    Display_ASCII8X16(10,10,"Welcome");
    Display_ASCII8X16(10,26,"QDtech");
    Display_ASCII8X16(10,42,"Dots:128*128");
    Display_ASCII8X16(10,58,"IC: ST7735");
    Display_ASCII8X16(10,74,"VA:6 o'clock");
    Display_ASCII8X16(10,90,"2013.10.26");
}


//??,??Gui_DrawFont_GBK16???




void Fast_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s)
{
	unsigned char i,j;
	unsigned short k;
	uint HZnum;
	HZnum=sizeof(hz16)/sizeof(typFNT_GBK16);
	while(*s) 
	{	
		if((*s) >= 128) 
		{		
			for (k=0;k<HZnum;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 	Lcd_SetRegion(x,y,x+16-1,y+16-1);
				    for(i=0;i<16*2;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if(hz16[k].Msk[i]&(0x80>>j))	LCD_WriteData_16Bit(fc);
							else 
							{
							if (fc!=bc) LCD_WriteData_16Bit(bc);
							}
						}
					}
					
					
				}
			}
		s+=2;x+=16;
		}
			
		
		else
		s+=1; 
		
	}
}
//?


void String_show(uint x, uint y, uint fc, uint bc, uchar *s)
{
	unsigned char i,j;
	unsigned short k;
	while(*s) 
	{	
		if((*s) <= 128) 
		{		
			for (k=0;k<128;k++) 
			{
			  if (k==(*s)-' ')
			  { 	Lcd_SetRegion(x,y,x+16-1,y-16+1);
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if((Font[k][i]<<j)&0x80)	LCD_WriteData_16Bit(fc);
							else 
							{
							if (fc!=bc) LCD_WriteData_16Bit(bc);
							}
						}
					}
					
					
				}
			}
		s+=1;y-=8;
		}
			 
		
	}
}



void Gui_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
	{	
		if((*s) >= 128) 
		{		
			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	PutPixel(x+j,y+i,fc);
								else {
									if (fc!=bc) PutPixel(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	PutPixel(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) PutPixel(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		}
		else
		s+=1; 
		
	}
}

void Font_Test(void)
{//
	dsp_single_colour(WHITE);
	Gui_DrawFont_GBK16(16,10,BLUE,YELLOW,	"中文显示测试");
	Fast_DrawFont_GBK16(32,30,RED,WHITE,	"壹伍电信");
	Fast_DrawFont_GBK16(40,50,BLUE,WHITE,	"欢迎您");
	Fast_DrawFont_GBK16(32,70,WHITE,BLUE, "因为专注");
	Fast_DrawFont_GBK16(32,90,WHITE,BLUE,	"所以专业");
	//delay_ms(1800);	
	//dsp_single_colour(BLUE);
}	

void showimage1(const unsigned char *p) //40*40 QQ??
{
  	int i; 
	unsigned char picH,picL;
	dsp_single_colour(WHITE); //??  
	
		Lcd_SetRegion(0,0,127,127);		//
			for(i=0;i<128*128;i++)
		 {	
			picL=*(p+i*2);	//??
			picH=*(p+i*2+1);				
			LCD_WriteData_16Bit(picH<<8|picL);  						
		 }		
}



void SetCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{		
	Lcd_WriteIndex(0x2A);
	Lcd_WriteData(0x00);   // 起始位置x高位，因为1.8寸屏是128*160, 不大于255, 直接写0省事
	Lcd_WriteData(xStart); // 起始位置x低位，值传递时自动舍弃了高8位，也省得运算了
	Lcd_WriteData(0x00);   // 起始位置y高位
	Lcd_WriteData(xEnd);   // 起始位置x位位

	Lcd_WriteIndex(0x2B);
	Lcd_WriteData(0x00);
	Lcd_WriteData(yStart);
	Lcd_WriteData(0x00);
	Lcd_WriteData(yEnd);
	
	Lcd_WriteIndex(0x2c);  // 准备写数据
}

void DrawPoint(u16 x, u16 y, u16 color)
{
	SetCursor(x, y, x, y);    
	LCD_WriteData_16Bit(color);
}

void DrawChar(u16 x,u16 y,u8 num,u8 size,u32 fColor, u32 bColor)
{  			
    
    u8 temp;
	u16 y0=y;
	u8 t,t1;
    
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		// 得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';                                    // 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{  
		if(size==16) temp=Font[num][t];	// 调用1608字体

		for( t1=0;t1<8;t1++)
		{			    
			if(temp&0x80) DrawPoint (x, y, fColor);
            else          DrawPoint (x, y, bColor);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  	    	 	  
} 

void Draw_String(u16 x, u16 y, u8* pFont, u8 size, u32 fColor,u32 bColor)
{ 
    
    u16 xStart = x;
    
    // 字体大小控制
    if( size!=12 && size!=16 && size!=24 && size!=32 )
        size=24;    
    
    while(*pFont != 0)
    {
        // 位置控制        
        if(x>(128-size))       // 如果这一行不够位置，就下一行
        {
            x=xStart;
            y=y+size;
        }
        if(y>(160 - size))    // 如果到了屏幕底部，就返回，不再输出
            return;        
        
        if(*pFont < 127)              // ASCII字符
        {
            DrawChar (x, y, *pFont, size, fColor, bColor );            
            pFont++;
            x+=size/2;            
        }
    }      
}






































