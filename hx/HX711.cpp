#include "hx711.h"

long HX711_Buffer = 0;
long Weight_Maopi = 0,Weight_Shiwu = 0;
//右侧
long HX711_BufferR= 0;
long Weight_MaopiR= 0,Weight_ShiwuR= 0;

#define GapValue 430

//****************************************************
//初始化HX711
//****************************************************
void Init_Hx711()
{
	pinMode(HX711_SCK, OUTPUT);	
	pinMode(HX711_DT, INPUT);
	//右侧
	pinMode(HX711_SCKR, OUTPUT);	
	pinMode(HX711_DTR, INPUT);
}


//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();		
} 
void Get_MaopiR()
{
	Weight_MaopiR = HX711_ReadR();		
} 
//****************************************************
//称重
//****************************************************
long Get_Weight()
{
	HX711_Buffer = HX711_Read();
	Weight_Shiwu = HX711_Buffer;
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	Weight_Shiwu = (long)((float)Weight_Shiwu/GapValue); 	
	return Weight_Shiwu;
}

long Get_WeightR()
{
	HX711_BufferR = HX711_ReadR();
	Weight_ShiwuR = HX711_BufferR;
	Weight_ShiwuR = Weight_ShiwuR - Weight_MaopiR;				//获取实物的AD采样数值。
	Weight_ShiwuR = (long)((float)Weight_ShiwuR/GapValue); 	
	return Weight_ShiwuR;
}
//****************************************************
//读取HX711
//****************************************************
unsigned long HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i;
	bool Flag = 0;

	digitalWrite(HX711_DT, HIGH);
	delayMicroseconds(1);

	digitalWrite(HX711_SCK, LOW);
	delayMicroseconds(1);

  	count=0; 
  	while(digitalRead(HX711_DT)); 
  	for(i=0;i<24;i++)
	{ 
	  	digitalWrite(HX711_SCK, HIGH); 
		delayMicroseconds(1);
	  	count=count<<1; 
		digitalWrite(HX711_SCK, LOW); 
		delayMicroseconds(1);
	  	if(digitalRead(HX711_DT))
			count++; 
	} 
 	digitalWrite(HX711_SCK, HIGH); 
	count ^= 0x800000;
	delayMicroseconds(1);
	digitalWrite(HX711_SCK, LOW); 
	delayMicroseconds(1);
	
	return(count);
}
//右侧读函数
unsigned long HX711_ReadR(void)	//增益128
{
	unsigned long count; 
	unsigned char i;
	bool Flag = 0;

	digitalWrite(HX711_DTR, HIGH);
	delayMicroseconds(1);

	digitalWrite(HX711_SCKR, LOW);
	delayMicroseconds(1);

  	count=0; 
  	while(digitalRead(HX711_DTR)); 
  	for(i=0;i<24;i++)
	{ 
	  	digitalWrite(HX711_SCKR, HIGH); 
		delayMicroseconds(1);
	  	count=count<<1; 
		digitalWrite(HX711_SCKR, LOW); 
		delayMicroseconds(1);
	  	if(digitalRead(HX711_DTR))
			count++; 
	} 
 	digitalWrite(HX711_SCKR, HIGH); 
	count ^= 0x800000;
	delayMicroseconds(1);
	digitalWrite(HX711_SCKR, LOW); 
	delayMicroseconds(1);
	
	return(count);
}
