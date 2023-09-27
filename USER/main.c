#include "led.h"
#include "delay.h"
#include "key.h"
#include "exti.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "adc.h"
#include "ADS1292.h"
#include "Timer.h"

float get_temp(void);
void ANO_DT_Send_Sensor(float,int,float);
s32 get_volt(u32 num);
void F2S(float d,char *str);
void ANO_DT_Send_temperature(float);
void ANO_DT_Send_bushu(int);
void ANO_DT_Send_juli(float);
void ANO_DT_Send_heartbeat(int);
void ANO_DT_Send_ADS(void);

int main(void)
{   
	//ads1292变量
	u8 usbstatus=0;	
	vu8 key=0;
	//int S_flag;
	char str[40];
	int i=0,j=0,k=0;
	//计步器变量定义
	char unStepCount=0;             //初始步数
	unsigned long unStepCountTmp=0;
	int rt;
	float juli=0;    //测定距离
	int bushu;    //步数
	
	//温度传感器变量定义
	float temperature;
  int heartbeat=0;
  flag=0;	
  SystemInit();	
  delay_init();//延时函数初始化
  delay_ms(100);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);         //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);   //串口初始化为115200
	MPU_Init();          //mpu6050初始化
  Adc_Init();
	LED_Init();	
	KEY_Init();
	EXTIX_Init();
	DisableJTAG();
	LCD_GPIO_Conf();
	LCD_Init();
	dsp_single_colour(WHITE);
				
	PBout(1)=1;
	PBout(3)=0;
	ADS1292_Init();	//初始化ads1292
	Draw_String(10, 10, "connecting...", 16, RED,WHITE);
	while(mpu_dmp_init())  //检测MPU6050有没有连接
	{
		 delay_ms(200); 
		Draw_String(10, 10, "no connection", 16, RED,WHITE);
	}
	
	dmp_set_pedometer_step_count(unStepCount);                //设置计步器步数初值为0
	Draw_String(10, 10, "successful", 16, RED,WHITE);
	dsp_single_colour(WHITE);
	while(Set_ADS1292_Collect(0))//0 正常采集  //1 1mV1Hz内部侧试信号 //2 内部短接噪声测试
		{	
			//printf("1292寄存器设置失败\r\n");
			//delay_ms(1000);		
		
		}	
		//printf("寄存器设置成功\r\n");
		delay_ms(1000);
		TIM2_Init(10000,7200);//系统指示
	  EXTI->IMR |= EXTI_Line8;//开DRDY中断	
	  unStepCountTmp=0;
	while(1)
	{
		GPIO_ResetBits(GPIO_TFT,RS);
		temperature=get_temp();
		rt = dmp_get_pedometer_step_count(&unStepCountTmp);   //得到步数    返回0表示函数调用成功
		bushu = unStepCountTmp;
		juli = 0.7 * bushu;
		
		
		if(flag==0)
		{
			  j=0;
			  Draw_String(10, 10, "mode:1", 16, RED,WHITE);
			  Draw_String(10, 26, "temperarure:", 16, RED,WHITE);
		    F2S(temperature,str);str[9]='\0';str[10]='\0';str[11]='\0';str[12]='\0';str[13]='\0';str[14]='\0';
		    Draw_String(20, 42, str, 16, RED,WHITE);
		      
	       Draw_String(10, 58, "step count:", 16, RED,WHITE);
		     F2S(bushu,str);
	     	 Draw_String(20, 74, str, 16, RED,WHITE);
				  
         Draw_String(10, 90, "distance:", 16, RED,WHITE);
		     F2S(juli,str);
		     Draw_String(20, 106, str, 16, RED,WHITE); 
			   
         //Draw_String(10, 106, "heartbeat:", 16, RED,WHITE);
		     //F2S(75,str);
		     //Draw_String(20, 122, str, 16, RED,WHITE);
		}
		else if(flag==1)
		{
			i=0;
			if(j==0)
			{
				dsp_single_colour(WHITE);
				Draw_String(10, 10, "mode:2", 16, RED,WHITE);
			}
			j++;
			if	(ads1292_recive_flag)
		  {
        ANO_DT_Send_ADS();				
        ads1292_recive_flag=0;			
		  }
			ANO_DT_Send_Sensor(temperature,bushu,juli);
			//ANO_DT_Send_temperature(temperature);
			//ANO_DT_Send_bushu(bushu);
			//ANO_DT_Send_juli(juli);
			//ANO_DT_Send_heartbeat(75);
		}	
    else
	  {
			if(i==0)
			{
				dsp_single_colour(WHITE);
				Draw_String(10, 10, "mode:3", 16, RED,WHITE);
				state=0;
				 Draw_String(10, 26, "stop", 16, RED,WHITE);
				Draw_String(10, 42, "heartbeat:", 16, RED,WHITE);
				if(heartbeat!=0)
				{
					F2S(heartbeat,str);
		      Draw_String(20, 58, str, 16, RED,WHITE);
				}
			}
			i++;
			if(start==0)
			{
				if(state==0)
				{
					dsp_single_colour(WHITE);
				  Draw_String(10, 10, "mode:3", 16, RED,WHITE);
				  Draw_String(10, 26, "stop", 16, RED,WHITE);
					Draw_String(10, 42, "heartbeat:", 16, RED,WHITE);
				if(heartbeat!=0)
				{
					F2S(heartbeat,str);
		      Draw_String(20, 58, str, 16, RED,WHITE);
				}
				}
				else if(state==1)
				{
					dsp_single_colour(WHITE);
				  Draw_String(10, 10, "mode:3", 16, RED,WHITE);
				  Draw_String(10, 26, "collecting", 16, RED,WHITE);	
				}		
			}
			start++;
      if(state==1)
			{
				heartbeat=HeartRate_get();
				ANO_DT_Send_heartbeat(heartbeat);
				
			}
      			
		}		
      delay_ms(10);		
		}
			
}


//================================================================================================================================================
float get_temp()
{
    int i;
	  float a[2];
	  for(i=0;i<=1;i++)
	  {
			a[i]=Get_Adc(ADC_Channel_1)-1;
		  a[i]=(float)a[i]*(3300.000/4096);
		  a[i]=-7.857923E-06*a[i]*a[i]-1.777501E-01*a[i]+2.046398E+02;
			delay_ms(1);
		}
		if(a[0]-a[1]>=2||a[0]-a[1]<=-2)
		{
			return get_temp();
		}
		else
		{
		  return (a[0]+a[1])/2;
		}
}



void ANO_DT_Send_ADS()//向ANO上位机发送传感器数据
{u8 i,sum=0,ac=0;	
		u8 data_to_send[60];//串口发送缓存
	  unsigned char _cnt=0;
		s32 cannle[2];	//存储两个通道的数据
		s32	p_Temp[2];	//数据缓存
   data_to_send[_cnt++]=0xAA;
   data_to_send[_cnt++]=0xFF;
	 data_to_send[_cnt++]=0xF1;	
	 data_to_send[_cnt++]=0;
					
							cannle[0]=ads1292_Cache[3]<<16 | ads1292_Cache[4]<<8 | ads1292_Cache[5];//获取原始数据		
							cannle[1]=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
						
							p_Temp[0] = get_volt(cannle[0]);	//把采到的3个字节转成有符号32位数
							p_Temp[1] = get_volt(cannle[1]);	//把采到的3个字节转成有符号32位数
					
							//有符号数为再转为无符号，无符号数为逻辑右移
							cannle[0] = p_Temp[0];
							cannle[1]	= p_Temp[1];
							/*data_to_send[4]=cannle[0]>>24;		//25-32位
							data_to_send[5]=cannle[0]>>16;  	//17-24
							data_to_send[6]=cannle[0]>>8;		//9-16
							data_to_send[7]=cannle[0]; 			//1-8*/
             
             data_to_send[_cnt++]=BYTE0(cannle[0]);
             data_to_send[_cnt++]=BYTE1(cannle[0]);
             data_to_send[_cnt++]=BYTE2(cannle[0]);
             data_to_send[_cnt++]=BYTE3(cannle[0]);
						 
						 data_to_send[_cnt++]=BYTE0(cannle[1]);
             data_to_send[_cnt++]=BYTE1(cannle[1]);
             data_to_send[_cnt++]=BYTE2(cannle[1]);
             data_to_send[_cnt++]=BYTE3(cannle[1]);
							
						data_to_send[3] = _cnt-4;

           for(i=0;i<_cnt;i++)
            {
							sum += data_to_send[i];
							ac +=sum;
						}
            data_to_send[_cnt++]=sum;
						data_to_send[_cnt++]=ac;
						
	      for( i=0;i<_cnt;i++)
	       {
	      USART_SendData(USART1, data_to_send[i]) ;
		    delay_us(100);
	       }
}


void ANO_DT_Send_Sensor(float temperature,int bushu,float distance)//向ANO上位机发送传感器数据
{   
		u8 data_to_send[60];//串口发送缓存
     
   unsigned char sum = 0,ac = 0;
    u16 temp;
    s32 temp2;
    unsigned char _cnt=0;
	unsigned char i=0;
	 data_to_send[_cnt++]=0xAA;
   data_to_send[_cnt++]=0xFF;
	 data_to_send[_cnt++]=0xF2;	
	 data_to_send[_cnt++]=0;
	//temp= (u16)(1);

    //data_to_send[_cnt++]=BYTE0(temp);
    //data_to_send[_cnt++]=BYTE1(temp);

    temp2=(s32)(temperature*100);

    data_to_send[_cnt++]=BYTE0(temp2);
    data_to_send[_cnt++]=BYTE1(temp2);
	  data_to_send[_cnt++]=BYTE2(temp2);
    data_to_send[_cnt++]=BYTE3(temp2);

    //temp= (u16)(2);

    //data_to_send[_cnt++]=BYTE0(temp);
    //data_to_send[_cnt++]=BYTE1(temp);
    temp= (u16)(bushu);

    data_to_send[_cnt++]=BYTE0(temp);
    data_to_send[_cnt++]=BYTE1(temp);

    //temp= (u16)(3);

    //data_to_send[_cnt++]=BYTE0(temp);
    //data_to_send[_cnt++]=BYTE1(temp);

    temp2=(s32) (distance*100);

    data_to_send[_cnt++]=BYTE0(temp2);
    data_to_send[_cnt++]=BYTE1(temp2);
	  data_to_send[_cnt++]=BYTE2(temp2);
    data_to_send[_cnt++]=BYTE3(temp2);

   

    data_to_send[3] = _cnt-4;

    for(i=0;i<_cnt;i++)
    {
				sum += data_to_send[i];
				ac +=sum;
		}
        data_to_send[_cnt++]=sum;
			  data_to_send[_cnt++]=ac;
	for( i=0;i<_cnt;i++)
	{
	  USART_SendData(USART1, data_to_send[i]) ;
		delay_us(100);
	}
				
				
}

void ANO_DT_Send_heartbeat(int heartbeat)//向ANO上位机发送传感器数据
{   
		u8 data_to_send[60];//串口发送缓存
     
   unsigned char sum = 0,ac = 0;
    u16 temp;
    s32 temp2;
    unsigned char _cnt=0;
	unsigned char i=0;
	 data_to_send[_cnt++]=0xAA;
   data_to_send[_cnt++]=0xFF;
	 data_to_send[_cnt++]=0xF3;	
	 data_to_send[_cnt++]=0;
	
    temp= (u16)(heartbeat);

    data_to_send[_cnt++]=BYTE0(temp);
    data_to_send[_cnt++]=BYTE1(temp);

    data_to_send[3] = _cnt-4;

    for(i=0;i<_cnt;i++)
    {
				sum += data_to_send[i];
				ac +=sum;
		}
        data_to_send[_cnt++]=sum;
			  data_to_send[_cnt++]=ac;
	for( i=0;i<_cnt;i++)
	{
	  USART_SendData(USART1, data_to_send[i]) ;
		delay_us(100);
	}
				
				
}
/*功能：把采到的3个字节转成有符号32位数 */
s32 get_volt(u32 num)
{		
			s32 temp;			
			temp = num;
			temp <<= 8;
			temp >>= 8;
	    //temp ^= 0x8000;
			return temp;
}

void F2S(float d,char *str)
{
	char str1[40];
	int j = 0,k,i;
	if(d == 0)
	{
		str[0]='0';
		str[1]='\0';
	}
	else
{
	i = (int)d;//浮点数的整数部分
	while(i > 0)
	{
		str1[j++] = i % 10 + '0';
		i = i / 10;
	}
	for(k = 0;k < j;k++)
	{
		str[k] = str1[j-1-k];//被提取的整数部分正序存放到另一个数组
	}
  d = d - (int)d;//小数部分提取
  if(d!=0)
	{
	str[j++] = '.';
	for(i = 0;i < 10;i++)
	{
		d = d*10;
		str[j++] = (int)d + '0';
		d = d - (int)d;
	}
	while(str[--j] == '0');
	str[++j] = '\0';
  }
	else
	str[j++] = '\0';
}
}

void ANO_DT_Send_temperature(float temperature)
{
	u8 data_to_send[60];//串口发送缓存
     
   unsigned char sum = 0;
    int16_t temp;

    unsigned char _cnt=0;
	unsigned char i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0xF1;
	data_to_send[_cnt++]=0x08;
	temp= temperature;

    data_to_send[_cnt++]=BYTE1(temp);
    data_to_send[_cnt++]=BYTE0(temp);


    data_to_send[3] = _cnt-5;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;
	for( i=0;i<_cnt;i++)
	{
	USART_SendData(USART1, data_to_send[i]) ;
		delay_us(100);
	}
}

void ANO_DT_Send_bushu(int bushu)
{
	u8 data_to_send[60];//串口发送缓存
     
   unsigned char sum = 0;
    int16_t temp;

    unsigned char _cnt=0;
	unsigned char i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0xF2;
	data_to_send[_cnt++]=0x08;
	temp= bushu;

    data_to_send[_cnt++]=BYTE1(temp);
    data_to_send[_cnt++]=BYTE0(temp);


    data_to_send[3] = _cnt-5;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;
	for( i=0;i<_cnt;i++)
	{
	USART_SendData(USART1, data_to_send[i]) ;
		delay_us(100);
	}
}

void ANO_DT_Send_juli(float juli)
{
	u8 data_to_send[60];//串口发送缓存
     
   unsigned char sum = 0;
    int16_t temp;

    unsigned char _cnt=0;
	unsigned char i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0xF3;
	data_to_send[_cnt++]=0x08;
	temp= juli;

    data_to_send[_cnt++]=BYTE1(temp);
    data_to_send[_cnt++]=BYTE0(temp);


    data_to_send[3] = _cnt-5;

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];

    data_to_send[_cnt++]=sum;
	for( i=0;i<_cnt;i++)
	{
	USART_SendData(USART1, data_to_send[i]) ;
		delay_us(100);
	}
}


