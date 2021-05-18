#ifndef __sht30_H
#define __sht30_H
#ifdef __cplusplus
 extern "C" {
#endif
//----------------------------------------------------------------------

#include "stdint.h"

/* ADDR Pin Conect to VSS */

#define	SHT30_ADDR_WRITE	0x44<<1         //10001000			//byte gui dia chi ghi xuong sht30
#define	SHT30_ADDR_READ		(0x44<<1)+1	    //10001011			//byte gui dia chi doc xuong sht30

typedef enum
{
    /* Software reset command */
			SOFT_RESET_CMD = 0x30A2,													//Cho phep reset mem
    /*
         Single measurement mode
         Naming format: Repeatability_CS_CMD
    CS: Clock stretching
    */
		//Lua chon slock stretching
    HIGH_ENABLED_CMD    = 0x2C06,											//cho phep clock stretching cao
    MEDIUM_ENABLED_CMD  = 0x2C0D,											//cho phep clock stretching muc trung binh
    LOW_ENABLED_CMD     = 0x2C10,											//cho phep clock stretching thap
    HIGH_DISABLED_CMD   = 0x2400,											//bo clock stretching cao
    MEDIUM_DISABLED_CMD = 0x240B,											//bo clock stretching trung binh
    LOW_DISABLED_CMD    = 0x2416,											//bo clock stretching thap

    /*
         Period measurement mode
         Naming format: Repeatability_MPS_CMD
    MPS:measurement per second
    */
		//Lua chon che do do theo chu ky(MPS:measurement per second)
    HIGH_0_5_CMD   = 0x2032,												// Lap lai cao 0.5 mps
    MEDIUM_0_5_CMD = 0x2024,												// Lap lai trung binh 0.5 mps
    LOW_0_5_CMD    = 0x202F,												// Lap lai thap 0.5 mps
    HIGH_1_CMD     = 0x2130,												// Lap lai cao 1 mps
    MEDIUM_1_CMD   = 0x2126,												// Lap lai trung binh 1 mps
    LOW_1_CMD      = 0x212D,												// Lap lai thap 1 mps
    HIGH_2_CMD     = 0x2236,												// Lap lai cao 2 mps
    MEDIUM_2_CMD   = 0x2220,												// Lap lai trung binh 2 mps
    LOW_2_CMD      = 0x222B,												// Lap lai thap 2 mps
    HIGH_4_CMD     = 0x2334,												// Lap lai cao 4 mps
    MEDIUM_4_CMD   = 0x2322,												// Lap lai trung binh 4 mps
    LOW_4_CMD      = 0x2329,												// Lap lai thap 4 mps
    HIGH_10_CMD    = 0x2737,												// Lap lai cao 10 mps
    MEDIUM_10_CMD  = 0x2721,												// Lap lai trung binh 10 mps
    LOW_10_CMD     = 0x272A,												// Lap lai thap 10 mps
		
	/* Period measurement mode read data command */
	READOUT_FOR_PERIODIC_MODE = 0xE000,               ///* Period measurement mode read data command */
} SHT30_CMD;

static uint8_t	SHT30_Send_Cmd(SHT30_CMD cmd);
void SHT30_reset(void);
uint8_t SHT30_Init(void);
uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value);
uint8_t SHT30_Dat_To_Float(uint8_t* const dat, float* temperature, float* humidity);
void SHT30_Read_Dat(uint8_t address, uint8_t* dat, uint8_t size);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */
