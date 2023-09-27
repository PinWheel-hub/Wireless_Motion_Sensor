#include "Timer.h"
#include "led.h" 
#include "key.h"
#include "ADS1292.h"
#include "exti.h"
#include "delay.h"
//�߼� TIM1 TIM8
//ͨ�� TIM2 TIM3 TIM4 TIM5
//���� TIM6 TIM7

//��ʱ��5��ʼ��
void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM5ʱ��ʹ�� 
	 
		TIM_TimeBaseStructure.TIM_Period = arr-1;; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
		TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����жϷ���
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM5�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure); 
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //��������ж�����λ
		TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );//TIM5 �������										
		TIM_Cmd(TIM2, ENABLE); 
}




void TIM2_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //�����ж� 
		{ 
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//���жϱ�־
				
		}				   
}




//��ʱ����ʼ��
//������ʱ�� +��������
void TIM3_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ�� 
	 
		TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
		TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����жϷ���
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure); 
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //��������ж�����λ
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );//�������										
		TIM_Cmd(TIM3, ENABLE); 
}

//��ʱ���жϷ������	 
void TIM3_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //�����ж� 
		{ 
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//���жϱ�־		
				EXTI->IMR |= EXTI_Line9;//ʹ���ⲿ�ж�
				TIM_Cmd(TIM3, DISABLE);
				TIM3->CNT=0;//��ն�ʱ��������
					
		}				   
}







static void OpenTimerForHc(void) ;
static void CloseTimerForHc(void) ;    


u32 msHcCount = 0;//ms??  


/********************************************************
* Function:TIM5_NVIC
* Description:???????
* Input:?
* Return:?
* Others:?
**********************************************************/
void TIM5_NVIC(void)  
{  
      NVIC_InitTypeDef NVIC_InitStructure;  
      //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
          
      NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 	
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //???????????1  
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //???????????1  
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //????  
      NVIC_Init(&NVIC_InitStructure);  
}  


/********************************************************
* Function:TIM5_Init
* Description:???4???
* Input:?
* Return:?
* Others:?
**********************************************************/
void TIM5_Init(u16 arr,u16 psc)  
{    
	
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //?????????????  
	
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);   //????RCC??  
            //??????????  
     //TIM_DeInit(HCSR04_TIM);  
     TIM_TimeBaseStructure.TIM_Period = arr-1; //???????????????????????????         ???1000?1ms  
     TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //??????TIMx???????????  1M????? 1US??  
     TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//???  
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????  
     TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????         
              
     TIM_ClearFlag(TIM5, TIM_FLAG_Update);   //??????,?????????????  
     TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);    //?????????  
     TIM5_NVIC();  
     TIM_Cmd(TIM5,DISABLE);       
}  
 

/********************************************************
* Function:OpenTimerForHc
* Description:?????
* Input:?
* Return:?
* Others:?
**********************************************************/   
static void OpenTimerForHc(void)        //?????  
{  
   TIM_SetCounter(TIM5,0);//????  
   msHcCount = 0;  
   TIM_Cmd(TIM5, ENABLE);  //??TIMx??  
}  

/********************************************************
* Function:CloseTimerForHc
* Description:?????
* Input:?
* Return:?
* Others:?
**********************************************************/     
static void CloseTimerForHc(void)        //?????  
{  
   TIM_Cmd(TIM5, DISABLE);  //??TIMx??  
}  
            
 /**
 * @brief  ???3??????
 * @param  ?
 * @retval ?
 */ 
 
/********************************************************
* Function:TIM5_IRQHandler
* Description:???3?????
* Input:?
* Return:?
* Others:?
**********************************************************/
void TIM5_IRQHandler(void)   //TIM6??  
{  
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //??TIM6????????  
     {  
       TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //??TIM6??????   
        msHcCount++;  
			 
     } 	 
}  

u32 GetEchoTimer(void)  
{  
    u32 t = 0;  
    t = msHcCount*1000;//??MS  
    t += TIM_GetCounter(TIM5);//??US  
    TIM3->CNT = 0;  //?TIM6???????????  
	  delay_ms(50);
     return t; 
} 
    
/********************************************************
* Function:HeartRate_get
* Description:????
* Input:?w
* Return:?
* Others:
**********************************************************/ 
float HeartRate_get(void)  
{  
   
     u32 cannle=0;
     u32 time[30]={0};
     u32 x[30]={0};
		 u32 sum;  
		 u32 sh1=0;
		 u32 sh2=0;
		 u32 ch1;
		 u32 ch2;
     int i = 0,m=0,j=0;    
     float ave= 0; //???
		 float fa=0;
		 float fa1=0;
		 float fa2=0;
	   float HeartRate=0;
		 TIM5_Init(1000,72);
		     OpenTimerForHc();        //?????
		         /*for(i=0;i<1000;i++)
							{
								cannle=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
								//ave=ave/(i+1)*i+(float)cannle/(i+1);
								if(j==0)
							{
								ch1=cannle;
								j=1;
								fa1=fa1/(i+1)*i+(float)abs(ch1-ch2)/(i+1);
							}
							if(j==1)
							{
								ch2=cannle;
								j=0;
								fa2=fa2/(i+1)*i+(float)abs(ch1-ch2)/(i+1);
							}
								delay_us(10);
								
							}
							fa=(fa1+fa2)/2;
							printf("fa:%d\r\n",(int)fa);*/
         while(state==1)
				 {		
					    delay_ms(16);
							cannle=ads1292_Cache[6]<<16 | ads1292_Cache[7]<<8 | ads1292_Cache[8];
					    sh2=sh1;
					    sh1=cannle;
					 //printf("cannle :%d\r\n",cannle); 
					 //printf("sh1 :%d\r\n",sh1);
					 //printf("sh2:%d\r\n",sh2);
					    if(sh2>sh1+10000)
							{
								time[m++]=GetEchoTimer();
								//printf("time:%d   m %d\r\n",time[m-1],m);
							}
							
					    /*if(i==0)
							{
								sh1=cannle;
								i=1;
								if(sh1>sh2+fa||sh1+fa<sh2)
								{ time[m++]=GetEchoTimer();
									printf("time:%d   m %d\r\n",time[m-1],m);
									delay_ms(10);
								}
							}
							if(i==1)
							{
								sh2=cannle;
								i=0;
								if(sh1>sh2+fa||sh1+fa<sh2)
								  { time[m++]=GetEchoTimer(); 
									printf("time:%d   m %d\r\n",time[m-1],m);
										delay_ms(10);
								}
							}*/
								 							
						
							/*if(cannel>ave)
							{
								time[m++]=GetEchoTimer();        //????,????1US 
								delay_ms(100);
							}*/
							
							
					   /* if(i!=0&&cannl/eave>)
							{
								time[m++]=GetEchoTimer();        //????,????1US 
								delay_ms(100);
								i=0;
							}
							else
							{
								printf("i=%d\r\n",i);
								printf("cannle:%d\r\n",cannle);
					     ave=ave/(i+1)*i+(float)cannle/(i+1);
								printf("ave:%d\r\n",(int)ave);
								i=i+1;
							}*/
				 }		 
         CloseTimerForHc();        //????? 
				 sum=0;
				 for(i=1;i<m;i++)
				 {
					 x[i-1]=time[i]-time[i-1];
					 sum+=x[i-1];
					 //printf("x%d %d\r\n",i-1,x[i-1]);
				 }
				 //printf("%d    %d\r\n",sum,m);
				 if(sum)
				 {
					 
				 HeartRate=60*1000000*(m-1)/sum;//adc����Ƶ��239/14MHZ
				   
					 //printf("rate: %d\r\n",(int)HeartRate);
				 } 			 
		 return(HeartRate);
}
