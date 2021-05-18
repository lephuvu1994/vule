/**
  ******************************************************************************
  * File Name          : I2C.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_H
#define __i2c_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */
#define	CCS811_ADDR_VSS_WRITE 			0x5B<<1
#define	CCS811_ADDR_VSS_READ			(0x5B<<1) | 1

/* ADDR Pin Conect to GND */
#define	CCS811_ADDR_GND_WRITE			0x5A<<1
#define	CCS811_ADDR_GND_READ			(0x5A<<1) | 1

	//------------------------------
	/* Software Address Register byte */


#define CCS811_REGISTER_STATUS			0x00   			//Status register
#define CCS811_REGISTER_MEAS_MODE 		0x01   			//Measurement mode and conditions register
#define CCS811_REGISTER_ALG_RESULT_DATA 0x02			// data register
#define CCS811_REGISTER_RAW_DATA 		0x03			//Raw ADC data values for resistance and current source used.
#define CCS811_REGISTER_ENV_DATA 		0x05			//Temperature and Humidity data can be written to enable compensation
#define CCS811_REGISTER_NTC 			0x06 			//Provides the voltage across the reference resistor													// from which the  ambient temperature can be determined.
#define CCS811_REGISTER_THRESHOLDS 		0x10			//Thresholds for operation when interrupts are only generated when eCO2 ppm crosses a threshold
#define CCS811_REGISTER_BASELINE		0x11			//The encoded current baseline value can be read. A previously saved encoded baseline can be written.
#define CCS811_REGISTER_HW_ID 			0x20 	 		//Hardware ID. The value is 0x81
#define CCS811_REGISTER_HW_Version 		0x21			//Hardware Version. The value is 0x1X
#define CCS811_REGISTER_FW_Boot_Version 0x23			//Firmware Boot Version. The first 2 bytes contain the
	 													//firmware version number for the boot code.
#define CCS811_REGISTER_FW_App_Version	0x24			//Firmware Application Version. The first 2 bytes contain
														//the firmware version number for the application code
#define CCS811_REGISTER_ERROR_ID 		0xE0			//Error ID. When the status register reports an error its
														//source is located in this register
#define CCS811_REGISTER_APP_ERASE 		0xF1			//If the correct 4 bytes (0xE7 0xA7 0xE6 0x09) are written
														//to this register in a single sequence the device will start
														//the application erase
#define CCS811_REGISTER_APP_VERIFY 		0xF2			//Transmit flash code for the bootloader to write to the
														//application flash code space.
#define CCS811_REGISTER_APP_DATA 		0xF3			//Transmit flash code for the bootloader to write to the
														//application flash code space.
#define CCS811_REGISTER_APP_START 	    0xF4
#define	REGISTER_SW_RESET	        	0xFF			// thanh ghi reset
#define timeout							300				// timeout


#define FIRST_BASELINE_ADDRESS_VAL              * ((  uint32_t *)(DATA_EEPROM_BASE+2))
#define BURN_IN_TIME_ADDRESS_VAL                * ((  uint32_t *)(DATA_EEPROM_BASE+3))
#define BASELINE_EARLYLIFE_PERIOD_ADDRESS_VAL   * ((  uint32_t *)(DATA_EEPROM_BASE+4))
	
#define FIRST_BASELINE_ADDRESS            			 (DATA_EEPROM_BASE+2)
#define BURN_IN_TIME_ADDRESS             				 (DATA_EEPROM_BASE+3)
#define BASELINE_EARLYLIFE_PERIOD_ADDRESS  			 (DATA_EEPROM_BASE+4)
	
#define BURN_IN_TIME              ((48*60*60)/APPLICATION_RUN_CYCLE)//48 Hours
#define RUN_IN_TIME               ((20*60)/APPLICATION_RUN_CYCLE)//20 Minutes
#define NEW_MODE_RUN_IN_TIME      ((10*60)/APPLICATION_RUN_CYCLE)//10 Minutes
#define BASELINE_EARLYLIFE_PERIOD ((500*60*60)/APPLICATION_RUN_CYCLE)//500 Hours
#define BASELINE_EL_STORE_PERIOD  ((24*60*60)/APPLICATION_RUN_CYCLE)//24 Hours
#define BASELINE_AEL_STORE_PERIOD ((7*BASELINE_EL_STORE_PERIOD)/APPLICATION_RUN_CYCLE)
#define CALIB_TEMP_HUM            ((20*60)/APPLICATION_RUN_CYCLE)//20 Minutes

#define CCS811_MEAS_MODE1	0x01u
#define CCS811_MEAS_MODE2	0x02u
#define CCS811_MEAS_MODE3	0x03u
#define CCS811_MEAS_MODE4	0x04u

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void readAlgorithmResults(uint16_t *CO2, uint16_t* tVOC);
void configureCCS811(void);
void read_Mul_Register(uint8_t addr, uint8_t * val,uint8_t size);
uint8_t checkForError(void);
uint8_t softRest(void);
uint8_t readRegister(uint8_t addr);
uint8_t writeRegister(uint8_t addr, uint8_t val);
void setEnvironmentalData(float relativeHumidity, float temperature);
void setDriveMode(uint8_t mode);
uint8_t  dataAvailable(void);
uint8_t  appValid(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ i2c_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
