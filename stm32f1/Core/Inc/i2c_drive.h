#ifndef __i2c_drive_H
#define __i2c_drive_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "stdint.h"

void i2c_init(uint8_t i2c);																			//function khoi tao i2c
void i2c_writemulti( uint8_t i2c2, uint8_t *data, uint8_t size);							//fuction ghi nhieu lan 8bit 
void i2c_stop(uint8_t i2c2);																			//fuction stop i2c
uint8_t i2c_address(uint8_t i2c, uint8_t address);															//fuction ghi dia chi i2c de gui
uint8_t i2c_write(uint8_t i2c, uint8_t data);																//fuction ghi 8bit
void i2c_start(uint8_t i2c);																			//fuction start i2c 
void i2c_read(uint8_t i2c, uint8_t Address, uint8_t *buffer, uint8_t size);				//fuctin doc i2c ghi vao *buffer
void i2c_send_cmd(uint8_t i2c, uint8_t addr, uint8_t cmd);


#ifdef __cplusplus
}
#endif
#endif /*__ i2c_drive_config_H */
//
