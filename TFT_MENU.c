/*
 * TFT_MENU.c
 *
 *  Created on: Nov 13, 2020
 *      Author: meh
 */

#include "TFT_MENU.h"
#include "stm32f4xx_hal.h"
#include "tft_st7735_driver.h"
#include "fonts.h"
#include "GFX_FUNCTIONS.h"

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

/*
** Start button unselected
*/
void start_button_unselect(void)
{
	drawRoundRect(29, 19, 102, 27, 8, WHITE);
	fillRoundRect(30, 20, 100, 25, 8, RED);
	ST7735_WriteString(38, 23, "Start Iron", Font_11x18, WHITE, RED);
	startButton = 0;  // start button is not selected
}

/*
** Stop button unselected
*/
void stop_button_unselect(void)
{
	drawRoundRect(29, 69, 102, 27, 8, WHITE);
	fillRoundRect(30, 70, 100, 25, 8, RED);
	ST7735_WriteString(38, 73, "Stop Iron", Font_11x18, WHITE, RED);
	stopButton = 0;  // stop button is not selected
}

/*
** Cancel button unselected
*/
void cancel_button_unselect(void)
{
	drawRoundRect(29, 69, 102, 27, 8, WHITE);
	fillRoundRect(30, 70, 100, 25, 8, RED);
	ST7735_WriteString(38, 23, "CANCEL", Font_11x18, WHITE, RED);
	cancelButton = 0;  // cancel button is not selected
}

/*
** Start button is selected
*/
void start_button_selected(void)
{
	drawRoundRect(29, 19, 102, 27, 8, RED);
	fillRoundRect(30, 20, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 23, "Start Iron", Font_11x18, BLACK, GREEN);
	startButton = 1;  // start button is selected
}

/*
** Stop button is selected
*/
void stop_button_selected(void)
{
	drawRoundRect(29, 69, 102, 27, 8, RED);
	fillRoundRect(30, 70, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 73, "Stop Iron", Font_11x18, BLACK, GREEN);
	stopButton = 1;  // stop button is selected
}

/*
** Cancel button is selected
*/
void cancel_button_selected(void)
{
	drawRoundRect(29, 69, 102, 27, 8, RED);
	fillRoundRect(30, 70, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 23, "CANCEL", Font_11x18, BLACK, GREEN);
	cancelButton = 1;  // cancel button is selected
}

/*
** Draw Ironing
*/
void ironing(void)
{
	ST7735_SetRotation(1);
	fillScreen(WHITE);
	drawRoundRect(29, 19, 102, 27, 8, BLUE);
	fillRoundRect(30, 20, 100, 25, 8, WHITE);
	ST7735_WriteString(38, 23, "IRONING...", Font_11x18, BLACK, BLACK);
}

/*
** Control step motor
*/
void motor_run(void)
{
	int32_t i = 0;
	for (i = 0; i < step; i++) {
		HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_SET);
		HAL_Delay(stepDelay);
		HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_RESET);
		HAL_Delay(stepDelay);
	}
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
}

/******Running Page******/
void running_page(void)
{
	homeMenu = 0;  			// "HomeMenu" is not selected
	pageStartMenu = 1;  		// "PageStartMenu" is selected

	ST7735_SetRotation(1);  			// set horizontal
	fillScreen(WHITE);  				// fill black

	ironing();

	cancel_button_unselect();  			// draw unslected "CANCEL" button

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);

	/*
	** Control motor
	*/
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);					//Enable motor
	motor_run();

	//Change direction
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);
	motor_run();

	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);						//Stop motor
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
	//stop_button_unselect();
}

/*
** EXTI callback function
** PA1 is ENTER
** PA4 is DOWN
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_1)  // If the PA1 (ENTER) is pressed
	{
		downButton = 0;  	// downButton set to 0
		enterButton = 1;  	// enterButton set to 1
		//homeMenu = 0;
		//count = 1;
	}

	if (GPIO_Pin == GPIO_PIN_2)  // If the PA2 (DOWN) is pressed
	{
		downButton = 1;  	// downButton set to 1 and others set to 0
		enterButton = 0;
		//homeMenu = 0;
		//count = 1;
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










/*
** Start button unselected
*/
void start_button_unselect(void)
{
	drawRoundRect(29, 19, 102, 27, 8, WHITE);
	fillRoundRect(30, 20, 100, 25, 8, RED);
	ST7735_WriteString(38, 23, "Start Iron", Font_11x18, WHITE, RED);
	startButton = 0;  // start button is not selected
}

/*
** Stop button unselected
*/
void stop_button_unselect(void)
{
	drawRoundRect(29, 69, 102, 27, 8, WHITE);
	fillRoundRect(30, 70, 100, 25, 8, RED);
	ST7735_WriteString(38, 73, "Stop Iron", Font_11x18, WHITE, RED);
	stopButton = 0;  // stop button is not selected
}

/*
** Cancel button unselected
*/
void cancel_button_unselect(void)
{
	drawRoundRect(29, 69, 102, 27, 8, WHITE);
	fillRoundRect(30, 70, 100, 25, 8, RED);
	ST7735_WriteString(38, 23, "CANCEL", Font_11x18, WHITE, RED);
	cancelButton = 0;  // cancel button is not selected
}

/*
** Start button is selected
*/
void start_button_selected(void)
{
	drawRoundRect(29, 19, 102, 27, 8, RED);
	fillRoundRect(30, 20, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 23, "Start Iron", Font_11x18, BLACK, GREEN);
	startButton = 1;  // start button is selected
}

/*
** Stop button is selected
*/
void stop_button_selected(void)
{
	drawRoundRect(29, 69, 102, 27, 8, RED);
	fillRoundRect(30, 70, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 73, "Stop Iron", Font_11x18, BLACK, GREEN);
	stopButton = 1;  // stop button is selected
}

/*
** Cancel button is selected
*/
void cancel_button_selected(void)
{
	drawRoundRect(29, 69, 102, 27, 8, RED);
	fillRoundRect(30, 70, 100, 25, 8, GREEN);
	ST7735_WriteString(38, 23, "CANCEL", Font_11x18, BLACK, GREEN);
	cancelButton = 1;  // cancel button is selected
}

/*
** Draw Ironing
*/
void ironing(void)
{
	ST7735_SetRotation(1);
	fillScreen(WHITE);
	drawRoundRect(29, 19, 102, 27, 8, BLUE);
	fillRoundRect(30, 20, 100, 25, 8, WHITE);
	ST7735_WriteString(38, 23, "IRONING...", Font_11x18, BLACK, BLACK);
}

/*
** Control step motor
*/
void motor_run(void)
{
	downButton = 0;
	int32_t i = 0;
	for (i = 0; i < step; i++) {
		if (downButton != 1) {
			HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_SET);
			HAL_Delay(stepDelay);
			HAL_GPIO_WritePin(STEP_PORT, STEP_PIN, GPIO_PIN_RESET);
			HAL_Delay(stepDelay);
		} else {
			HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
			homeMenu = 1;
			HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_RESET);
		}


	}
}

/******Home Menu******/
void home_menu(void)
{
	ST7735_SetRotation(1);  			// set horizontal
	fillScreen(WHITE);  				// fill black

    if (shtc3_perform_measurements(&hi2c1, &temp_1, &hum_1) &&
    			  shtc3_perform_measurements(&hi2c3, &temp_2, &hum_2)) {
    	temp_average = ((temp_1 + temp_2) / 2.0) / 100.0;
    	humid_average = (hum_1 + hum_2) / 2.0;
    }
    sprintf(temp_str, "temp: %ld C", temp_average);
	sprintf(humid_str, "humid: %ld %c", humid_average, '%');

	ST7735_WriteString(10, 10, temp_str, Font_7x10, BLUE, BLACK);
	ST7735_WriteString(10, 20, humid_str, Font_7x10, BLUE, BLACK);

    start_button_unselect();  			// draw unselected "Start Iron" button
	stop_button_unselect();  			// draw unselected "Stop Iron" button
	homeMenu = 1;  			// "homeMenu" is selected
	pageStartMenu = 0;  		// "PageStartMenu" is not selected
	pageStopMenu = 0;
}

/******Running Page******/
void running_page(void)
{
	homeMenu = 0;  			// "HomeMenu" is not selected
	pageStartMenu = 1;  		// "PageStartMenu" is selected

	ST7735_SetRotation(1);  			// set horizontal
	fillScreen(WHITE);  				// fill black

	ironing();

	cancel_button_unselect();  			// draw unslected "CANCEL" button

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);

	/*
	** Control motor
	*/
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);					//Enable motor
	motor_run();

	//Change direction
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);
	motor_run();

	HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);						//Stop motor

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
	//stop_button_unselect();
}

/*
** EXTI callback function
** PA1 is ENTER
** PA4 is DOWN
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_1)  // If the PA1 (ENTER) is pressed
	{
		downButton = 0;  	// downButton set to 0
		enterButton = 1;  	// enterButton set to 1
		//homeMenu = 0;
		//count = 1;
	}

	if (GPIO_Pin == GPIO_PIN_2)  // If the PA2 (DOWN) is pressed
	{
		downButton = 1;  	// downButton set to 1 and others set to 0
		enterButton = 0;
		//homeMenu = 0;
		//count = 1;
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


