#ifndef __HX711__H__
#define __HX711__H__

#include <Arduino.h>

#define HX711_SCK 3
#define HX711_DT 2

#define HX711_SCKR 9
#define HX711_DTR  8

extern void Init_Hx711();
extern unsigned long HX711_Read(void);
extern long Get_Weight();
extern void Get_Maopi();
//右侧
extern unsigned long HX711_ReadR(void);
extern long Get_WeightR();
extern void Get_MaopiR();

#endif
