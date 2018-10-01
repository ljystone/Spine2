#include "HX711.h"
float WeightL = 0;
float WeightR = 0;
//左侧板子SCK 3   DT  2   右侧板子  SCK  9  DT  8
int pinL = 4 ;//左侧位移引脚   D0=4
int pinR = 7 ;//右侧位移引脚   D0=7
int durationL=0;
int durationR=0;
int alldurationR=0;
int alldurationL=0;
void setup()
{
	Init_Hx711();				//初始化HX711模块连接的IO设置
	Serial.begin(9600);
  pinMode(pinL, INPUT);
  pinMode(pinR, INPUT);
	Get_Maopi();		//获取毛皮
  Get_MaopiR();
}

void loop()
{
  /*for(int i=0;i<5;i++)
  {
      durationL += pulseIn(pinL, HIGH); 
      durationL += pulseIn(pinL, LOW); 
      durationR += pulseIn(pinR, HIGH); 
      durationR += pulseIn(pinR, LOW); 
  }
    durationR = durationR/5;
    durationR=1000000/durationR;
    alldurationR+=durationR;
    durationL = durationL/5;
    durationL=1000000/durationL;
    alldurationL+=durationL;*/
    
	WeightL = Get_Weight()*9.8;	//
Serial.println(float(WeightL)/1000);
 if(WeightL>10000)
 {}
 else
 {
	 // Serial.print(float(WeightL)/1000);	//串口显示重量
   // Serial.print("#");
 }

  WeightR = Get_WeightR()*9.8;
  if(WeightR>10000)
  {
    
 }else
 {
    Serial.println(float(WeightR)/1000);
   // Serial.print("#");  
  }
 // Serial.print(alldurationL);
	//Serial.print("#");
 // Serial.print(alldurationR);
//Serial.print("!");
	
	
}
