#include "startup.h"
#include "uart_printf.h"

extern uint8_t device_name[25];

void first_boot()
{	
	uint8_t dev_name[] ="STM32name";
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t seconds = 0;
	uint8_t week_day[] ="Mon"; 
	uint8_t month[] ="Jan";
	uint8_t day = 1;
	uint8_t year = 0;
	
	uint8_t data[100];
	
	TIME_setTime(hours, minutes, seconds, week_day, month, day, year);

	
	concatenate_data(data, dev_name, hours, minutes, seconds, week_day, month, day, year);
	uint8_t numofwords = (sizeof(data)/4)+((sizeof(data)%4)!=0);

	Flash_Write_Data(0x080E0000, (uint32_t *)data, numofwords);
	strcpy((char*)device_name,(char*)dev_name);
}

void concatenate_data(uint8_t *buffer,
                      const uint8_t *dev_name,
                      uint8_t hours,
                      uint8_t minutes,
                      uint8_t seconds,
                      const uint8_t *week_day,
                      const uint8_t *month,
                      uint8_t day,
                      uint8_t year) 
											{
		strcpy((char *)buffer, (char *)dev_name);
    
    // Append delimiter
    strcat((char *)buffer, ",");
    
    // Append hours
    char temp[3];
    int_to_str(hours, temp);
    strcat((char *)buffer, temp);
    strcat((char *)buffer, ",");
    
    // Append minutes
    int_to_str(minutes, temp);
    strcat((char *)buffer, temp);
    strcat((char *)buffer, ",");
    
    // Append seconds
    int_to_str(seconds, temp);
    strcat((char *)buffer, temp);
    strcat((char *)buffer, ",");
    
    // Append week day
    strcat((char *)buffer, (char *)week_day);
    strcat((char *)buffer, ",");
    
    // Append month
    strcat((char *)buffer, (char *)month);
    strcat((char *)buffer, ",");
    
    // Append date
    int_to_str(day, temp);
    strcat((char *)buffer, temp);
    strcat((char *)buffer, ",");
    
    // Append year
    int_to_str(year, temp);
    strcat((char *)buffer, temp);
		strcat((char *)buffer, ",");
}
											
void int_to_str(uint8_t value, char *str) {
    // Convert tens digit
    str[0] = '0' + (value / 10);
    // Convert units digit
    str[1] = '0' + (value % 10);
    // Null-terminate the string
    str[2] = '\0';
}

void star_function(uint32_t *Data)
{
	uint8_t hours, minutes, seconds, *week_day, *month, day, year;
	char *dev_name;
	uint8_t string[100];
	Convert_To_Str(Data, string);
	dev_name = strtok((char *)string, ",");
	hours = (uint8_t)atoi(strtok(NULL, ","));
	minutes = (uint8_t)atoi(strtok(NULL, ","));
	seconds = (uint8_t)atoi(strtok(NULL, ","));
	week_day = (uint8_t*)strtok(NULL, ",");
	month = (uint8_t*)strtok(NULL, ",");
	day =  (uint8_t)atoi(strtok(NULL, ","));
	year = (uint8_t)atoi(strtok(NULL, ","));
	
	//UART_Printf_Dbg("Data = %s,%d,%d,%d,%s,%s,%d,%d\r\n", dev_name, hours, minutes, seconds, week_day, month, day, year);
	
	TIME_setTime(hours, minutes, seconds, week_day, month, day, year);
	strcpy((char*)device_name, dev_name);
	
}
void data_save(const uint8_t *dev_name,
											uint8_t hours,
											uint8_t minutes,
											uint8_t seconds,
											uint8_t week_day_int,
											uint8_t month_int,
											uint8_t day,
											uint8_t year)
{
	uint8_t data[100];
	uint8_t month[4];
	uint8_t week_day[4];
	
	//UART_Printf_Dbg("Data = %d, %d", week_day_int, month_int);
	
	switch (week_day_int)
	{
		case 1:
			strcpy((char *)week_day, "Mon");
			break;
		case 2:
			strcpy((char *)week_day, "Tue");
			break;
		case 3:
			strcpy((char *)week_day, "Wed");
			break;
		case 4:
			strcpy((char *)week_day, "Thu");
			break;
		case 5:
			strcpy((char *)week_day, "Fri");
			break;
		case 6:
			strcpy((char *)week_day, "Sat");
			break;
		case 7:
			strcpy((char *)week_day, "Sun");
			break;
	}
	
	switch (month_int)
	{
		case 1:
			strcpy((char *)month, "Jan");
			break;
		case 2:
			strcpy((char *)month, "Feb");
			break;
		case 3:
			strcpy((char *)month, "Mar");
			break;
		case 4:
			strcpy((char *)month, "Apr");
			break;
		case 5:
			strcpy((char *)month, "May");
			break;
		case 6:
			strcpy((char *)month, "Jun");
			break;
		case 7:
			strcpy((char *)month, "Jul");
			break;
		case 8:
			strcpy((char *)month, "Aug");
			break;
		case 9:
			strcpy((char *)month, "Sep");
			break;
		case 10:
			strcpy((char *)month, "Oct");
			break;
		case 11:
			strcpy((char *)month, "Nov");
			break;
		case 12:
			strcpy((char *)month, "Dec");
			break;
	}
	
	concatenate_data(data, dev_name, hours, minutes, seconds, week_day, month, day, year);
	uint8_t numofwords = (sizeof(data)/4)+((sizeof(data)%4)!=0);
	Flash_Write_Data(0x080E0000, (uint32_t *)data, numofwords);
}