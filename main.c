/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TFT_MENU.h"
#include "tft_st7735_driver.h"
#include "shtc3_hal_driver.h"
#include "fonts.h"
#include "GFX_FUNCTIONS.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
  int32_t temp_1, temp_2;
  int32_t hum_1, hum_2;
  int32_t temp_average;
  int32_t humid_average;
  char temp_str[30], humid_str[30];

  /*
  ** Menu home define
  */
  int8_t homeMenu = 0;
  int8_t pageStartMenu = 0;
  int8_t startButton = 0;
  int8_t stopButton = 0;
  int8_t cancelButton = 0;
  int8_t pageStopMenu = 0;

  /*
  ** Physical button state
  */
  int8_t downButton = 0;
  int8_t enterButton = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C3_Init(void);
static void MX_TIM1_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_HOST_Init();
  MX_I2C3_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);

  ST7735_Init(0);
  home_menu();

  if (!shtc3_read_id(&hi2c1) || !shtc3_read_id(&hi2c3)) {
    // Sensor is not found
  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */

    menu_handler();
  }
  /* USER CODE END 3 */
}
/*
** Start button unselected
*/
void start_button_unselect(void)
{
	//drawRoundRect(29, 39, 102, 27, 8, WHITE);
	//fillRoundRect(30, 40, 100, 25, 8, RED);
	ST7735_WriteString(38, 33, "Start Iron", Font_11x18, WHITE, RED);
	startButton = 0;  // start button is not selected
}

/*
** Stop button unselected
*/
void stop_button_unselect(void)
{
	//drawRoundRect(29, 79, 102, 27, 8, WHITE);
	//fillRoundRect(30, 80, 100, 25, 8, RED);
	ST7735_WriteString(38, 83, "Stop Iron", Font_11x18, WHITE, RED);
	stopButton = 0;  // stop button is not selected
}

/*
** Cancel button unselected
*/
void cancel_button_unselect(void)
{
	//drawRoundRect(29, 79, 102, 27, 8, WHITE);
	//fillRoundRect(30, 80, 100, 25, 8, RED);
	ST7735_WriteString(38, 83, "CANCEL", Font_11x18, WHITE, RED);
	cancelButton = 0;  // cancel button is not selected
}

/*
** Start button is selected
*/
void start_button_selected(void)
{
	//drawRoundRect(29, 39, 102, 27, 8, RED);
	//fillRoundRect(30, 40, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 33, "Start Iron", Font_11x18, BLACK, GREEN);
	startButton = 1;  // start button is selected
}

/*
** Stop button is selected
*/
void stop_button_selected(void)
{
	//drawRoundRect(29, 79, 102, 27, 8, RED);
	//fillRoundRect(30, 80, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 83, "Stop Iron", Font_11x18, BLACK, GREEN);
	stopButton = 1;  // stop button is selected
}

/*
** Cancel button is selected
*/
void cancel_button_selected(void)
{
	//drawRoundRect(29, 79, 102, 27, 8, RED);
	//fillRoundRect(30, 80, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 33, "IRONING...", Font_11x18, WHITE, WHITE);
	ST7735_WriteString(38, 83, "CANCEL", Font_11x18, BLACK, GREEN);
	cancelButton = 1;  // cancel button is selected
}

/*
** Draw Ironing
*/
void ironing(void)
{
	ST7735_SetRotation(1);
	fillScreen(WHITE);
	//drawRoundRect(39, 39, 102, 27, 8, BLUE);
	//fillRoundRect(40, 40, 100, 25, 8, WHITE);
	ST7735_WriteString(38, 33, "IRONING...", Font_11x18, BLACK, RED);
}

/*
** Control step motor
*/
void motor_run(void)
{
	downButton = 0;
	int32_t i = 0;
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_SET);
	HAL_Delay(500);

	for (i = 0; i < step; i++) {
			HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_SET);
			delay_us(stepDelay);
			HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_RESET);
			delay_us(stepDelay);
	}
	//HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_RESET);
}

/******Home Menu******/
void home_menu(void)
{
	ST7735_SetRotation(1);  			// set horizontal
	fillScreen(WHITE);  				// fill black



    start_button_unselect();  			// draw unselected "Start Iron" button
	stop_button_unselect();  			// draw unselected "Stop Iron" button
	homeMenu = 1;  			// "homeMenu" is selected
	pageStartMenu = 0;  		// "PageStartMenu" is not selected
	pageStopMenu = 0;

//	while (homeMenu && downButton == 0) {
//		 if (shtc3_perform_measurements(&hi2c1, &temp_1, &hum_1) ||
//			 shtc3_perform_measurements(&hi2c3, &temp_2, &hum_2)) {
//			    	temp_average = ((temp_1 + temp_2) / 1.0) / 100.0;
//			    	humid_average = (hum_1 + hum_2) / 1.0;
//			    }
//		sprintf(temp_str, "temp: %ld C", temp_average);
//		sprintf(humid_str, "humid: %ld %c", humid_average, '%');
//
//		ST7735_WriteString(10, 10, temp_str, Font_7x10, BLUE, WHITE);
//		ST7735_WriteString(10, 20, humid_str, Font_7x10, BLUE, WHITE);
//
//		HAL_Delay(1000);
//	}

}

/******Running Page******/
void running_page(void)
{
	homeMenu = 0;  			// "HomeMenu" is not selected
	pageStartMenu = 1;  		// "PageStartMenu" is selected

	ST7735_SetRotation(1);  			// set horizontal
	fillScreen(WHITE);  				// fill black
	ironing();
	while (pageStartMenu) {
			 if (shtc3_perform_measurements(&hi2c1, &temp_1, &hum_1) &&
				 shtc3_perform_measurements(&hi2c3, &temp_2, &hum_2)) {	//Confirm STM32 can read value
				    	temp_average = ((temp_1 + temp_2) / 1.0) / 100.0;
				    	humid_average = (hum_1 + hum_2) / 1.0;
				    }
			//Convert Integer -> String to Write to Screen
			sprintf(temp_str, "temp: %ld C", temp_average);
			sprintf(humid_str, "humid: %ld %c", humid_average, '%');

			//Write Sensor Value to Screen
			ST7735_WriteString(10, 10, temp_str, Font_7x10, BLUE, WHITE);
			ST7735_WriteString(10, 20, humid_str, Font_7x10, BLUE, WHITE);

			HAL_Delay(1000);	//Read value 1 time per second

			//Check if have hot air, it will break the while loop to start motor
			if (humid_average > 80) {
				break;
			}
	}

	cancel_button_unselect();  			// draw unslected "CANCEL" button

	//HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(20000);
	/*
	** Control motor
	*/

	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);					//Enable motor
	motor_run();
	HAL_Delay(500);

	//Change direction
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);
	motor_run();

	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);						//Stop motor
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_RESET);

	homeMenu = 1;
}

/******Stop Page******/
void stop_page(void)
{
	homeMenu = 0;
	pageStopMenu = 1;

	ST7735_SetRotation(1);
	fillScreen(CYAN);

	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);

	HAL_Delay(3000);
	//homeMenu = 1;
	//stSop_button_unselect();
}

/*
** EXTI callback function
** PA1 is ENTER
** PA2 is DOWN
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_1)  // If the PA1 (ENTER) is pressed
	{
		downButton = 0;  	// downButton set to 0
		enterButton = 1;  	// enterButton set to 1
	}

	if (GPIO_Pin == GPIO_PIN_2)  // If the PA2 (DOWN) is pressed
	{
		downButton = 1;  	// downButton set to 1 and others set to 0
		enterButton = 0;
	}
}

void menu_handler(void)
{
	if (homeMenu) {
		home_menu();		//Draw home menu

		while (homeMenu) {	//Control inside home menu

			if (downButton) {		//If press down button
				HAL_Delay(200); 	//Debounce button

				if (startButton) {
					start_button_unselect();
					stop_button_selected();

					downButton = 0;	//reset down button
					startButton = 0;	//reset start button
				} else if (stopButton) {
					stop_button_unselect();
					start_button_selected();

					downButton = 0;	//reset down button
					stopButton = 0;	//reset stop button
				} else {	//If none of button is selected
					start_button_selected();
					stop_button_unselect();

					downButton = 0;
				}

			} else if (enterButton) {	//If press enter
				HAL_Delay(200);		//Debounce button

				if (startButton) {
					pageStartMenu = 1;
					homeMenu = 0;

					enterButton = 0;
					startButton = 0;
				} else if (stopButton) {
					pageStopMenu = 1;
					homeMenu = 0;

					enterButton = 0;
					stopButton = 0;
				} else ;


			}
		}
	} else if (pageStartMenu) {
		running_page();			//Draw running page

		while (pageStartMenu) {	//Control inside page running

			if (downButton) {
				HAL_Delay(200);		//Debounce button

				cancel_button_selected();	//Draw select cancel button

				downButton = 0;	//Reset button state
			} else if (enterButton) {
				if (cancelButton) {
					homeMenu = 1;		//Back to home menu
					pageStartMenu = 0;
					cancelButton = 0;

					enterButton = 0;
				} else ;
			} else ;
		}
	} else if (pageStopMenu) {
		stop_page();			//Draw stop page

		HAL_Delay(200);

		homeMenu = 1;		//Back to home menu
		pageStopMenu = 0;	//Reset state
	}
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 168-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|Relay_Motor_Pin|Relay_Heat_Pin|EN_PIN_Pin
                          |STEP_PIN_Pin|DIR_PIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, CS_PIN_Pin|DC_PIN_Pin|RST_PIN_Pin|LD4_Pin
                          |LD3_Pin|LD5_Pin|LD6_Pin|Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Button_Enter_Pin Button_Down_Pin */
  GPIO_InitStruct.Pin = Button_Enter_Pin|Button_Down_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Relay_Motor_Pin Relay_Heat_Pin EN_PIN_Pin STEP_PIN_Pin
                           DIR_PIN_Pin */
  GPIO_InitStruct.Pin = Relay_Motor_Pin|Relay_Heat_Pin|EN_PIN_Pin|STEP_PIN_Pin
                          |DIR_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_PIN_Pin DC_PIN_Pin RST_PIN_Pin */
  GPIO_InitStruct.Pin = CS_PIN_Pin|DC_PIN_Pin|RST_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

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
