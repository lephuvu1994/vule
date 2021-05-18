#include "Lora.h"

#include "gp_drive.h"
#include "uart_drive.h"
#include "stdio.h"
#include "help_func.h"

void Lora_Mode(unsigned short mode)
{
	 if(mode == 0)
	 {		 
			W_GP(PA,5,LOW); //5-MD0
		  W_GP(PA,6,LOW);  // 6-MD1
	 }
	 else if(mode == 1)
	 {		 
			W_GP(PA,6,HIGH);
		  W_GP(PA,5,LOW);
	 }
	 else if(mode == 2)
	 {		 
			W_GP(PA,6,LOW);
		  W_GP(PA,5,HIGH);
	 }
	 else if(mode == 3)
	 {		 
			W_GP(PA,5,HIGH);
		  W_GP(PA,6,HIGH);
	 }
}


void Lora_ISR(unsigned short uart,unsigned short uart_mgr[], char str[])
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
void Lora_send_response(unsigned short uart,unsigned int id_send, unsigned int id_dest,unsigned int status,unsigned int cb1, unsigned int cb2)
{
	  char str[50];
	  // xu ly du leiu cb 1
	  sprintf(str,"%d%d%d,%d,%d\n%d%d%d,%d,%d\n", id_dest,id_send,status,cb1,cb2,id_dest,id_send,status,cb1,cb2);
	  UART_SEND(uart,str);
	  str_empty(str);
}
void Lora_process(unsigned short uart,unsigned char id, char msg[] ,unsigned short mgr[],unsigned short cb1, unsigned short cb2 )
{		
		if(mgr[1] == 1)
		{
			  	if( (msg[1] -48) == 0)  // neu co chuoi gui ve
						{
								if( (msg[0] -48) == id)  // neu day la id goi ban than minh thi se kiem tra la goi resquest hay response
								{
										if ( (msg[2] -48) == 1) // newu == 0 la goi response , neu == 1 la goi request
										{
												/* gui lai chuoi gia tri tra loi sau */ // form 0 id 0 data1 data2  
												  Lora_Mode(NORMAL_MODE);
													//W_GP(PC,13,LOW);
													HAL_Delay(1);
													Lora_send_response(uart,id,0,0,cb1,cb2);
													mgr[1] = 0;	
													str_empty(msg); 
													
										}
										else if( (msg[2] -48) == 0 ) 
										{
													Lora_Mode(WAKE_UP_MODE);
													//W_GP(PC,13,HIGH);
													mgr[1] = 0;	
													str_empty(msg); 
										}
								}
								else 
								{
												Lora_Mode(WAKE_UP_MODE);
												//W_GP(PC,13,HIGH);
												mgr[1] = 0;	
												str_empty(msg);
								}
						}
						else
						{
										Lora_Mode(WAKE_UP_MODE);
										//W_GP(PC,13,HIGH);
										mgr[1] = 0;	
										str_empty(msg);
						}
		}			
}
