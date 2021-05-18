/**
  ******************************************************************************
  * File Name          : I2C.c
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "I2C_CCS811.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c2;

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* USER CODE BEGIN 1 */
uint8_t dtvalue =0;
uint8_t mosetting = 0;
uint8_t appvalue=0;
uint8_t  Mode_CCS811 = 1;


/*
unsigned int getBaseline()
{
     
    uint8_t ada[2];
    HAL_StatusTypeDef status = HAL_OK;
   
    status = HAL_I2C_Mem_Read( &hi2cxc, CCS811_ADDRD, ( uint8_t )CSS811_BASELINE,1,  ada, 2,100 );
    while (HAL_I2C_GetState(&hi2cxc) != HAL_I2C_STATE_READY)
    {
    } 
    while (HAL_I2C_IsDeviceReady(&hi2cxc, CCS811_ADDRD, 10, 300) == HAL_TIMEOUT);
    while(HAL_I2C_GetState(&hi2cxc) != HAL_I2C_STATE_READY)
    {
    }

    uint8_t baselineMSB = ada[0];
    uint8_t baselineLSB = ada[1];

    unsigned int baseline = ((unsigned int)baselineMSB << 8) | baselineLSB;
    HAL_FLASHEx_DATAEEPROM_Unlock();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, DATA_EEPROM_BASE, baseline);
      
    return (baseline);
}
*/
/*
  * @brief  configureCCS811.
  * @param  NONE.
  * @retval None.
 */
void configureCCS811()
{
    
  HAL_Delay(69);
  //Verify the hardware ID is what we expect
  uint8_t ret;
  uint8_t hwID = readRegister(CCS811_REGISTER_HW_ID); // doc id ve tu thanh ghi dia chi 0x20
  // kiem tra gia tri cua hwid
  if (hwID != 0x81)
  {
//    printf("CCS811 not found. Please check wiring.");
    while (1); //Freeze!
  }
   
  ret = readRegister(CCS811_REGISTER_STATUS);						// doc thanh ghi trang thai status
  while((ret & (1<<4)) == 0 )
  {
    ret = readRegister(CCS811_REGISTER_STATUS);					//doi den khi doc thanh ghi status khac gia tri 1 o thanh ghi app_valid
  }																											// o day la co chuong trinh ung dung

    
  uint8_t    lodata[1];
  lodata[0] = CCS811_REGISTER_APP_START;							//send thanh ghi 0xF4 app_start. cho phep ung dung chay			
    
  HAL_I2C_Master_Transmit(&hi2c2, CCS811_ADDR_GND_WRITE , lodata, 1, 100);		// gui thanh ghi app_start bang i2c
   
  HAL_Delay(20);
  setDriveMode(Mode_CCS811); 															//lua chon che do doc moi 1s (send)
  HAL_Delay(10);  
  //Check for errors
  if (checkForError() == 1)															// kiem tra bit error tu thanh ghi 0xE0( ERROR_ID)
  {
    while (1); //Freeze!
  }
    //Set Drive Mode
  
    //Check for errors

    //readHumidity();
    //readTemperature();
    //setEnvironmentalData(  HDC1080_Humidity/1000,  HDC1080_Temp);
                //RunIn_Time_Complete= BURN_IN_TIME_ADDRESS;
    // BurnIn_Time_Complete = BURN_IN_TIME_ADDRESS_VAL ;
   // Baseline_Time_Complete = BASELINE_EARLYLIFE_PERIOD_ADDRESS_VAL;
    
}
//
//Ðoc data và tính toán kêt qua? do
void readAlgorithmResults(uint16_t *CO2, uint16_t* tVOC)
{
    uint8_t data_rq[4];
		// doc gia tri tu thanh ghi ALG_result_data(0x02), 4 byte 2byte CO2, 2byte tVOC. neu 5byte them 1 byte trang thai
    uint8_t nam = HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR_GND_READ, ( uint8_t )CCS811_REGISTER_ALG_RESULT_DATA, I2C_MEMADD_SIZE_8BIT, data_rq, 4,100 );
    
    uint8_t co2MSB = data_rq[0];
    uint8_t co2LSB = data_rq[1];
    uint8_t tvocMSB = data_rq[2];
    uint8_t tvocLSB = data_rq[3];
   
  /*  TVOC value, in parts per billion (ppb)
    eC02 value, in parts per million (ppm) */
    *CO2 = ((unsigned int)co2MSB << 8) | co2LSB;
    *tVOC = ((unsigned int)tvocMSB << 8) | tvocLSB;
}
 /*
  * @brief //Given a temp and humidity, write this data to the CSS811 for better compensation
   //This function expects the humidity and temp to come in as floats
  * @param  relativeHumidity HUMIDITY.
  * @param  temperature TEMPERATURE.
  * @retval None.
 */
//
// Subrotine do nhiet do, do am
// Trong ung dung nay chua can dung
/*void setEnvironmentalData(float relativeHumidity, float temperature)
{
    int rH = relativeHumidity * 1000; //42.348 becomes 42348
    int temp = temperature * 1000; //23.2 becomes 23200
    // khai bao mang 4 gia tri 
    uint8_t envData[4];

    //Split value into 7-bit integer and 9-bit fractional
    envData[0] = ((rH % 1000) / 100) > 7 ? (rH / 1000 + 1) << 1 : (rH / 1000) << 1;
    envData[1] = 0; //CCS811 only supports increments of 0.5 so bits 7-0 will always be zero
    if (((rH % 1000) / 100) > 2 && (((rH % 1000) / 100) < 8))
    {
      envData[0] |= 1; //Set 9th bit of fractional to indicate 0.5%
    }

    temp += 25000; //Add the 25C offset
    //Split value into 7-bit integer and 9-bit fractional
    envData[2] = ((temp % 1000) / 100) > 7 ? (temp / 1000 + 1) << 1 : (temp / 1000) << 1;
    envData[3] = 0;
    if (((temp % 1000) / 100) > 2 && (((temp % 1000) / 100) < 8))
    {
      envData[2] |= 1;  //Set 9th bit of fractional to indicate 0.5C
    }

    uint8_t env[6];
    env[0]=CCS811_REGISTER_ENV_DATA;
    env[1]=envData[0];
    env[2]=envData[1];
    env[3]=envData[2];
    env[5]=envData[3];
    HAL_I2C_Mem_Write( &hi2c2, CCS811_ADDR_GND_WRITE, ( uint8_t )CCS811_REGISTER_ENV_DATA, I2C_MEMADD_SIZE_8BIT, envData, 4,100 );
     while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
      {
      } 
      while (HAL_I2C_IsDeviceReady(&hi2c2, CCS811_ADDR_GND_WRITE, 10, 300) == HAL_TIMEOUT);
      while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
      {
      }
     
 }*/
//
	
//Chuong trinh set mode 
	/*
  * @brief  //Mode 0 = Idle				/ tat che do do
	    //Mode 1 = read every 1s		/ doc moi 1s
	    //Mode 2 = every 10s				/ doc moi 10s
	    //Mode 3 = every 60s				// doc moi 60s
	    //Mode 4 = RAW mode.
  * @param  MODE.
  * @retval None.
 */
void setDriveMode(uint8_t mode)
{
		if (mode > 4) mode = 4; 					//kiem tra neu la loi mode>4 ->mode 4

		mosetting = readRegister(CCS811_REGISTER_MEAS_MODE); //Doc trang thia mode tu thanh ghi 0x01

		mosetting &=~(7<<4); //Clear DRIVE_MODE bits
		mosetting |= (mode << 4); //Mask in mode (mode ao)

		writeRegister(CCS811_REGISTER_MEAS_MODE, mosetting);				// viet mode vao lai thanh ghi meas mode 0x01
		HAL_Delay(500);
		mosetting = readRegister(CCS811_REGISTER_MEAS_MODE); //doc lai gia tri mode tu thanh ghi 0x01

}

//Checks to see if DATA_READ flag is set in the status register
//Kiem tra thanh ghi data_read neu duoc set len 1 thi co data moi
uint8_t  dataAvailable()
{   // dtvalue = readRegister(CSS811_ERROR_ID);				//check loi co the khong ca dung
                 //HAL_Delay(00);
	dtvalue = readRegister(CCS811_REGISTER_STATUS);				//kiem tra thanh ghi status
	dtvalue &=(1 << 3) ;																	// kiem tra bit thu 3 thanh ghi status, bit data_readly
	return  (dtvalue >> 3) ;															//return ve 0-> chua co du lieu, 1 neu co du lieu
}
//



//Checks to see if APP_VALID flag is set in the status register
//kiem tra xem bit ung dung duoc set len chua
uint8_t  appValid()
  {
    appvalue = readRegister(CCS811_REGISTER_STATUS);
    return (appvalue & (1 << 4));
  }


 /*
  * @brief  reading_Multiple_Register
  * @param  addr ADDRESS.
  * @param  val VALUE.
  * @param  size SIZE.
  * @retval None.
 */
//
//check loi
uint8_t checkForError()
  {
    uint8_t errvalue = 0;
    errvalue = readRegister(CCS811_REGISTER_ERROR_ID);
    errvalue = readRegister(CCS811_REGISTER_STATUS);
    return (errvalue & 1 << 0);
}



// ham doc nhieu gai tri tu thanh ghi 
void read_Mul_Register(uint8_t addr, uint8_t * val,uint8_t size)
{

   HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR_GND_READ, ( uint8_t )addr, I2C_MEMADD_SIZE_8BIT, val, size,100 );
    
}
//
//Chuong trinh reset mem có thê thêm nêu xay ra lôi thì reset
uint8_t softRest() {
    uint8_t ret =0xff;
    uint8_t rstCMD[5] = {REGISTER_SW_RESET, 0x11,0xE5,0x72,0x8A};
    // g?i 5 bye gia tri ve cam bien de reset gia tri 
    ret = HAL_I2C_Mem_Write( &hi2c2, CCS811_ADDR_GND_WRITE, REGISTER_SW_RESET, I2C_MEMADD_SIZE_8BIT, rstCMD, 5,300);
    // kiem tra xem co ok khong 
    while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
    return ret;
} 
//


// ham doc gia tri ve 
uint8_t readRegister(uint8_t addr)
{
      uint8_t dt;
      //Read an amount of data in blocking mode from a specific memory address  
			//send bit 0xB5 doc gia tri tra ve 1byte ghi vao chuoi dt tu dia chi thanh ghi addr
      HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR_GND_READ, ( uint8_t )addr,I2C_MEMADD_SIZE_8BIT, &dt, 1, 300 );
      // ham tra ve gia tri  cua i2c bang gia tri 0x20u
      while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
      return dt;
 }
    
  
// ham viet gia tri xuong thanh ghi 

uint8_t writeRegister(uint8_t addr, uint8_t val)
{
			uint8_t ret = 2 ;
			//the function sent address and memory and data
			ret = HAL_I2C_Mem_Write( &hi2c2, CCS811_ADDR_GND_WRITE, ( uint8_t )addr , 1, &val, 1,timeout);
			if(ret == HAL_ERROR)
			{
//				printf("thuan haaaaa %d",ret);
			}
//			printf("gia tri cua ret ");
			// ham tra ve gia tri  cua i2c bang gia tri 0x20u
			while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
			{
			}
			//Checks if target device is ready for communication.
			while (HAL_I2C_IsDeviceReady(&hi2c2, CCS811_ADDR_GND_WRITE, 10, timeout) == HAL_TIMEOUT);
			while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
			{
			}
			return ret;
}
//

/* I am Vu */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
