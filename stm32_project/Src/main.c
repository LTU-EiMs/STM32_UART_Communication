/* USER CODE BEGIN Header */
#include "tmpsensor.h"
#include "uart_printf.h"
#include "time.h"
#include <stdbool.h>
#include "nw_memory.h"
#include "startup.h"

/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef struct AdcValues{
uint16_t Raw[2]; /* Raw values from ADC */
double IntSensTmp; /* Temperature */
}adcval_t;
adcval_t Adc;

#define MainBuf_SIZE 50
#define RxBuf_SIZE 50

bool ADC_Flg = false;
bool user_button_Flg = false;
bool dma_rx_complete_flag = false;
bool writing_data = false;

RTC_DateTypeDef getDate = {0};
RTC_TimeTypeDef getTime = {0};
RTC_DateTypeDef* pDate = &getDate;
RTC_TimeTypeDef* pTime = &getTime;

uint8_t UART2_MainBuffer[MainBuf_SIZE];
uint8_t UART2_rxBuffer[MainBuf_SIZE];
uint8_t device_name[25];
uint8_t stm_convert_id[25];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char *header;
	uint8_t hours, minutes, seconds, *week_day, *month, day, year;
	uint32_t stm_id[3];
	uint32_t Rx_Data[50];
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Adc.Raw, 2);
	TIM3->SR &= ~TIM_SR_UIF;
	
		// Loads data from flash

	//first_boot(); // for debugging 
	if (Flash_Read_Data(0x080E0000, Rx_Data)) first_boot(); // writes data for first boot 
	else star_function(Rx_Data);
	
	// gets STM UID 
	stm_id[0]=HAL_GetUIDw0();
	stm_id[1]=HAL_GetUIDw1();
	stm_id[2]=HAL_GetUIDw2();
	UID_convert(stm_id, stm_convert_id);
	
	//HAL_UART_Receive_IT (&huart2, UART2_rxBuffer, 25);
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, UART2_rxBuffer, RxBuf_SIZE);
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	
	
	HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		while(1)
		{
			if (dma_rx_complete_flag) {			
				UART_Printf_Dbg("New message\r\n");
				header = strtok((char *)UART2_MainBuffer, ",");
				if (strcmp(header, "COMMAND_DATE") == 0) // date sync command 
					{
						hours = (uint8_t)atoi(strtok(NULL, ","));
						minutes = (uint8_t)atoi(strtok(NULL, ","));
						seconds = (uint8_t)atoi(strtok(NULL, ","));
						week_day = (uint8_t*)strtok(NULL, ",");
						month = (uint8_t*)strtok(NULL, ",");
						day =  (uint8_t)atoi(strtok(NULL, ","));
						year = (uint8_t)atoi(strtok(NULL, ","));
						HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_7);
						HAL_Delay(1000);
						HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_7);
						TIME_setTime(hours, minutes, seconds, week_day, month, day, year);
					}
				else if (strcmp(header, "COMMAND_NAME") == 0) // gets command with new name
				{
					memset(device_name, '\0', 25);
					strcpy((char*)device_name, strtok(NULL, ","));
					//HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_9); //green led is dead :/
					HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_7); // blue led
					HAL_Delay(1000);
					HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_7);
					/*if (!writing_data)
					{
					writing_data = true;
					data_save(device_name,pTime->Hours,pTime->Minutes, pTime->Seconds, pDate->WeekDay, pDate->Month, pDate->Date, pDate->Year;
					writing_data = false;
					}*/ // for some reason UARTS2 stops working after this 
				}
				else // bad command 
				{
					UART_Printf_Dbg("Bad message\r\n");
					HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_14);
					HAL_Delay(1000);
					HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_14);
				}
				memset(UART2_MainBuffer, '\0', MainBuf_SIZE);
				dma_rx_complete_flag = false;
				
			}
			HAL_Delay(50);
		}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 13;
  RCC_OscInitStruct.PLL.PLLN = 195;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) /* Check if the interrupt comes from ACD1 */
    {
		ADC_Flg = true;
    }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
    if(GPIO_Pin == GPIO_PIN_13) // If The INT Source Is EXTI Line13 (PC13 Pin)
    {
			if(HAL_GPIO_ReadPin (GPIOC, GPIO_PIN_13)) user_button_Flg = true;
			else user_button_Flg = false;
			//UART_Printf_Dbg("Button trigger\r\n");
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2 )
  {
		if (ADC_Flg)
		{
			TIME_getdate_time(&pDate,&pTime);
			ADC_Flg = false;
			Adc.IntSensTmp = TMPSENSOR_getTemperature(Adc.Raw[1], Adc.Raw[0]);
			//UART_Printf_Dbg("%02d:%02d:%02d\r\n", pTime->Hours, pTime->Minutes, pTime->Seconds);
			//UART_Printf_Dbg("%02d/%02d/%02d\r\n", pDate->Date,  pDate->Month,  (pDate->Year)+2000);
			//UART_Printf_Dbg("Reference: Adc.Raw[0] = %u\r\n", Adc.Raw[0]);
			//UART_Printf_Dbg("Sensor: Adc.Raw[1] = %u\r\n", Adc.Raw[1]);
			//UART_Printf_Dbg("Temperature: Adc.IntSensTmp = %.2f?\r\n", Adc.IntSensTmp);
			//UART_Printf_Dbg("Button status: %s\r\n", user_button_Flg ? "True" : "False");
			
			// Outputs data to UI
			UART_Printf("SENSOR_DATA,%.2f,%02d,%02d,%02d,%02d,%02d,%02d,%d,%s,%s,\n",Adc.IntSensTmp,pTime->Hours, pTime->Minutes, pTime->Seconds,pDate->Date,  pDate->Month, pDate->Year, user_button_Flg, stm_convert_id, device_name);
			//UART_Printf_Dbg("SENSOR_DATA,%.2f,%02d,%02d,%02d,%02d,%02d,%02d,%d\r\n",Adc.IntSensTmp,pTime->Hours, pTime->Minutes, pTime->Seconds,pDate->Date,  pDate->Month, pDate->Year, user_button_Flg);
		}
  }
		else if (htim == &htim4) // AUTO SAVES DATA 
			{
				if (!writing_data)
				{
					writing_data = true;
					data_save(device_name,pTime->Hours,pTime->Minutes, pTime->Seconds, pDate->WeekDay, pDate->Month, pDate->Date, pDate->Year); 
					writing_data = false;
				}
			}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART2)
	{
		
		memcpy ((uint8_t *)UART2_MainBuffer, UART2_rxBuffer, Size);
		dma_rx_complete_flag = true;
			
		/* start the DMA again */
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *) UART2_rxBuffer, RxBuf_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
