#ifndef __MYIIC_H
#define __MYIIC_H 			   
#include "stdint.h"
#include "stm32f1xx.h"
#include "gp_drive.h"
//////////////////////////////////////////////////////////////////////////////////	 


////////////////////////////////////////////////////////////////////////////////// 
//IO
#define SDA_IN()  {GPIOB->CRH&=0XFF0FFFFF;GPIOA->CRL|=8<<20;}	//PA5IN
#define SDA_OUT() {GPIOB->CRH&=0XFF0FFFFF;GPIOA->CRL|=3<<20;} //PA5OUT
 

#define IIC_SCL 	4   //SCL out 10
#define IIC_SDA   5  //PBout(11) //SDA	 
#define READ_SDA   R_GP(PA, IIC_SDA)//PBin(9)  //SDA IN

//IIC
void IIC_Init(void);                			 
void IIC_Start(void);				
void IIC_Stop(void);	  			
void IIC_Send_Byte(uint8_t txd);		
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void); 				
void IIC_Ack(void);					
void IIC_NAck(void);	
///////////////////////////////
void IIC_Cmd_Write(uint8_t add ,uint8_t reg,uint8_t data);
uint8_t IIC_Write(uint8_t addr,uint8_t reg,uint8_t data);
uint8_t Read_IIC(uint8_t addr, uint8_t reg);
uint8_t IIC_ReadMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t IIC_WriteMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
void IIC_Write_CMD(uint8_t addr,uint8_t reg);

#endif

