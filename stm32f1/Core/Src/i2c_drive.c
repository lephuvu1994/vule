#include "i2c_drive.h"
#include "stdio.h"
#include "stm32f1xx.h"
#include "gp_drive.h"

void i2c_init(uint8_t i2c)
{
	//GPIO config
	if(i2c==1){
		RCC->APB1ENR |= (1<<21);																		// enable clock i2c
		init_GP(PB, 6 ,OUT50, O_AF_OD);															// init GPIOB, 50Mhz, alternate fuction, open drain
		init_GP(PB, 7 ,OUT50, O_AF_OD);
		AFIO->MAPR |= (0<<0);
		//I2C config
		I2C1->CR1 |= I2C_CR1_PE;								// enable i2c
		
		//reset i2c	
		I2C1->CR1 |=(1<<15);											//reset i2c
		I2C1->CR1 &=~(1<<15);
		
		// config frequency
		I2C1->CR2 |=(36<<0);											// 36Mhz
		
		//config the clock control register
		I2C1->CCR = (180<<0);											// tra bang i2c character = (1000+4000)/222.22(1/45)
		I2C1->OAR1 |=(0x00004000);
		
		//Config the rise time register
		I2C1->TRISE = (37<<0);
		
		// enable i2c PE=1
		I2C1->CR1 |= (1<<0);											//PE=1 enabale i2c
	}
	if(i2c==2){
		RCC->APB1ENR |= (1<<22);
		init_GP(PB, 10 ,OUT50, O_AF_OD);															// init GPIOB, 50Mhz, alternate fuction, open drain
		init_GP(PB, 11 ,OUT50, O_AF_OD);
		AFIO->MAPR |= (0<<0);
		//I2C config
		I2C2->CR1 |= I2C_CR1_PE;								// enable i2c
		
		//reset i2c	
		I2C2->CR1 |=(1<<15);											//reset i2c
		I2C2->CR1 &=~(1<<15);
		
		// config frequency
		I2C2->CR2 |=(36<<0);											// 36Mhz
		
		//config the clock control register
		I2C2->CCR = (180<<0);											// tra bang i2c character = (1000+4000)/222.22(1/45)
		I2C2->OAR1 |=1<<14;
		
		//Config the rise time register
		I2C2->TRISE = (37<<0);
	
		// enable i2c PE=1
		I2C2->CR1 |= (1<<0);											//PE=1 enabale i2c
	}
}
//
void i2c_start(uint8_t i2c)
{
	if(i2c ==1){
		I2C1->CR1 |= (1<<10);  										// Enable the ACK
		I2C1->CR1 |= (1<<8);  										// Generate START
		while(!(I2C1->SR1 &(1<<0)));							// doi bit SB dc set len
	}
	if(i2c ==2){
		I2C2->CR1 |= (1<<10);  										// Enable the ACK
		I2C2->CR1 |= (0<<8);  										// Generate START
		I2C2->CR1 |= (1<<8);
		while(!(I2C2->SR1 &(1<<0)));							// doi bit SB dc set len
	}
	
}
uint8_t i2c_write(uint8_t i2c, uint8_t data)
{
	if(i2c==1){
		while (!(I2C1->SR1 & (1<<7)));  					// wait for TXE bit to set
		I2C1->DR = data;
		while (!(I2C1->SR1 & (1<<2)));  					// wait for BTF bit to set
	//	while (!(I2C1->CR1 & (1<<10)));					// doi co bit xac nhan ack tra ve
	//	I2C1->CR1 &= ~(1<<10);;
	}
	if(i2c==2){
		while (!(I2C2->SR1 & (1<<7)));  					// wait for TXE bit to set
		I2C2->DR = data;
		while (!(I2C2->SR1 & (1<<2)));  					// wait for BTF bit to set
	//	while (!(I2C1->CR1 & (1<<10)));					// doi co bit xac nhan ack tra ve
	//	I2C1->CR1 &= ~(1<<10);;
		
	}
	return 1;
}//
//



// Sending the address + R or Write	
uint8_t i2c_address(uint8_t i2c, uint8_t address)												// i2c send address
{
	volatile uint8_t tmp;
		if(i2c==1){
		I2C1->DR = address;
			while (!(I2C1->SR1 & (1<<1))){};												// doi cho bit address bit duoc set
				tmp = I2C1->SR1 | I2C1->SR2;													// doc thanh ghi SR1 va SR2 de clear bit ADD
			while( !(I2C1->CR1 &(1<<10))){};												// doi bit ack tra ve
			//I2C1->CR1 &= ~(1<<10);
				return 1;
		}
		if(i2c==2){
		I2C2->DR = address;
			while (!(I2C2->SR1 & (1<<1))){};												// doi cho bit address bit duoc set
				tmp = I2C2->SR1 | I2C2->SR2;													// doc thanh ghi SR1 va SR2 de clear bit ADD
			while( !(I2C2->CR1 &(1<<10))){};												// doi bit ack tra ve
			//I2C1->CR1 &= ~(1<<10);
				return 1;
		}
	return 1;
}
//
void i2c_send_cmd(uint8_t i2c, uint8_t addr, uint8_t cmd)
	{
			i2c_start(i2c);
			i2c_address(i2c, addr);
			i2c_write(i2c, cmd);
	}

void i2c_stop(uint8_t i2c)
{
	if(i2c==1){
		I2C1->CR1 |= (1<<9);																				// stop bit set
	}
	if(i2c==2){
		I2C2->CR1 |= (1<<9);																				// stop bit set
	}
}
//
void i2c_writemulti(uint8_t i2c, uint8_t *data, uint8_t size)
{
	if(i2c==1){
			while(!(I2C1->SR1 & (1<<7)));				// wait for TXE bit to set
			while(size)
			{	
				while(!(I2C1->SR1 & (1<<7)));				// wait for TXE bit to set
				I2C1->DR = (volatile uint32_t) *data++;
				size--;
				while (!(I2C1->CR1 & (1<<10)));		// co bit xac nhan ack tra ve
			}
			while ((I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
	}
	if(i2c==2){
			while(!(I2C2->SR1 & (1<<7)));				// wait for TXE bit to set
			while(size)
			{	
				while(!(I2C2->SR1 & (1<<7)));				// wait for TXE bit to set
				I2C2->DR = (volatile uint32_t) *data++;
				size--;
				while (!(I2C2->CR1 & (1<<10)));		// co bit xac nhan ack tra ve
			}
			while ((I2C2->SR1 & (1<<2)));  // wait for BTF bit to set
	}
}
//
void i2c_read(uint8_t i2c, uint8_t Address, uint8_t *buffer, uint8_t size)					// doc du lieu ghi vao buffer
{
	int remaining = size;
	if(i2c==1){
					/**** STEP 1 ****/	
			if (size == 1)
			{
				/**** STEP 1-a ****/	
				I2C1->DR = Address;  //  send the address
				while (!(I2C1->SR1 & (1<<1)));  											// wait for ADDR bit to set
				
				/**** STEP 1-b ****/	
				I2C1->CR1 &= ~(1<<10);  															// clear the ACK bit 
				uint8_t temp = I2C1->SR1 | I2C1->SR2;  								// read SR1 and SR2 to clear the ADDR bit.... EV6 condition
				I2C1->CR1 |= (1<<9);  																// Stop I2C

				/**** STEP 1-c ****/	
				while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
				
				/**** STEP 1-d ****/	
				buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER
				
			}

		/**** STEP 2 ****/		
			else 
			{
				/**** STEP 2-a ****/
				I2C1->DR = Address;  //  send the address
				while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
				
				/**** STEP 2-b ****/
				uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
				
				while (remaining>2)
				{
					/**** STEP 2-c ****/
					while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
					
					/**** STEP 2-d ****/
					buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer			
					
					/**** STEP 2-e ****/
					I2C1->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received
					
					remaining--;
				}
				
				// Read the SECOND LAST BYTE
				while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
				buffer[size-remaining] = I2C1->DR;
				
				/**** STEP 2-f ****/
				I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
				
				/**** STEP 2-g ****/
				I2C1->CR1 |= (1<<9);  // Stop I2C
				
				remaining--;
				
				// Read the Last BYTE
				while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
				buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer
			}
	}
	if(i2c==2){
					/**** STEP 1 ****/	
			if (size == 1)
			{
				/**** STEP 1-a ****/	
				I2C2->DR = Address;  //  send the address
				while (!(I2C2->SR1 & (1<<1)));  											// wait for ADDR bit to set
				
				/**** STEP 1-b ****/	
				I2C2->CR1 &= ~(1<<10);  															// clear the ACK bit 
				uint8_t temp = I2C2->SR1 | I2C2->SR2;  								// read SR1 and SR2 to clear the ADDR bit.... EV6 condition
				I2C2->CR1 |= (1<<9);  																// Stop I2C

				/**** STEP 1-c ****/	
				while (!(I2C2->SR1 & (1<<6)));  // wait for RxNE to set
				
				/**** STEP 1-d ****/	
				buffer[size-remaining] = I2C2->DR;  // Read the data from the DATA REGISTER
				
			}

		/**** STEP 2 ****/		
			else 
			{
				/**** STEP 2-a ****/
				I2C2->DR = Address;  //  send the address
				while (!(I2C2->SR1 & (1<<1)));  // wait for ADDR bit to set
				
				/**** STEP 2-b ****/
				uint8_t temp = I2C2->SR1 | I2C2->SR2;  // read SR1 and SR2 to clear the ADDR bit
				
				while (remaining>2)
				{
					/**** STEP 2-c ****/
					while (!(I2C2->SR1 & (1<<6)));  // wait for RxNE to set
					
					/**** STEP 2-d ****/
					buffer[size-remaining] = I2C2->DR;  // copy the data into the buffer			
					
					/**** STEP 2-e ****/
					I2C2->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received
					
					remaining--;
				}
				
				// Read the SECOND LAST BYTE
				while (!(I2C2->SR1 & (1<<6)));  // wait for RxNE to set
				buffer[size-remaining] = I2C2->DR;
				
				/**** STEP 2-f ****/
				I2C2->CR1 &= ~(1<<10);  // clear the ACK bit 
				
				/**** STEP 2-g ****/
				I2C2->CR1 |= (1<<9);  // Stop I2C
				
				remaining--;
				
				// Read the Last BYTE
				while (!(I2C2->SR1 & (1<<6)));  // wait for RxNE to set
				buffer[size-remaining] = I2C2->DR;  // copy the data into the buffer
			}		
	}
}
