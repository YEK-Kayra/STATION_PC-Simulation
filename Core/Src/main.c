/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "StationTelemetry.h"
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/*---BEGIN---------My externed variable------------*/
extern Variable_HandleTypeDef data;		 /* is for save all telemetry data into the struct's varaibles */
extern uint16_t Written_Bytes;			 /* is for save number of total converted buffer's characters  */
/*---END-----------My externed variable------------*/


/*! brief : This array is for getting command from the Station PC
 * 			received datas will be download into the array to extract
 *
 * 	note  : There are few character
 * 				@arg :  '*' is the enable command  character for related mission
 * 				@arg :  '#' is the disable command character for related mission
 * 				@arg :  '%' is the request command character to send your telemetry data to the Station PC
 *
 *
 * [0] = manuel deployment
 * [1] = swtich camera
 * [2] = Colorization number_1
 * [3] = Colorization letter_1
 * [4] = Colorization number_2
 * [5] = Colorization letter_2
 * [6] = Request getting telemetry packet
 */
char Stationcommands[7]={0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief : Station PC programmer should look at here for sending command
 *
 * 			For manuel deployment ==> *#????#
 * 			For switch camera 	  ==> #*????#
 * 			For colorization task ==> ##RHRH#
 * 			For request telemetry datas from STM32 mcu of Station part ==> ##????%
 *
 * @note : 	if you want to get data and doing manuel deployment at the same time
 * 			Station PC send the data to the STM32 mcu as like *#????%
 * 			this is not a problem because we are using if else blocks
 *
 */
//TODO : We add codes
/* This same code blocks will be using in payload code of satellite */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	/*! For manuel deployment */
	if(Stationcommands[0] == '*'){
		/**/
	}
	else if(Stationcommands[0] == '#'){
		/**/
	}


	/* For switch camera */
	if(Stationcommands[1] == '*'){
		/**/
	}
	else if(Stationcommands[0] == '#'){
		/**/
	}


	/* For image colorization task */
	if((Stationcommands[2] !='?') && (Stationcommands[3] !='?') && (Stationcommands[4] !='?') && (Stationcommands[5] !='?')){

		data.rhrh[0]= Stationcommands[2];
		data.rhrh[1]= Stationcommands[3];
		data.rhrh[2]= Stationcommands[4];
		data.rhrh[3]= Stationcommands[5];

	}


	/* For sending */
	if(Stationcommands[6] == '%'){
		/* Run create and sending telemetry packet functions to push and print data via USBtoTTL */
		data.PermissionFlag = Permission_On;
	}
	else if(Stationcommands[6] == '#'){
		data.PermissionFlag = Permission_Off;
	}


	/* Reset all indexes values */
	for(int i = 0; i < sizeof(Stationcommands) ; i++){
		Stationcommands[i]=0;
	}

	/* Send the array to the Payload of Satellite */
	//HAL_UART_Transmit_IT(&huart2, Stationcommands, sizeof(Stationcommands));

	/* Start again uart receive interrupt function */
	HAL_UART_Receive_IT(&huart2, Stationcommands, sizeof(Stationcommands));
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /*! Set first startup values of the system */
  	  SATELLITE_set_first_values();

  /*! Upload the datas into the releated variables*/
  	  SATELLITE_fill_array();

  /*! Gets datas from the Station PC by using IT */
  	  HAL_UART_Receive_IT(&huart2, Stationcommands, sizeof(Stationcommands));


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	/*! First step is from ground to peak point. This value is compatible the process */
		  SATELLITE_ready_for_flight__to__apogie();

	/*! First step is from peak point to ground. This value is compatible the process */
		  SATELLITE_apogie__to__ground();

	/*! After all satellite process, Upload first step value into the variables */
		  SATELLITE_set_first_values();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
