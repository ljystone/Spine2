#include "main.h"
#include "LCD1602.h"
#include "HX711.h"

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0,Weight_Shiwu = 0;
char Price_Count = 0;
unsigned char KEY_NUM = 0;
unsigned char Price_Buffer[3] = {0x00,0x00,0x00};
unsigned long Money = 0;
bit Flag_OK = 0;

//****************************************************
//主函数
//****************************************************
void main()
{
	Init_LCD1602();									//初始化LCD1602

	LCD1602_write_com(0x80);						//指针设置
	LCD1602_write_word("Welcome to use! ");			//开机画面第一行


	Delay_ms(2000);		 //延时2s


loop:Price_Count = 0;
	Price_Buffer[0] = 0;
	Price_Buffer[1] = 0;
	Price_Buffer[2] = 0;
	Flag_OK = 0;

	LCD1602_write_com(0x80);						//指针设置
	LCD1602_write_word("+WEI |PRI | MON ");
	LCD1602_write_com(0x80+0x40);				//指针设置
	LCD1602_write_word("0.000|  . |   . ");

	

	
	Get_Maopi();				//称毛皮重量

	while(1)
	{
		if( Flag_OK == 0)
		{
			Get_Weight();			//称重
	
			//显示当前重量
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_data(Weight_Shiwu/1000 + 0x30);
			LCD1602_write_data('.');
			LCD1602_write_data(Weight_Shiwu%1000/100 + 0x30);
			LCD1602_write_data(Weight_Shiwu%100/10 + 0x30);
			LCD1602_write_data(Weight_Shiwu%10 + 0x30);
		}

		KEY_NUM = KEY_Scan();

		if( KEY_NUM != 0x55)			//当返回的不是初值时候，确认按键按下。
		{
			if(KEY_NUM == 16)			//数字A键，去皮功能
			{
				Get_Maopi();			//去皮	
			}

			if(KEY_NUM == 15)			//数字B键清除键，二次测量
			{
				goto loop;	
			}

			if(KEY_NUM == 12)			//数字C输入单价错误时返回上一步
			{
				Price_Count--;
				if( Price_Count < 0)
				{
					Price_Count = 0;
				}
				
				Price_Buffer[Price_Count] = 0;			   //清除上一个输入的数据

				switch(Price_Count)
				{
					case 0:
							LCD1602_write_com(0x80+0x40+6);
							LCD1602_write_data(' ');
							break;
					case 1:
							LCD1602_write_com(0x80+0x40+7);
							LCD1602_write_data(' ');
							break;
					case 2:
							LCD1602_write_com(0x80+0x40+9);
							LCD1602_write_data(' ');
							break;
					default : break;
				}	
			}

			if(KEY_NUM == 13)			//数字D键，计算总价
			{
				Money = Price_Buffer[0] * 100 + Price_Buffer[1] * 10 + Price_Buffer[2];	
				Money = Money * Weight_Shiwu / 1000;
				LCD1602_write_com(0x80+0x40+11);
				LCD1602_write_data(Money/1000 + 0x30);			
				LCD1602_write_data(Money%1000/100 + 0x30);
				LCD1602_write_data(Money%100/10 + 0x30);
				LCD1602_write_data('.');
				LCD1602_write_data(Money%10 + 0x30);
				
				Flag_OK = 1;					
			}

			if(KEY_NUM >= 0 && KEY_NUM <= 9)					  //显示输入的价格值
			{
				Price_Buffer[Price_Count] = KEY_NUM;
	
				switch(Price_Count)
				{
					case 0:
							LCD1602_write_com(0x80+0x40+6);
							LCD1602_write_data(Price_Buffer[0] + 0x30);
							break;
					case 1:
							LCD1602_write_com(0x80+0x40+7);
							LCD1602_write_data(Price_Buffer[1] + 0x30);
							break;
					case 2:
							LCD1602_write_com(0x80+0x40+9);
							LCD1602_write_data(Price_Buffer[2] + 0x30);
							break;
					default : break;
				}
	
				Price_Count++;
	
				if( Price_Count >= 3)
				{
					Price_Count = 3;
				}
			}

		}

	}
}

//****************************************************
//矩阵键盘扫描
//****************************************************
unsigned char KEY_Scan()
{
	unsigned char temp = 0;
	unsigned char com = 0x55 , com1 = 0 , com2 = 0;
	P3=0xf0;
	if(P3!=0xf0)
	{
		com1=P3;
		P3=0x0f;
		com2=P3;        
	}
	P3=0xf0;
    while(P3!=0xf0);	  
	temp=com1|com2;
	if(temp==0xee)com=1;//数字1
	if(temp==0xed)com=4;//数字4
	if(temp==0xeb)com=7;//数字7
	if(temp==0xe7)com=11;//备用键*号键
	if(temp==0xd7)com=0;//数字0
	if(temp==0xb7)com=14;//备用键*号键
	

	if(temp==0xde)com=2; //数字2
	if(temp==0xdd)com=5; //数字5
	if(temp==0xdb)com=8;//数字8

	if(temp==0xbe)com=3;//数字3
	if(temp==0xbd)com=6; //数字6
	if(temp==0xbb)com=9;//数字9

	if(temp==0x7e)com=16;//数字A键，去皮功能	 
	if(temp==0x7d)com=15;//数字B键清除键，二次测量
	if(temp==0x7b)com=12;//数字C输入单价错误时返回上一步
	if(temp==0x77)com=13;//数字D键，计算总价	 
	return(com);
}

//****************************************************
//称重
//****************************************************
void Get_Weight()
{
	HX711_Buffer = HX711_Read();
	HX711_Buffer = HX711_Buffer/100;
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/4.22+0.05); 	//计算实物的实际重量
																		//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的4.30这个除数。
																		//当发现测试出来的重量偏大时，增加该数值。
																		//如果测试出来的重量偏小时，减小改数值。
																		//该数值一般在4.0-5.0之间。因传感器不同而定。
																		//+0.05是为了四舍五入百分位
		Buzzer = 1;				//关闭警报
	}
//	else if(HX711_Buffer < Weight_Maopi - 30)
//	{
//		Buzzer = 0;				//负重量报警
//	}
//	else if(HX711_Buffer > Weight_Maopi + 24970)		//大于5Kg的最大量程，报警	
//	{
//		Buzzer = 0;
//	}

	if(Weight_Shiwu > 5000 || HX711_Buffer < Weight_Maopi - 30)
	{
		Buzzer = 0;	
	}
	else
	{
		Buzzer = 1;
	}

	
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	HX711_Buffer = HX711_Read();
	Weight_Maopi = HX711_Buffer/100;		
} 

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}