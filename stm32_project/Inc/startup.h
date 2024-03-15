#ifndef INC_STARTUP_H_
#define INC_STARTUP_H_

#include "main.h"
#include "nw_memory.h"
#include "time.h"
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

void first_boot();
void concatenate_data(uint8_t *buffer,
                      const uint8_t *dev_name,
                      uint8_t hours,
                      uint8_t minutes,
                      uint8_t seconds,
                      const uint8_t *week_day,
                      const uint8_t *month,
                      uint8_t date,
                      uint8_t year);
void int_to_str(uint8_t value, char *str);
void star_function(uint32_t *Data);
void data_save(const uint8_t *dev_name,
											uint8_t hours,
											uint8_t minutes,
											uint8_t seconds,
											uint8_t week_day_int,
											uint8_t month_int,
											uint8_t day,
											uint8_t year);

#endif