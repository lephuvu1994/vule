#include "stm32f1xx.h"  
#include "gp_drive.h" 


unsigned long USART_BRR(unsigned short usart, unsigned long BR);
void UART_init(unsigned short usart, unsigned long BR);
char UART_RX(unsigned short uart);
void UART_TX(unsigned short uart, char c);
void UART_ISR(unsigned short uart,unsigned short uart_mgr[], char str[]);
void UART_SEND(unsigned short uart, char str[]);
void uart_send_reply(unsigned short uart, char str[], float nhietdo, int doam , int adc1,int adc2, int id);

