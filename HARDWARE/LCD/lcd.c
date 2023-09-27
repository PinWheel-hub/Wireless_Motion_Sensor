#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ս��STM32F103������V3
//2.4��/2.8��/3.5��/4.3��/7�� TFTҺ������	  
//֧������IC�ͺŰ���:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963��		    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/7/4
//�汾��V2.9
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.2�޸�˵��
//֧����SPFD5408������,�����Һ��IDֱ�Ӵ�ӡ��HEX��ʽ.����鿴LCD����IC.
//V1.3
//�����˿���IO��֧��
//�޸��˱�����Ƶļ��ԣ�������V1.8���Ժ�Ŀ�����汾��
//����1.8�汾֮ǰ(������1.8)��Һ��ģ��,���޸�LCD_Init������LCD_LED=1;ΪLCD_LED=1;
//V1.4
//�޸���LCD_ShowChar������ʹ�û��㹦�ܻ��ַ���
//�����˺�������ʾ��֧��
//V1.5 20110730
//1,�޸���B505Һ������ɫ�����bug.
//2,�޸��˿���IO�������������÷�ʽ.
//V1.6 20111116
//1,�����LGDP4535Һ��������֧��
//V1.7 20120713
//1,����LCD_RD_DATA����
//2,���Ӷ�ILI9341��֧��
//3,����ILI9325�Ķ�����������
//4,����LCD_Scan_Dir����(����ʹ��)	  
//6,�����޸��˲���ԭ���ĺ���,����Ӧ9341�Ĳ���
//V1.8 20120905
//1,����LCD��Ҫ�������ýṹ��lcddev
//2,����LCD_Display_Dir����,֧�����ߺ������л�
//V1.9 20120911
//1,����RM68042������ID:6804��������6804��֧�ֺ�����ʾ����ԭ�򣺸ı�ɨ�跽ʽ��
//����6804��������ʧЧ���Թ��ܶ෽�������У���ʱ�޽⡣
//V2.0 20120924
//�ڲ�Ӳ����λ�������,ILI9341��ID��ȡ�ᱻ�����9300,�޸�LCD_Init,���޷�ʶ��
//�����������IDΪ9300/�Ƿ�ID��,ǿ��ָ������ICΪILI9341��ִ��9341�ĳ�ʼ����
//V2.1 20120930
//����ILI9325����ɫ��bug��
//V2.2 20121007
//����LCD_Scan_Dir��bug��
//V2.3 20130120
//����6804֧�ֺ�����ʾ
//V2.4 20131120
//1,����NT35310��ID:5310����������֧��
//2,����LCD_Set_Window����,�������ô���,�Կ������,�Ƚ�����,���Ǹú����ں���ʱ,��֧��6804.
//V2.5 20140211
//1,����NT35510��ID:5510����������֧��
//V2.6 20140504
//1,����ASCII 24*24�����֧��(���������û������������)  
//2,�޸Ĳ��ֺ�������,��֧��MDK -O2�Ż�
//3,���9341/35310/35510,дʱ������Ϊ���,�����ܵ�����ٶ�
//4,ȥ����SSD1289��֧��,��Ϊ1289ʵ����̫����,������Ҫ1us...��ֱ����.���ʺ�F4ʹ��
//5,����68042��C505��IC�Ķ���ɫ������bug.
//V2.7 20140710
//1,����LCD_Color_Fill������һ��bug. 
//2,����LCD_Scan_Dir������һ��bug.
//V2.8 20140721
//1,���MDKʹ��-O2�Ż�ʱLCD_ReadPoint��������ʧЧ������.
//2,����LCD_Scan_Dir����ʱ���õ�ɨ�跽ʽ��ʾ��ȫ��bug.
//V2.9 20141130
//1,������SSD1963 LCD��֧��.
//2,����LCD_SSD_BackLightSet����
//3,ȡ��ILI93XX��Rxx�Ĵ�������
//////////////////////////////////////////////////////////////////////////////////	 
/*

�ڲ��������϶������ű�ʶ�����Ժ�����֪�������ܽŵ���;��TFT����������ѹΪ3.3V����Ȼ���ܽ�5VҲ����ʹ�ã�����Һ�����ã�����1������û��ǽ�3.3V��

 2����ΪGND��ֱ�ӽӵؾͺ��ˡ�

 3����CSΪ��ģ���SPIƬѡ�ź��������ţ���STM32оƬIO����Ӽ��ɣ�������ӵ���A13��

 4����REST(RST)Ϊ��ģ�鸴λ�������ţ���STM32оƬIO����Ӽ��ɣ�������ӵ���A14��

 5A0(RS)����Ϊ��ģ������ /���� �ź�����ѡ�����ţ���STM32оƬIO����Ӽ��ɣ�������ӵ���A15��

 6����SDA��SDI��Ϊ��ģ���SPI�����������ţ���STM32оƬIO����Ӽ��ɣ�������ӵ���A11��

 7����SCL(SCK)Ϊ��ģ���SPIʱ���ź��������ţ���STM32оƬIO����Ӽ��ɣ�������ӵ���A12��
 8����LEDΪ��ģ���LED���⣬ע�⣬LCDû�б����һƬ�ڣ�ɶҲ����������������ӵ���VCC3.3��
  
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
//�����ܽų�ʼ��
void LCD_GPIO_Conf()
{
	GPIO_InitTypeDef GPIO_InitStruct;//����ṹ��
	RCC_APB2PeriphClockCmd(TFT_RCC_APB,ENABLE);//��ʹ��GPIOA�������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=SDA|SCL|CS|RESET|RS;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TFT,&GPIO_InitStruct);
}



//#define USE_LANDSCAPE//�������л�

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
	Gui_DrawFont_GBK16(16,10,BLUE,YELLOW,	"������ʾ����");
	Fast_DrawFont_GBK16(32,30,RED,WHITE,	"Ҽ�����");
	Fast_DrawFont_GBK16(40,50,BLUE,WHITE,	"��ӭ��");
	Fast_DrawFont_GBK16(32,70,WHITE,BLUE, "��Ϊרע");
	Fast_DrawFont_GBK16(32,90,WHITE,BLUE,	"����רҵ");
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
	Lcd_WriteData(0x00);   // ��ʼλ��x��λ����Ϊ1.8������128*160, ������255, ֱ��д0ʡ��
	Lcd_WriteData(xStart); // ��ʼλ��x��λ��ֵ����ʱ�Զ������˸�8λ��Ҳʡ��������
	Lcd_WriteData(0x00);   // ��ʼλ��y��λ
	Lcd_WriteData(xEnd);   // ��ʼλ��xλλ

	Lcd_WriteIndex(0x2B);
	Lcd_WriteData(0x00);
	Lcd_WriteData(yStart);
	Lcd_WriteData(0x00);
	Lcd_WriteData(yEnd);
	
	Lcd_WriteIndex(0x2c);  // ׼��д����
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
    
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		// �õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';                                    // �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{  
		if(size==16) temp=Font[num][t];	// ����1608����

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
    
    // �����С����
    if( size!=12 && size!=16 && size!=24 && size!=32 )
        size=24;    
    
    while(*pFont != 0)
    {
        // λ�ÿ���        
        if(x>(128-size))       // �����һ�в���λ�ã�����һ��
        {
            x=xStart;
            y=y+size;
        }
        if(y>(160 - size))    // ���������Ļ�ײ����ͷ��أ��������
            return;        
        
        if(*pFont < 127)              // ASCII�ַ�
        {
            DrawChar (x, y, *pFont, size, fColor, bColor );            
            pFont++;
            x+=size/2;            
        }
    }      
}






































