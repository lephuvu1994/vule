#include "i2c_mem.h"  
#include "gp_drive.h"
#include "stm32f1xx.h"
#include "delay.h"
#include "stdint.h"
void IIC_Init(void){
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;			    //LED0-->PB.5 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_PP; 	 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_50MHz;	 
//  GPIO_Init(GPIOB, &GPIO_InitStructure);			     //GPIOB.5
	init_GP(PA, IIC_SCL, OUT50, O_GP_PP);
	init_GP(PA, IIC_SDA, OUT50, O_GP_PP);
	
	W_GP(PA, IIC_SCL, 1);																//SCL =1
	W_GP(PA, IIC_SDA, 1);																//SDA_IN out =1
}

void IIC_Start(void){
	SDA_OUT();
//	IIC_SCL=1;
	W_GP(PA, IIC_SCL, 1);
//	IIC_SDA =1;
	W_GP(PA, IIC_SDA, 1);
	delay_us(4);
//	IIC_SDA =0;
	W_GP(PA, IIC_SDA, 0);
	delay_us(4);
//	IIC_SCL=0;	
	W_GP(PA, IIC_SCL, 0);
}

void IIC_Stop(void){	
	SDA_OUT();
//	IIC_SCL=0;
	W_GP(PA, IIC_SCL, 0);
//	IIC_SDA =0;
	W_GP(PA, IIC_SDA, 0);
	delay_us(4);
//	IIC_SDA =1;
//	IIC_SCL=1;
	W_GP(PA, IIC_SDA, 1);
//	IIC_SDA =1;
	W_GP(PA, IIC_SCL, 1);
	delay_us(4);		
}

void IIC_Send_Byte(uint8_t txd){//11111111&10000000
	int i=0;
	SDA_OUT();
//	IIC_SCL=0;
	W_GP(PA, IIC_SCL, 0);
	for(i=0;i<8;i++){
//		IIC_SDA=(txd&0x80)>>7;
		W_GP(PA, IIC_SDA,(txd&0x80)>>7);
		txd<<=1;
		delay_us(50);
//		IIC_SCL=1;
		W_GP(PA, IIC_SCL, 1);
		delay_us(50);
//		IIC_SCL=0;
		W_GP(PA, IIC_SCL, 0);
		
	}
	
}


void IIC_Ack(void){
//	IIC_SCL=0;
	W_GP(PA, IIC_SCL, 0);														//SCL =0
	SDA_OUT();
//	IIC_SDA=0;
	W_GP(PA, IIC_SDA, 0);														//SDA =0
	delay_us(5);
//	IIC_SCL=1;
	W_GP(PA, IIC_SCL, 1);														//SCL =1
	delay_us(5);
//	IIC_SCL=0;
	W_GP(PA, IIC_SCL, 0);														//SCL =0
}					
void IIC_NAck(void){
//	IIC_SCL=0;
	W_GP(PA, IIC_SCL, 0);														//SCL =0
	SDA_OUT();
//	IIC_SDA=1;
	W_GP(PA, IIC_SDA, 1);														//SDA =1
	delay_us(5);
//	IIC_SCL=1;
	W_GP(PA, IIC_SCL, 1);														//SCL =1
	delay_us(5);
//	IIC_SCL=0;
	W_GP(PA, IIC_SCL, 0);														//SCL =0
}	

uint8_t IIC_Read_Byte(unsigned char ack){
	int i=0;
	uint8_t rec=0;
	SDA_IN();
	for(i=0;i<8;i++){//11111111
//		IIC_SCL=0;
		W_GP(PA, IIC_SCL, 0);														//SCL =0
		delay_us(5);
//		IIC_SCL=1;
		W_GP(PA, IIC_SCL, 1);														//SCL =1
		delay_us(5);
		rec<<=1;
		if(READ_SDA) {rec++;}
	}
	if(!ack){
		IIC_Ack();
	}
		else{
		IIC_NAck();	
		}
	return rec;
}

uint8_t IIC_Wait_Ack(void){
	uint8_t time=0;
	SDA_IN();
//	IIC_SDA=1;
	W_GP(PA, IIC_SDA, 1);														//SDA =0
	delay_us(1);
//	IIC_SCL=1;
	W_GP(PA, IIC_SCL, 1);														//SCL =1
	delay_us(1);
	while(READ_SDA){
		time++;
		if(time>250) {IIC_Stop(); return 1;}
	}
//	IIC_SCL=0;
	W_GP(PA, IIC_SCL, 0);														//SCL =0
	return 0; 
}

void IIC_Cmd_Write(uint8_t add ,uint8_t reg,uint8_t data){
	IIC_Start();
	
	IIC_Send_Byte(add|0);
	while(IIC_Wait_Ack());
	IIC_Send_Byte(reg);
	while(IIC_Wait_Ack());
	IIC_Send_Byte(data);
	while(IIC_Wait_Ack());
	
	IIC_Stop();
	delay_ms(50);
}
//
void IIC_Write_CMD(uint8_t addr,uint8_t reg){
	IIC_Start();
	
	IIC_Send_Byte(addr|0);
	if(IIC_Wait_Ack())
	IIC_Send_Byte(reg);
	if(IIC_Wait_Ack())
	
	IIC_Stop();
	delay_ms(50);
}


uint8_t IIC_Write(uint8_t addr,uint8_t reg,uint8_t data){
	IIC_Start();
	
	IIC_Send_Byte(addr|0);
	if(IIC_Wait_Ack()){IIC_Stop(); return 1;}
	IIC_Send_Byte(reg);
	if(IIC_Wait_Ack()){IIC_Stop(); return 1;}
	IIC_Send_Byte(data);
	if(IIC_Wait_Ack()){IIC_Stop(); return 1;}
	
	IIC_Stop();
	delay_ms(50);
	return 0;
}

uint8_t Read_IIC(uint8_t addr, uint8_t reg){
	IIC_Start();
	IIC_Send_Byte(addr|0);
	while(IIC_Wait_Ack());
	IIC_Send_Byte(reg);
	while(IIC_Wait_Ack());
	delay_us(180);
	
	IIC_Start();
	IIC_Send_Byte(addr|1);
	while(IIC_Wait_Ack());
	reg=IIC_Read_Byte(1);																				//doc du lieu tu slave, ket thuc truyen bit NACK=1
	IIC_Stop();
	delay_ms(50);
	return reg;
}

uint8_t IIC_ReadMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
	
	IIC_Start();
	IIC_Send_Byte(addr|0);
	while(IIC_Wait_Ack());
	IIC_Send_Byte(reg);
	while(IIC_Wait_Ack());
	delay_us(180);
	
	IIC_Start();
	IIC_Send_Byte(addr|1);
	while(IIC_Wait_Ack());
	delay_us(200);
	
	
	while(len){
		if(len==1) *buf= IIC_Read_Byte(1);
		else *buf= IIC_Read_Byte(0);
		len = len -1;
		buf++;
	}	
	
	IIC_Stop();
	return 0;

}//

//
uint8_t IIC_WriteMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf){
	uint8_t i=0;
	IIC_Start();
	IIC_Send_Byte(addr|0);
	while(IIC_Wait_Ack());
	IIC_Send_Byte(reg);
	while(IIC_Wait_Ack());
	
	for(i=0;i<len;i++){
		IIC_Send_Byte(buf[i]);
		if(IIC_Wait_Ack()){IIC_Stop(); return 1;}
	}
	
	IIC_Stop();	
	return 0;
}
