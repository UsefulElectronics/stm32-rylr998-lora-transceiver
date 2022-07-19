/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hc_sr501.h"
#include "rylr998.h"
#include "siren.h"
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
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
TIM_HandleTypeDef htim2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  MX_TIM2_Init();

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  rylr998_enable();
  HAL_Delay(100);
  rylr998Get(&hLoRaModule, Rylr998_ADDRESS);
  HAL_Delay(10);
  hLoRaModule.rylr998Transmitter.timer = HAL_GetTick();
  hLoRaModule.rylr998Transmitter.address[0] = '1';
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, hLoRaModule.rylr998Receiver.rxBuffer, 100);
  HAL_Delay(10);


  sirenInitialize(&hSiren,
  					100,
  					10,
  					1);
  sirenSoundLevelSet(&hSiren, (uint32_t*) &htim2.Instance->CCR1, 50);

  sirenStop(&hSiren, (uint32_t*) &htim2.Instance->CCR1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//Start packet parsing once a packet is received over UART
		if(RYLR998_ReadInterruptFlag())
		{
			  RYLR998_WriteInterruptFlag(DISABLE);

			  rylr998ReceivePacketParser(&hLoRaModule);
		}
		//Turn on the Blue LED for 300ms when ULoRa ACK is received
		if(RYLR998_ReadSuccessfulTxFlag())
		{
			 RYLR998_WriteSuccessfulTxFlag(DISABLE);
			 hLoRaModule.rylr998Timer = HAL_GetTick();
			 LED_ON;
		}
		//Send ULORA_CONN_COUNT when a button press is detected every on second
		if(!HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin))
		{
			if(HAL_GetTick() - hLoRaModule.rylr998Transmitter.timer >= 1000)
			{
				hLoRaModule.rylr998Transmitter.timer = HAL_GetTick();

				rylr998Send(&hLoRaModule, ULORA_CONN_COUNT);
			}
		}
		//Keep blue LED turned on for 300ms
		if(HAL_GetTick() - hLoRaModule.rylr998Timer > 300)
		{
		  LED_OFF;
		}
		if(RYLR998_ReadPirSensorPackRxFlag())
		{
			RYLR998_WritePirSensorPackRxFlag(DISABLE);
			rylr998Send(&hLoRaModule, ULORA_ACK);
			hLoRaModule.rylr998Timer = HAL_GetTick();
			LED_ON;
		}

		if(hUloraProtocol.uloraPirDetection)
		{
			if(0 == hSiren.activisionStatus)
			{
				sirenStart(&hSiren,(uint32_t*) &htim2.Instance->CCR1, HAL_GetTick());
			}
		}
		else
		{
			sirenStop(&hSiren,(uint32_t*) &htim2.Instance->CCR1);
		}
		sirenHandler(&hSiren,(uint32_t*) &htim2.Instance->PSC, HAL_GetTick());

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
//	hUart.Instance = USART1;
//	hUart.Init.BaudRate = 115200;
//	hUart.Init.WordLength = UART_WORDLENGTH_8B;
//	hUart.Init.StopBits = UART_STOPBITS_1;
//	hUart.Init.Parity = UART_PARITY_NONE;
//	hUart.Init.Mode = UART_MODE_TX_RX;
//	hUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	hUart.Init.OverSampling = UART_OVERSAMPLING_16;
//	if (HAL_UART_Init(&hUart) != HAL_OK)
//	{
//		Error_Handler();
//	}
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RYLR998_RST_GPIO_Port, RYLR998_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BLUE_LED_Pin */
  GPIO_InitStruct.Pin = BLUE_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLUE_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RYLR998_RST_Pin */
  GPIO_InitStruct.Pin = RYLR998_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RYLR998_RST_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 100;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	RYLR998_WriteInterruptFlag(ENABLE);
	UNUSED(huart);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	RYLR998_WriteInterruptFlag(ENABLE);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, hLoRaModule.rylr998Receiver.rxBuffer, 100);
	 __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}

void receiverTask(void)
{
	//Start packet parsing once a packet is received over UART
	if(RYLR998_ReadInterruptFlag())
	{
		RYLR998_WriteInterruptFlag(DISABLE);

		hLoRaModule.rylr998Timer = HAL_GetTick();

		rylr998ReceivePacketParser(&hLoRaModule);
	}
	//Turn on the Green LED for 300ms when ULoRa Packet is from the transmitter side.
	if(RYLR998_ReadSuccessfulRxFlag())
	{
	  RYLR998_WriteSuccessfulRxFlag(DISABLE);

	  rylr998Send(&hLoRaModule, ULORA_ACK);

	  LED_ON;
	}
	//Keep blue LED turned on for 300ms
	if(HAL_GetTick() - hLoRaModule.rylr998Timer > 300)
	{
	  LED_OFF;
	}
	//Send PIR sensor status continuously to the main alarm board
	if(HAL_GetTick() - hLoRaModule.rylr998SensorTimer  > 500)
	{
		hLoRaModule.rylr998SensorTimer 	= HAL_GetTick();

		hUloraProtocol.uloraPirDetection= hc_sr501ReadStatus(&hPirSensor);

		rylr998Send(&hLoRaModule, ULORA_PIR_SENS);

	}
	//PIR sensor handler
	hc_sr501Handle( &hPirSensor,
				  HAL_GPIO_ReadPin(PIR_OUT_GPIO_Port, PIR_OUT_Pin),
				  HAL_GetTick());
	//ACK handler
	if(RYLR998_ReadSuccessfulTxFlag())
	{
		RYLR998_WriteSuccessfulTxFlag(DISABLE);
		hLoRaModule.rylr998Timer = HAL_GetTick();
		LED_ON;
	}
	if(HAL_GetTick() - hLoRaModule.rylr998Timer > 300)
	{
	  LED_OFF;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
