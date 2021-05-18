#include "i2c_drive.h"


#define BH1750_BASE_ADDRESS (0x23) //address low
//#define BH1750_TIME_OUT 	180	 //address low
#define BH1750_WRITE_ADDRESS	( BH1750_BASE_ADDRESS<<1)
#define BH1750_READ_ADDRESS		(( BH1750_BASE_ADDRESS<<1)| 0x01)
#define BH1750_CMD_POWER_ON 	(0x01)	 //address POWER_ON
#define BH1750_CMD_RESET		(0x07)	 //address RESET
#define BH1750_CMD_MODE_RESOLUTION	(0x10)	 //address RESET

#define _bh1750_stable_delay(void) do{delay_ms(180);}while(0);


uint8_t BH1750_Data_to_Int(uint8_t* const data, uint16_t* lux);
uint16_t BH1750_Read_Data( uint8_t *buffer);
void BH1750_init(void);
//



