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
//������
//****************************************************
void main()
{
	Init_LCD1602();									//��ʼ��LCD1602

	LCD1602_write_com(0x80);						//ָ������
	LCD1602_write_word("Welcome to use! ");			//���������һ��


	Delay_ms(2000);		 //��ʱ2s


loop:Price_Count = 0;
	Price_Buffer[0] = 0;
	Price_Buffer[1] = 0;
	Price_Buffer[2] = 0;
	Flag_OK = 0;

	LCD1602_write_com(0x80);						//ָ������
	LCD1602_write_word("+WEI |PRI | MON ");
	LCD1602_write_com(0x80+0x40);				//ָ������
	LCD1602_write_word("0.000|  . |   . ");

	

	
	Get_Maopi();				//��ëƤ����

	while(1)
	{
		if( Flag_OK == 0)
		{
			Get_Weight();			//����
	
			//��ʾ��ǰ����
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_data(Weight_Shiwu/1000 + 0x30);
			LCD1602_write_data('.');
			LCD1602_write_data(Weight_Shiwu%1000/100 + 0x30);
			LCD1602_write_data(Weight_Shiwu%100/10 + 0x30);
			LCD1602_write_data(Weight_Shiwu%10 + 0x30);
		}

		KEY_NUM = KEY_Scan();

		if( KEY_NUM != 0x55)			//�����صĲ��ǳ�ֵʱ��ȷ�ϰ������¡�
		{
			if(KEY_NUM == 16)			//����A����ȥƤ����
			{
				Get_Maopi();			//ȥƤ	
			}

			if(KEY_NUM == 15)			//����B������������β���
			{
				goto loop;	
			}

			if(KEY_NUM == 12)			//����C���뵥�۴���ʱ������һ��
			{
				Price_Count--;
				if( Price_Count < 0)
				{
					Price_Count = 0;
				}
				
				Price_Buffer[Price_Count] = 0;			   //�����һ�����������

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

			if(KEY_NUM == 13)			//����D���������ܼ�
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

			if(KEY_NUM >= 0 && KEY_NUM <= 9)					  //��ʾ����ļ۸�ֵ
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
//�������ɨ��
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
	if(temp==0xee)com=1;//����1
	if(temp==0xed)com=4;//����4
	if(temp==0xeb)com=7;//����7
	if(temp==0xe7)com=11;//���ü�*�ż�
	if(temp==0xd7)com=0;//����0
	if(temp==0xb7)com=14;//���ü�*�ż�
	

	if(temp==0xde)com=2; //����2
	if(temp==0xdd)com=5; //����5
	if(temp==0xdb)com=8;//����8

	if(temp==0xbe)com=3;//����3
	if(temp==0xbd)com=6; //����6
	if(temp==0xbb)com=9;//����9

	if(temp==0x7e)com=16;//����A����ȥƤ����	 
	if(temp==0x7d)com=15;//����B������������β���
	if(temp==0x7b)com=12;//����C���뵥�۴���ʱ������һ��
	if(temp==0x77)com=13;//����D���������ܼ�	 
	return(com);
}

//****************************************************
//����
//****************************************************
void Get_Weight()
{
	HX711_Buffer = HX711_Read();
	HX711_Buffer = HX711_Buffer/100;
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/4.22+0.05); 	//����ʵ���ʵ������
																		//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������4.30���������
																		//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
																		//������Գ���������ƫСʱ����С����ֵ��
																		//����ֵһ����4.0-5.0֮�䡣�򴫸�����ͬ������
																		//+0.05��Ϊ����������ٷ�λ
		Buzzer = 1;				//�رվ���
	}
//	else if(HX711_Buffer < Weight_Maopi - 30)
//	{
//		Buzzer = 0;				//����������
//	}
//	else if(HX711_Buffer > Weight_Maopi + 24970)		//����5Kg��������̣�����	
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
//��ȡëƤ����
//****************************************************
void Get_Maopi()
{
	HX711_Buffer = HX711_Read();
	Weight_Maopi = HX711_Buffer/100;		
} 

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}