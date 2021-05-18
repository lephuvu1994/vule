#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stdint.h"
//////////////////////////////////////////////////////////////////////////////////	 

//SysTick STM32F10x
//delay_us,delay_ms

////////////////////////////////////////////////////////////////////////////////// 
	 
void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint16_t nus);

#endif





























