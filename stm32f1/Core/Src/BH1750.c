#include "BH1750.h"
#include "delay.h"


void bh1570_send_cmd(uint8_t cmd)
{
	i2c_send_cmd(1,BH1750_WRITE_ADDRESS,  cmd);
}
//


uint16_t BH1750_Read_Data( uint8_t *buffer)
{
		i2c_start(1);
	_bh1750_stable_delay();
	i2c_read(1,BH1750_READ_ADDRESS, buffer, 2);
	i2c_stop(1);
	return 0;
}
//
uint8_t BH1750_Data_to_Int(uint8_t* const data, uint16_t* lux)
{
		uint16_t recv_lux = 0; 
		recv_lux = ((data[0]<<8 )| data[1]);
		*lux = (int)recv_lux/ 1.2;
		return 0;
}
	//
void BH1750_init(void)
{
	//power sensor
	bh1570_send_cmd(BH1750_CMD_POWER_ON);
	_bh1750_stable_delay();
	// config continous measure mode
	bh1570_send_cmd(BH1750_CMD_MODE_RESOLUTION);
	_bh1750_stable_delay();
	//little delay for stable
}
//


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

