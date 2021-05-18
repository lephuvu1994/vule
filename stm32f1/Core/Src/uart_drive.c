#include "uart_drive.h"
#include <stdio.h>
#include "String.h"

void UART_init(unsigned short usart, unsigned long BR)
{
 /// If using USART1 clock speed 72Mhz, else 36Mhz
	/// USART3 -> PB10 (Tx) and PB11(Rx)
	/// USART2 -> PA2 (Tx) and PA3(Rx)
	/// USART1 -> PA9 (Tx) and PA10(Rx)
	
	// Enable the Alternate Function for PINs
	unsigned long BRR_Cal;
	
	BRR_Cal = USART_BRR(usart,BR);
	
		RCC->APB2ENR |= 1;													// AFIO RCC
	
	
	if(usart == 1)
	{
		__disable_irq();														// Vô hieu hoa ngat
	RCC->APB2ENR |=0x4000;												// usart1 enable
	init_GP(PA,9,OUT50,O_AF_PP);
	init_GP(PA,10,IN,I_PP);
	// Setup the baude rate for 9600 bps
	USART1->BRR = BRR_Cal; 
	USART1->CR1 |= 8;
	USART1->CR1 |= 4;
	USART1->CR1 |= 0x2000;
	USART1->CR1 |= 0x20;
		NVIC_EnableIRQ(USART1_IRQn);
		__enable_irq();
	}
	else if (usart == 2)
	{		//-----------Init UART ------------///
	// Enable UART2
		__disable_irq();
		RCC->APB1ENR |=0x20000;
	// Enable the related PINs
	init_GP(PA,2,OUT50,O_AF_PP);
	init_GP(PA,3,IN,I_PP);
	// Setup the baude rate for 9600 bps
	USART2->BRR = BRR_Cal; 
	// Enable Uart Transmit
	USART2->CR1 |= 8;
	// Enable Uart Recive
	USART2->CR1 |= 4;
	// Enable Uart
	USART2->CR1 |= 0x2000;
		USART2->CR1 |= 0x20;
		NVIC_EnableIRQ(USART2_IRQn);
		__enable_irq();
	}
	else if (usart == 3)
	{
	__disable_irq();
	RCC->APB1ENR |=0x40000;
	init_GP(PB,10,OUT50,O_AF_PP);
	init_GP(PB,11,IN,I_PP);
	// Setup the baude rate for 9600 bps
	USART3->BRR = BRR_Cal; 
	USART3->CR1 |= 8;
	USART3->CR1 |= 4;
	USART3->CR1 |= 0x2000;
	USART3->CR1 |= 0x20;
	NVIC_EnableIRQ(USART3_IRQn);
		__enable_irq();
	}
}

unsigned long USART_BRR(unsigned short usart, unsigned long BR)
{
	unsigned long div = 36000000UL;
	unsigned long dec;
	unsigned long final;
	double frac = 36000000.00;
	double frac2 = 1.00;
	
	if(usart == 1)
	{
	div = 72000000UL;
	frac = 72000000.00;
	}
	div = div / (BR*16);
	frac = 16*((frac / (BR*16))-div);
	dec = frac;
	frac2 = 100*(frac-dec);
	if(frac2>50)
	{
		dec ++;
		if(dec == 16)
		{
			dec = 0;
			div ++;
		}
	}
	
	final = (div<<4);
	final += dec;
	
	return final;
	
}


char UART_RX(unsigned short uart)
{
	char c;
	if(uart == 1)
		{
		while((USART1->SR & 0x20) == 0x00)
		{};
		c = USART1->DR;
		}
	else if(uart == 2)
		{
		while((USART2->SR & 0x20) == 0x00)
		{};
		c = USART2->DR;
		}
		if(uart == 3)
		{
		while((USART3->SR & 0x20) == 0x00)
		{};
		c = USART3->DR;
		}
		return c;
}

void UART_TX(unsigned short uart,char c)
{
	if(uart == 1)
		{
			while((USART1->SR & (1<<6)) == 0x00)
			{};
			USART1->DR = c;
		}
	else if(uart == 2)
		{
			while((USART2->SR & (1<<6)) == 0x00)
			{};
			USART2->DR = c;
		}
		if(uart == 3)
		{
			while((USART3->SR & (1<<6)) == 0x00)
			{};
			USART3->DR = c;
		}
}

/*
1- Define my uart
2- is it bridge or process or both 
if process : Sting to fullfill, cnt , signal 
*/

void UART_ISR(unsigned short uart,unsigned short uart_mgr[], char str[])
{
	if(uart_mgr[2] == 0)
		{
			str[uart_mgr[0]] = UART_RX(uart);
			if(uart_mgr[3])
			{
						if(str[uart_mgr[0]] == uart_mgr[4])
						{
							uart_mgr[0]= 0;
							uart_mgr[1] = 1;
						}
						else
						{
							uart_mgr[0]++;
						}
			}else
					{
						// Timer strategy
						uart_mgr[0]++;
						uart_mgr[6] = uart_mgr[5];
//						systick_int_start();
					}
		}
	else
	{
		UART_TX(uart_mgr[2],UART_RX(uart));
	}
}

void UART_SEND(unsigned short uart, char str[])
{
	int i = 0;
	while(str[i] != '\0')
	{
		UART_TX(uart,str[i]);
		i++;
	}
}

void UART_msg(unsigned short uart, char str[], unsigned short str_mgr[])
{
	unsigned long timeOut = 720000000;
	UART_SEND(uart,str);
	while(str_mgr[1] == 0 & timeOut != 0)
	{
		timeOut --;
	}
	str_mgr[1] = 0;
}
void uart_send_reply(unsigned short uart, char str[],float nhietdo, int doam , int adc1,int adc2, int id)
{
	  char str_reply[100]; 
	  if(id == 1)  // neu id  ==1 thi lam cai o duoi
		{
					if(strstr(str,"tram1") != NULL) // trong chuoi tin nhan nhan duoc co chu tram1
					{

						 sprintf(str_reply,"%.2f,%d,%d,%d tram1\n",nhietdo,doam,adc1,adc2); // gui lai tin nhan cho bên kia
			    }
					else
					{
							sprintf(str_reply,"");	
					}
		}
						
		else if(id == 2)
		{
					if(strstr(str,"tram2") != NULL)
					{
						 sprintf(str_reply,"%.2f,%d,%d,%d tram2\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else if(id == 3)
		{
					if(strstr(str,"tram3") != NULL)
					{
						 sprintf(str_reply,"%.2f,%d,%d,%d tram3\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else if(id == 4)
		{
					if(strstr(str,"tram4") != NULL)
					{
						  sprintf(str_reply,"%.2f,%d,%d,%d tram4\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else if(id == 5)
		{
					if(strstr(str,"tram5") != NULL)
					{
						  sprintf(str_reply,"%.2f,%d,%d,%d tram5\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else if(id == 6)
		{
					if(strstr(str,"tram6") != NULL)
					{
						  sprintf(str_reply,"%.2f,%d,%d,%d tram6\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else if(id == 7)
		{
					if(strstr(str,"tram7") != NULL)
					{
						  sprintf(str_reply,"%.2f,%d,%d,%d tram7\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else if(id == 8)
		{
					if(strstr(str,"tram8") != NULL)
					{
						 sprintf(str_reply,"%.2f,%d,%d,%d tram8\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else if(id == 9)
		{
					if(strstr(str,"tram9") != NULL)
					{
						 sprintf(str_reply,"%.2f,%d,%d,%d tram9\n",nhietdo,doam,adc1,adc2);
					}
		}
		else if(id == 10)
		{
					if(strstr(str,"tram10") != NULL)
					{
						 sprintf(str_reply,"%.2f,%d,%d,%d tram10\n",nhietdo,doam,adc1,adc2);
					}
					else
					{
							sprintf(str_reply,"");	
					}
		}
		else 
		{
			   sprintf(str_reply,"");				
		}
		 UART_SEND(uart,str_reply); // gui
		
}
