#include "main.h"
#include "I2C_CCS811.h"
#include "gpio.h"
#include "math.h"
#include "uart_drive.h"
#include "sht30.h"
#include "Lora.h"
#include "help_func.h"
#include "gp_drive.h"
#include "i2c_drive.h"
#include "adc_drive.h"
#include "filter.h"
#include "i2c_mem.h"
#include "delay.h"
#include "BH1750.h"

/* Private includes ----------------------------------------------------------*/
/*
UART Manager
0- count
1- signal
2- Bridge
3- Terminator should 1: Terminator / 0: Interrupt
4- terminator char 
5- time cst
6- time counter 

*/
/*
char USART_1_msg[250];
unsigned short USART_1_cnt = 0;
unsigned short USART_1_sig = 0;
unsigned short USART_1_bdg = 0;
*/
//--------------------------------------------------------------------------------
#define PI 3.1415926553589793f
volatile float _sin,_cos,_random,random_LPF_HPF, random_kalman, signal_LPF, signal_HPL, signal_kalman,signal_kalman1,signal_kalman2,signal_kalman3,signal_kalman4;

#define ADC1_DR_ADDRESS 			((uint32_t)0x4001244C)

//--------------------------------------------------------------------------------
/* Cac goi tin cua uart1*/
char USART_1_msg[1000];																		// goi tin cua uart1
unsigned short USART_1_cnt = 0;
unsigned short USART_1_sig = 0;
unsigned short USART_1_bdg = 0;
unsigned short uart_1_mgr[7]={0,0,0,1,'\n',0,0};
//---------------------------------------------------------------------------------
/* Cac goi tin cua uart2*/
char USART_2_msg[1000];																		// goi tin cua uart2
unsigned short USART_2_cnt = 0;
unsigned short USART_2_sig = 0;
unsigned short USART_2_bdg = 0;
unsigned short uart_2_mgr[7]={0,0,0,1,'\n',0,0};
//-----------------------------------------------------------------------------------
/* Cac goi tin cua uart2*/
char USART_3_msg[1000];																			// goi tin cua uart2
unsigned short USART_3_cnt = 0;
unsigned short USART_3_sig = 0;
unsigned short USART_3_bdg = 0;
unsigned short uart_3_mgr[7]={0,0,0,1,'\n',0,0};
//------------------------------------------------------------------------------------

int process = 0;
char ADC_channels[4] = {PB1,PA4,PA5,PA6};  								// PB1, PA4, PA5, PA6 doc tin hieu analog ung voi cac chan
volatile uint16_t ADC_value[4]; 														// mang luu gia tri analog tra ve o moi channel
char channels = 4;  																				// so channel can doc analog lua chon 4 kenh analog
volatile int diachi;  																			// dia chi cua slave 
uint8_t recv_dat[6] = {0}; 																	// mang luu gia tri temperature, hummidity va 2 bit checksume
float temperature = 0.0;  																	// bien luu gia tri nhiet do cua cam bien doc ve
float humidity = 0.0;  																			// bien luu gia tri do am cua cam bien doc ve
char str[50];																								// mang luu string de gui uart

uint16_t CO2;
uint16_t tVOC;
	uint8_t recv_lux;
	uint16_t lux;

//------------------------------------------------------------------------------------------
//Subrotine set dia chi = 2^3 PB4+ 2^1 PB3 + 2^1 PA15 +2^0 PA12
void set_dia_chi() /// gan dia chi cho thiet bi
{
		diachi = ((1 * R_GP(PB,4) )+( 2 * R_GP(PB,3) )+ (4 * R_GP(PA,15) )+ (8 * R_GP(PA,12))); // 
}//
//
//------------------------------------------------------------------------------------------
void SystemClock_Config(void);
void Filter(void);
void DMA_configChannel_11(uint32_t *pStartAddress, uint32_t *pDestination, uint32_t u32NumberDataTransfer);
//------------------------------------------------------------------------------------------
//

int main(void)
{
	int counttime =0;
  HAL_Init();																				// Hal init
  SystemClock_Config();
  MX_GPIO_Init();
//  MX_I2C2_Init();
	
	//Khoi tao A0, A1 muc thap chon che do LoRa
	init_GP(PA,0,OUT50,O_GP_PP);														// khoi tao PA0, output 50Mhz, che do Push pull de dieu khien MD0 Lora
	W_GP(PA,0,0);																						// set PA0(MD0 LoRa) muc thap
	init_GP(PA,1,OUT50,O_GP_PP);														// khoi tao PA1, output 50Mhz, che do Push pull de dieu khien MD0 Lora
	W_GP(PA,1,0);																						// set PA1 (MD0 LoRa) muc thap
	
	
	//Khoi tao systick
	//systick_init(); 																				// khoi tao systick cho ham delay				
	
	//Khoi tao GPIO B4, B3, A12, A15 chon dia chi slave
	init_GP(PB,4,IN ,I_PP);  																// khoi tao gpio in put che do co tro keo len 3.3v
	init_GP(PB,3,IN ,I_PP);  																// khoi tao gpio in put che do co tro keo len 3.3v// doan nay can xem lai
	init_GP(PA,15,IN ,I_PP);																// khoi tao gpio in put che do co tro keo len 3.3v
	init_GP(PA,12,IN,I_PP);  																// khoi tao gpio in put che do co tro keo len 3.3v
	
	
	//Khoi tao UART
	UART_init(1,115200); 																		// khoi tao uart 1 baudrate 115200
	UART_init(2,115200); 																		// khoi tao uart 2 baudrate 115200
//	UART_init(3,115200); 																		// khoi tao uart 3 baudrate 115200
	delay_init();

	//Khoi tao I2C1
	i2c_init(1); 																						// khoi tao i2c
	
	
	//Reset SHT30
	SHT30_reset();  																				// reseet SHT30
	
	
	//Khoi tao ADC1 multi channels
	adc_multi_ch_init(adc1, channels, ADC_channels); 				//khoi tao che do nhieu kenh adc1 (so channel)
	DMA_configChannel_11( (uint32_t*)ADC1_DR_ADDRESS, (uint32_t*)ADC_value, 4);
	
	
	//Khoi tao systick cho uart
	//systick_int_start(); 																		// khoi tao systick cho uart
  delay_ms(100); 																					// doi khoi tao 
	
	
	//Set dia chi cho slave
	set_dia_chi(); 																					// doc dia chi tu switch lua cho dia chi cho slave
	
	//Gui uart1 kiem tra slave
	UART_SEND(1,"Slave\n");																	// gui string ra uart1(PA9, PA10) 
	
	
	//Khoi tao SHT30 va gui string khoi tao neu thanh cong
	if(SHT30_Init() == 1) {UART_SEND(1,"Khoi tao ok!!!");}  // khoi tao SHT30 neu thanh cong thi gui ra uart1(PA9, PA10) 
  else {UART_SEND(1,"Chua khoi tao duoc!!!");} 						// sht30 khoi tao that bai gui string	

  delay_ms(10);
	//Config CCS811
//	softRest();																							// reset ccs811
//  configureCCS811();
	delay_ms(500);
// Vong lap while	
	delay_init();
	BH1750_init();
  while (1)
  {
			Filter();
			counttime++;									// dinh thoi gian de doc cam bien
			if(counttime >5000)
				{
					set_dia_chi();													// cap nhat dia chi SLAVE
					BH1750_Read_Data(&recv_lux);
					BH1750_Data_to_Int(&recv_lux, &lux);
					delay_ms(1000);
//				if(dataAvailable() ==1){
//						readAlgorithmResults(&CO2, &tVOC);
//				}
//				adc_multi_ch_rx(adc1, channels, ADC_channels, (uint16_t*)ADC_value);	  // doc multi adc1, voi nhieu channel

				
					SHT30_Read_Dat(0x88,recv_dat,6); 														// doc gia tri nhiet do, do am tu SHT30 dia chi 0x44<<1, ghi vao mang recv_dat 6 bit
					if( SHT30_Dat_To_Float(recv_dat, &temperature, &humidity) == 0)			// doi temperature, humidity data sang dang float
					{					
					}	//END doi data sht30->float 
/*				sprintf(str,"CO2 = %d", CO2);
				UART_SEND(1,str);
				UART_SEND(1,",");
				sprintf(str,"tVOC = %d", tVOC);
				UART_SEND(1,str);
				UART_SEND(1,",");
				sprintf(str,"temperature = %.2f",temperature); 
				UART_SEND(1,str);	                 									 // gui gia ra thoi PA9 Pa10
				UART_SEND(1,",");
				sprintf(str,"humidity = %.2f\n", humidity);
				UART_SEND(1,str);*/	
				counttime =0;
			}
//			UART_SEND(1,",");
//			sprintf(str,"adc1 = %d\n", CO2);
//			UART_SEND(1,str);	
//			sprintf(str,"adc2 = %d\n", analog_rx[1]);
//			UART_SEND(1,str);	
//			sprintf(str,"adc3 = %d\n", analog_rx[2]);
//			UART_SEND(1,str);	
//			sprintf(str,"adc4 = %d\n", analog_rx[3]);
//			UART_SEND(1,str);
	}
	
}//

//Subrotine UART//

void USART1_IRQHandler()
{
	UART_ISR(1,uart_1_mgr, USART_1_msg);
	
}
//--------------------------------------------------------------------------------

void USART2_IRQHandler()
{
	UART_ISR(2,uart_2_mgr, USART_2_msg);
	if(uart_2_mgr[1] == 1) 																									// kiem tra neu nhan duoc tin nhan o RX uart2 co ky tu 1
			{
				 UART_SEND(1,"Noi dung tin nhan la :\t");													// thuc hien gui "noi dung tin nhan" vao uart1
				 UART_SEND(1,USART_2_msg);																				// gui noi dung vua nhan duoc o uart2 vao uart1
				 UART_SEND(1,"\n");																								// gui ky tu xuong dong vao uart1
				 uart_send_reply(2,USART_2_msg,(float)temperature, humidity, CO2, tVOC, diachi); // thu hien tra loi tin nhan neu duoc goi
				 uart_2_mgr[1] = 0;																								// xoa du lieu RX o uart2 ve =0
				 str_empty(USART_2_msg); 																					//cainayhoibinh
			}
}
//---------------------------------------------------------------------------------

void USART3_IRQHandler()
{
	UART_ISR(3,uart_3_mgr, USART_3_msg);
}
//----------------------------------------------------------------------------------
//Bo loc tin hieu kalman filter

void Filter()
{
//	static float x =0;
//	_sin = sin(x)*1000;
//	x+=2*PI/10000;
//	_random = (float)rand()/1000000;
//	
//	random_LPF_HPF = _random + _sin;
//	signal_LPF = LPF(random_LPF_HPF,1,1000);
//	signal_HPL = HPF(random_LPF_HPF,1,1000);
	
//	random_kalman = (float)rand()/1000000;
	//signal_kalman = kalman_single(temperature, 10,1);		// loc vui cai nhiet do cai duoc khong
	signal_kalman1 = kalman_single(ADC_value[0], 500,10);	// loc analog 1
//	signal_kalman2 = kalman_single(ADC_value[1], 500,10);	// loc analog 2
//	signal_kalman3 = kalman_single(ADC_value[2], 500,10);	// loc analog 3
//	signal_kalman4 = kalman_single(ADC_value[3], 500,10);	// loc analog 4
}
//
//------------------------------------------------------------------------------------------------------
// Ham khoi tao config DMA cho ADC
void DMA_configChannel_11(uint32_t *pStartAddress, uint32_t * pDestination, uint32_t u32NumberDataTransfer)
{
	//Khoi tao clock cho DMA1
	RCC->AHBENR |=1;
	// Tro den dia chi Periphernal
	DMA1_Channel1->CPAR = ((uint32_t)0x4001244C);
	//Tro den dia chi memmory
	DMA1_Channel1->CMAR = ((uint32_t)0x20000022);
	// do dai thanh ghi data
	DMA1_Channel1->CNDTR = u32NumberDataTransfer;
	//lua chon cac che do bo nho, muc uu tien
	DMA1_Channel1->CCR = 0x25A0; // che do circular, memory tang bo nho, memory va peripheral 01 tuc 16bit, va muc do uu tien kenh(10) high
	//DMA enable
	DMA1_Channel1->CCR |=0x01;
}
//



//-----------------------------------------------------------------------------------

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
