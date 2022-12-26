#include "stm32f10x.h"
#include "open.h"
#include "adc.h"

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;

int delta_speed=10;
const int turnLeftFirst = 1;
const int turnRightFirst = 2;
const int turnBackFirst = 0;

const int turnLeft = 3;
const int turnRight = 2;
const int turnBack = 0;
const int turnTryLeft = 5;
const int turnTryRight = 4;

const int PE8 = 0;
const int PE10 = 1;
const int PE12 = 2;

const int red = 1;
const int green = 2;
const int blue = 3;
int flag=0;

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal;  
void RTX_Off(void);
/********************************************
*函数名：int sensor(void)
*函数功能: 通过比较3中颜色下，电压最大的值来暂定此时识别的相应颜色
*函数参数说明：void
*函数返回值：1-blue 2-red 3-green
*******************************************/
int sensor(void)
{
	float bl;
	float re;
	float gr;
	int val;
	float max;
	RTX_Off();
	
	//测蓝光返回的电压值
	GPIO_ResetBits(GPIOE,GPIO_Pin_12);
	delay_nus(3333);
	bl=(float) ADC_ConvertedValue/4096.0*3.3;
//	printf("\r\n blue The current AD value = %f V \r\n",bl); 
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
	
		val=1;
		max=bl;
		delay_nus(100);
	//测红光返回的电压值
	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	delay_nus(3333);
	re=(float) ADC_ConvertedValue/4096.0*3.3;
//	printf("\r\n red The current AD value = %f V \r\n",re); 
	GPIO_SetBits(GPIOE,GPIO_Pin_8);
		if(re>max)
		{
			val=2;
			max=re;
		}
		delay_nus(100);
		//测绿光返回的电压值
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
	delay_nus(3333);
	gr =(float) ADC_ConvertedValue/4096.0*3.3*1.027;
//	printf("\r\n green The current AD value = %f V \r\n",gr); 
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
		if(gr>max)
		{
			val=3;
			max=gr;
		}
		delay_nus(100);
//	printf("* blue=%f,red=%f,green=%f\r\n",bl,re,gr);
//	printf("\r\n the current return %d \r\n ",val);
	return val;
}
/********************************************
*函数名：int judge(void)
*函数功能：用于通过读入sensor函数返回的值来，通过某个颜色出现的次数来判断识别的颜色
*函数参数说明：void
*函数返回值：1-blue 2-red 3-green 0-不能判断
*******************************************/
int judge(void)
{
	int cur; //保存当前获取的颜色 1-blue 2-red 3-green
	int blue=0;
	int red=0;
	int green=0;
	int i;
	for(i=0;i<10;i++)
	{
		cur=sensor();
		switch(cur)
		{
			case 1 :
			blue++;
			break;
			case 2 :
			red++;
			break;
			case 3 :
			green++;
			break  ;
			default :
			printf("\r\n wrong return \r\n");
			break;
		}
	}
	printf("blue=%d,red=%d,green=%d ",blue,red,green);
	if(blue>0&&red>3&&green==0)
	{
		return 4;//绿色
	}
	if(blue>0&&red>0&&green>0)
	{
		return 5;//白色
	}
//	if(blue>4 )
//	{
//		return 1;
//	}
//	if(red>5)  
//	{
//		return 2;
//	}
//	if(green>5)
//	{		
//		return 3;
//	}
	else
	{
		return 0;
	}
}

void isClear(int* left, int* right)
{
	*left = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5);
	*right = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);
}


void forward(int time)//参数越大，直走越远。
{
		int i,left, right;
		for(i=0;i<time*50;i++)
	{
		
			isClear(&left, &right);
			if(left == 0 || right == 0) return;
		GPIO_SetBits(GPIOC,GPIO_Pin_4);
		delay_nus(1900+delta_speed*10);
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	
		GPIO_SetBits(GPIOC,GPIO_Pin_3);
		delay_nus(1900-delta_speed*10);
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);
		
		delay_nus(20000);
	}
}

void drawback(int time)//参数越大，后退越远。
{
		int i,left, right;
		for(i=0;i<time;i++)
	{
		
			isClear(&left, &right);
			if(left == 0 || right == 0) return;
		GPIO_SetBits(GPIOC,GPIO_Pin_4);
		delay_nus(1900-delta_speed*10);
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	
		GPIO_SetBits(GPIOC,GPIO_Pin_3);
		delay_nus(1900+delta_speed*10);
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);
		
		delay_nus(20000);
	}
}

void halt(void)
{
		GPIO_SetBits(GPIOC,GPIO_Pin_4);
		delay_nus(1900);
		GPIO_ResetBits(GPIOC,GPIO_Pin_4);
	
		GPIO_SetBits(GPIOC,GPIO_Pin_3);
		delay_nus(1900);
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);
		
		delay_nus(20000);
}
void turn(int direction)//参数代表旋转方向：左/右/后
{
	int i;
	switch(direction)
	{
		case(turnLeft):
				for(i=0;i<20;i++)
				{
					//1.9ms时轮胎不动
					GPIO_SetBits(GPIOC,GPIO_Pin_3);
					delay_nus(1800);
					GPIO_ResetBits(GPIOC,GPIO_Pin_3);
					delay_nus(20000);
					
					GPIO_SetBits(GPIOC,GPIO_Pin_4);
					delay_nus(1800);	
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					delay_nus(20000);
				}
				break;
			
		case(turnRight):
				for(i=0;i<18;i++)
				{
					//1.9ms时轮胎不动
					GPIO_SetBits(GPIOC,GPIO_Pin_3);
					delay_nus(2000);
					GPIO_ResetBits(GPIOC,GPIO_Pin_3);
					delay_nus(20000);
					
					GPIO_SetBits(GPIOC,GPIO_Pin_4);
					delay_nus(2100);	
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					delay_nus(20000);
				}
				break;
			
		case(turnBack):
				for(i=0;i<36;i++)
				{
					//1.9ms时轮胎不动
					GPIO_SetBits(GPIOC,GPIO_Pin_3);
					delay_nus(1800);
					GPIO_ResetBits(GPIOC,GPIO_Pin_3);
					delay_nus(20000);
					
					GPIO_SetBits(GPIOC,GPIO_Pin_4);
					delay_nus(1800);	
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					delay_nus(20000);
				}
				break;
		case(turnTryLeft):
				for(i=0;i<3;i++)
				{
					//1.9ms?????
					GPIO_SetBits(GPIOC,GPIO_Pin_3);
					delay_nus(1800);
					GPIO_ResetBits(GPIOC,GPIO_Pin_3);
					delay_nus(20000);
					
					GPIO_SetBits(GPIOC,GPIO_Pin_4);
					delay_nus(1800);	
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					delay_nus(20000);
				}
				break;
			
		case(turnTryRight):
				for(i=0;i<3;i++)
				{
					//1.9ms?????
					GPIO_SetBits(GPIOC,GPIO_Pin_3);
					delay_nus(2000);
					GPIO_ResetBits(GPIOC,GPIO_Pin_3);
					delay_nus(20000);
					
					GPIO_SetBits(GPIOC,GPIO_Pin_4);
					delay_nus(2100);	
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					delay_nus(20000);
				}
				break;
	}
}
void RTX_Off(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_8);
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
}
void RTX_On(int type, int time)
//第一个参数代表使用的引脚：PE8/PE10/PE12
//第二个参数代表亮灯时间，单位是毫秒
//接线应当按照R/G/B--8/10/12顺序
{
	switch(type)
	{
		case(PE8):
			RTX_Off();
			delay_nms(time);
			GPIO_ResetBits(GPIOE,GPIO_Pin_8);
			delay_nms(time);
			break;
		
		case(PE10):
			RTX_Off();
			delay_nms(time);
			GPIO_ResetBits(GPIOE,GPIO_Pin_10);
			delay_nms(time);
			break;
		
		case(PE12):
			RTX_Off();
			delay_nms(time);
			GPIO_ResetBits(GPIOE,GPIO_Pin_12);
			delay_nms(time);
			break;
	}
}


int Timer;
int cur_color=0, tmp=0,tmp0=0,try_times=0;
int hadturnback=0;
void autoPilot(int type)
{
		
	
		while(1)
		{
			int left=1;
			int right=1;
			isClear(&left, &right);
			if(left == 0 || right == 0)
			{
				switch(type)
				{
					case(turnLeftFirst):
						turn(hadturnback^turnRight);
						forward(1);
						turn(hadturnback^turnLeft);
						turn(hadturnback^turnTryRight);
						break;
//						turn(turnLeft);
//						isClear(&left, &right);
//						if(left == 1 && right == 1)
//							break;
//						turn(turnBack);
//						isClear(&left, &right);
//						if(left == 1 && right == 1)
//							break;
//						turn(turnRight);
//						break;
//					
//					case(turnRightFirst):
//						turn(turnRight);
//						isClear(&left, &right);
//						if(left == 1 && right == 1)
//							break;
//						turn(turnBack);
//						isClear(&left, &right);
//						if(left == 1 && right == 1)
//							break;
//						turn(turnLeft);
//						break;
//					
//					case(turnBackFirst):
//						turn(turnBack);
//						break;
						
				}
			}
			
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
			delay_nus(1900+delta_speed*10);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
		
			GPIO_SetBits(GPIOC,GPIO_Pin_3);
			delay_nus(1900-delta_speed*10);
			GPIO_ResetBits(GPIOC,GPIO_Pin_3);
			
			delay_nus(20000);
			
			Timer++;
			Timer %= 10;
			if( Timer == 0 )
			{
				tmp = judge();
				if(cur_color!=tmp)
				{
					//delay_nus(1000);
					tmp=judge();
				}
				if(cur_color!=4 && tmp == 4 && hadturnback == 0) 
					turn(turnBack), hadturnback = 1;
				if(cur_color!=5 && tmp == 5)
				{
					try_times = 0;
					while( tmp==5 && try_times<5 ){
						turn(hadturnback^turnTryLeft);
						drawback(2);
						tmp = judge();
						try_times++;
					}
					try_times = 0;
					while( tmp==5 && try_times<5 ){
						turn(hadturnback^turnTryRight);
						drawback(2);
						tmp = judge();
						try_times++;
					}
				}
				printf("cur_color: %d->%d\r\n", tmp, cur_color);
				//if( tmp!=0 ) 
					cur_color = tmp;
				
				break;
			}
		}
}





int main(void)
{
	Open_Init();
// ADC 初始化
	ADC1_Init();

	printf("\r\n ----这是一个ADC实验(DMA传输)-颜色识别---\r\n");
	while(1)
	{
		autoPilot(turnLeftFirst);
	}
}

