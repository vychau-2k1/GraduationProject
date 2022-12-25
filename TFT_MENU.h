/*
 * TFT_MENU.h
 *
 *  Created on: 20/12/2022
 *      Author: Vy
 */

#ifndef INC_TFT_MENU_H_
#define INC_TFT_MENU_H_

#define MOTOR_PORT	GPIOE
#define MOTOR_PIN	GPIO_PIN_7

#define EN_PORT		GPIOE
#define EN_PIN		GPIO_PIN_9

#define STEP_PORT	GPIOE
#define STEP_PIN	GPIO_PIN_10

#define DIR_PORT	GPIOE
#define DIR_PIN		GPIO_PIN_11

#define step  		60000
#define stepDelay	500

void start_button_unselect(void);
void stop_button_unselect(void);
void cancel_button_unselect(void);
void start_button_selected(void);
void stop_button_selected(void);
void cancel_button_selected(void);
void ironing(void);
void motor_run(void);
void home_menu(void);
void running_page(void);
void stop_page(void);
void menu_handler(void);


#endif /* INC_TFT_MENU_H_ */
