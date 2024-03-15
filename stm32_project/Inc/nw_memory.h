#ifndef INC_NW_MEMORY_H_
#define INC_NW_MEMORY_H_

#include "stm32f2xx_hal.h"
#include "string.h"



#define FLASH_VOLTAGE_RANGE_1 0x00000000U // 1.8V - 2.1V
#define FLASH_VOLTAGE_RANGE_2 0x00000001U // 2.1V - 2.7V
#define FLASH_VOLTAGE_RANGE_3 0x00000002U // 2.7V - 3.6V
#define FLASH_VOLTAGE_RANGE_4 0x00000003U // 2.7V - 3.6V + external Vpp



uint8_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);
uint8_t Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf);
void Convert_To_Str (uint32_t *Data, uint8_t *Buf);
void UID_convert(uint32_t *Data, uint8_t *Buf);

#endif