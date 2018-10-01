int pin = 2; //定义引脚为D2
unsigned long duration=0; //定义duration变量为无符号长整数型变量
int countI = 0;
void setup()
{
Serial.begin(9600); //串口波特率为9600
pinMode(pin, INPUT); //设置引脚为输入模式
Serial.println("begin"); 
}
void loop()
{
  if(digitalRead(2)==0)
  {
        Serial.println(countI++);
        delay(50);
  } 
}
