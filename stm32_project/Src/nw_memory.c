#include "nw_memory.h"
#include <ctype.h>


uint8_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SECTORError;
	int sofar=0;


	 /* Unlock the Flash to enable the flash control register access *************/
	  HAL_FLASH_Unlock();

	  /* Erase the user Flash area */

	  /* Get the number of sector to erase from 1st sector */

	  uint32_t StartSector = FLASH_SECTOR_11;
	  uint32_t EndSectorAddress = StartSectorAddress + numberofwords*4;
	  uint32_t EndSector = FLASH_SECTOR_11;

	  /* Fill EraseInit structure*/
	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	  EraseInitStruct.Sector        = StartSector;
	  EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;

	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	     you have to make sure that these data are rewritten before they are accessed during code
	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	     DCRST and ICRST bits in the FLASH_CR register. */
	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	  {
		  return HAL_FLASH_GetError ();
	  }
	  /* Program the user Flash area word by word
	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	   while (sofar<numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartSectorAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	  /* Lock the Flash to disable the flash control register access (recommended
	     to protect the FLASH memory against possible unwanted operation) *********/
	  HAL_FLASH_Lock();

	   return 0;
}

uint8_t Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *RxBuf)
{
	uint32_t buffer;
	
  if (*(__IO uint32_t *)StartSectorAddress==0x00000000) return 1;
	
	while (1)
	{
		buffer = *(__IO uint32_t *)StartSectorAddress;
		if (buffer==0xFFFFFFFF) break;
		*RxBuf = buffer;
		StartSectorAddress += 4;
		RxBuf++;
	}
	return 0;
}

void Convert_To_Str(uint32_t *Data, uint8_t *Buf)
{
		int numberofbytes = (sizeof(uint8_t) * 4) * (sizeof(Data) * sizeof(uint32_t));

    for (int i = 0; i < numberofbytes; i++)
    {
        Buf[i] = (uint8_t)(Data[i / 4] >> (8 * (i % 4)));
    }
}

void UID_convert(uint32_t *Data, uint8_t *Buf)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            uint8_t byte = (Data[i] >> (8 * j)) & 0xFF;
            
            // Check if the byte is alphanumeric
            if (isalnum(byte))
            {
                Buf[i * 4 + j] = byte;
            }
            else
            {
                // Replace non-alphanumeric characters with a default value, e.g., '0'
                Buf[i * 4 + j] = '0';
            }
        }
    }
}