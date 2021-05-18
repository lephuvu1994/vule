#include "sht30.h"
#include "i2c_drive.h"
#include "stm32f1xx.h"

uint8_t rx_data[6];																		//Khoi tao chuoi rx_data[6] de luu byte tra ve gia tri nhietdo,doam,checksum

//----------------------------------------------------------------------------------------
// chuong trinh sht30 gui lenh
static uint8_t	SHT30_Send_Cmd(SHT30_CMD cmd)
{
    uint8_t cmd_buffer[2];														// khoi tao mang 8bit 2chieu de luu lenh 16bit
    cmd_buffer[0] = cmd >> 8;													// lay 8bit cao cua lenh
    cmd_buffer[1] = cmd;															//lay 8bit thap cua lenh
//		i2c_start();
//    i2c_address(address);
		i2c_writemulti(1,cmd_buffer,2);											//tien hanh gui lenh bang cach ghi 2byte i2c
		return 1;																					//return 1 neu gui thanh cong
//		i2c_stop();
}//END guilenhsht30
//----------------------------------------------------------------------------------------

//CT reset sht30
void SHT30_reset(void)
{
		i2c_start(1);																		// gui bit bat dau giao tiep i2c
		i2c_address(1, SHT30_ADDR_WRITE);									//gui dia chi bat dau ghi du lieu xuong sht30
    SHT30_Send_Cmd(SOFT_RESET_CMD);									//gui lenh cho phep reset mem sht30
    HAL_Delay(20);																		// xem lai stop
		i2c_stop(1);																			//stop i2c
}//END sht30reset
//-----------------------------------------------------------------------------------------

//CT khoi tao sht30 co tra ve
uint8_t SHT30_Init(void)
{
		i2c_start(1);																		//bat dau giao tiep i2c
		i2c_address(1,SHT30_ADDR_WRITE);									//gui lenh ghi du lieu xuong sht30
    return SHT30_Send_Cmd(MEDIUM_2_CMD);						//tra ve khi gui lenh chon che do medium du lieu / second
	
		i2c_stop(1);																			//stop i2c
	//
}//END SHT30init
//--------------------------------------------------------------------------------------

//CT doc du lieu sht30 gui len
void SHT30_Read_Dat(uint8_t address, uint8_t* dat, uint8_t size)
{	
		i2c_start(1);																		// bat dau giao tiep i2c
		i2c_address(1,SHT30_ADDR_WRITE);									//gui lenh ghi du lieu xuong sht30
		SHT30_Send_Cmd(READOUT_FOR_PERIODIC_MODE);			//gui lenh ghi du lieu chon che do doc ket qua dinh ky 
		i2c_start(1);																		// lai khoi tao giao tiep i2c
		i2c_read(1, address+0x01,dat, size);								// doc gia tri tra ve khi gui bit dia chi 0x44<<1 +1, ghi buffer vao data voi size
		i2c_stop(1);																			//stop i2c
//		while(!(I2C1->CR1 &(1<<10)));																	// doi bit ack duoc set
	//return HAL_I2C_Master_Receive(&hi2c1, SHT30_ADDR_READ, dat, 6, 0xFFFF);
}//END Read_data
//---------------------------------------------------------------------------

//CT con kiem tra checksum
#define CRC8_POLYNOMIAL 0x31		

uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value)
{
    uint8_t  remainder;	    //remainder
    uint8_t  i = 0, j = 0;  //Loop variable

    /* Initialization */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* Calculate sequentially from the highest bit */
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1)^CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /* Return the calculated CRC code */
    return remainder;
}// END kiem tra checksum
//----------------------------------------------------------------------------------

//CT chuyen doi data sang float
uint8_t SHT30_Dat_To_Float(uint8_t* const dat, float* temperature, float* humidity)
{
	uint16_t recv_temperature = 0;
	uint16_t recv_humidity = 0;
	
	/* Verify that the temperature data and humidity data are received correctly */
	if(CheckCrc8(dat, 0xFF) != dat[2] || CheckCrc8(&dat[3], 0xFF) != dat[5])
		return 1;
	
	/* Convert temperature data */
	recv_temperature = ((uint16_t)dat[0]<<8)|dat[1];
	*temperature = -45 + 175*((float)recv_temperature/65535);
	
	/* Convert humidity data */
	recv_humidity = ((uint16_t)dat[3]<<8)|dat[4];
	*humidity = 100 * ((float)recv_humidity / 65535);
	
	return 0;
}//END chuyen doi
//
