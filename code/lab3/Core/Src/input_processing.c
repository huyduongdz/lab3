/*
 * input_processing.c
 *
 *  Created on: Oct 14, 2025
 *      Author: DELL
 */

#include "main.h"
#include "input_reading.h"

extern int mode;
extern int greenTime;
extern int yellowTime;
extern int redTime;
extern int cooldown1;
extern int cooldown2;
extern int greenTemp;
extern int yellowTemp;
extern int redTemp;
extern int temp1;
extern int temp2;
extern int ledIndex;
extern int timeSEG1[2];
extern int timeSEG2[2];
extern int toogleCount;

extern void offLED();
extern void displayWhatSEG1(int time);
extern void displayWhichSEG1(int index);
extern void displaySEG1(int num);
extern void displayWhatSEG2(int time);
extern void displayWhichSEG2(int index);
extern void displaySEG2(int num);

#define N0_OF_BUTTONS 3

enum ButtonState{BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND} ;
enum ButtonState buttonStates[N0_OF_BUTTONS] = {BUTTON_RELEASED, BUTTON_RELEASED, BUTTON_RELEASED};

void fsm_for_input_processing(void){
	for (uint8_t i = 0; i < N0_OF_BUTTONS; i++)
	{
		switch(buttonStates[i]){
		case BUTTON_RELEASED:
			if(is_button_pressed(i)){
				buttonStates[i] = BUTTON_PRESSED;
			}
			break;
		case BUTTON_PRESSED:
			if(!is_button_pressed(i)){
				buttonStates[i] = BUTTON_RELEASED;

				if (mode == 1)
				{
					if (i == 0)
					{
						offLED();
						mode = 2;
						redTemp = redTime;
						yellowTemp = yellowTime;
						greenTemp = greenTime;
					}
				}
				else if (mode == 2)
				{
					if (i == 0)
					{
						offLED();
						mode = 3;
						redTemp = redTime;
						yellowTemp = yellowTime;
						greenTemp = greenTime;
					}
					else if (i == 1)
					{
						redTemp++;
						if (redTemp > 99)
							redTemp = 0;
					}
					else if (i == 2)
					{
						int temp = redTemp - redTime;
						temp = temp + greenTime;
						if (greenTime > 0 && greenTime < 100)
						{
							redTime = redTemp;
							greenTime = temp;
						}
					}
				}
				else if (mode == 3)
				{
					if (i == 0)
					{
						offLED();
						mode = 4;
						redTemp = redTime;
						yellowTemp = yellowTime;
						greenTemp = greenTime;
					}
					else if (i == 1)
					{
						yellowTemp++;
					}
					else if (i == 2)
					{
						int temp = yellowTemp - yellowTime;
						if (redTime + temp < 100 && redTime + temp > 2)
						{
							redTime += temp;
							yellowTime = yellowTemp;
						}
					}
				}
				else if (mode == 4)
				{
					if (i == 0)
					{
						mode = 1;
						redTemp = redTime;
						yellowTemp = yellowTime;
						greenTemp = greenTime;
						cooldown1 = redTime + yellowTime + greenTime;
						cooldown2 = redTime + yellowTime + greenTime;
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
						temp1 = greenTime - 1;
						temp2 = redTime - 1;
					}
					else if (i == 1)
					{
						greenTemp++;
					}
					else if (i == 2)
					{
						int temp = greenTemp - greenTime;
						if ( redTime + temp < 100 && redTime + temp > 2 )
						{
							redTime += temp;
							greenTime = greenTemp;
						}
					}
				}

			} else {
				if(is_button_pressed_1s(i)){
					buttonStates[i] = BUTTON_PRESSED_MORE_THAN_1_SECOND;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1_SECOND:
			if(!is_button_pressed(i)){
				buttonStates[i] = BUTTON_RELEASED;
			}

			if (i == 1)
			{
				if (mode == 2)
				{
					redTemp += 1;
					if (redTemp > 99)
						redTemp = 0;
				}
				if (mode == 3)
				{
					yellowTemp += 1;
					if (yellowTemp > 99)
						yellowTemp = 0;
				}
				if (mode == 4)
				{
					greenTemp += 1;
					if (greenTemp > 99)
						greenTemp = 0;
				}
			}
			break;
		}
	}
}

void fsm_for_1s_logic(void)
{
	switch(mode)
	{
		case 1:
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15); //blinky led

			cooldown1--;
			cooldown2--;

			if (cooldown1 == redTime + yellowTime)
			{
				temp1 = yellowTime;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); //bat vang 1
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
			}
			if (cooldown2 == greenTime + yellowTime)
			{
				temp2 = greenTime;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); //bat xanh 2
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			}
			if (cooldown1 == redTime)
			{
				temp1 = redTime;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //bat do 1
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
			}
			if (cooldown2 == yellowTime)
			{
				temp2 = yellowTime;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); //bat vang 2
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			}
			if (cooldown1 == 0)
			{
				temp1 = greenTime;
				cooldown1 = greenTime + redTime + yellowTime;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //bat xanh 1
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
			}
			if (cooldown2 == 0)
			{
				temp2 = redTime;
				cooldown2 = greenTime + redTime + yellowTime;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); //bat do 2
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
			}

			temp1--;
			temp2--;
			break;
		}
		case 2:
		case 3:
		case 4:
		default:
			break;
	}
}

void fsm_for_display_logic(void)
{
	switch(mode)
	{
		case 1:
		{
			// Quét LED 7-seg cho mode 1
			displayWhatSEG1(temp1);
			displayWhichSEG1(ledIndex);
			displaySEG1(timeSEG1[ledIndex]);

			displayWhatSEG2(temp2);
			displayWhichSEG2(ledIndex);
			displaySEG2(timeSEG2[ledIndex]);

			ledIndex++;
			if (ledIndex > 1)
				ledIndex = 0;
			break;
		}
		case 2:
		{
			// Nhấp nháy LED Đỏ
			if (toogleCount == 0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
			}
			// Hiển thị thời gian Đỏ tạm thời
			displayWhatSEG1(redTemp);
			displayWhichSEG1(ledIndex);
			displaySEG1(timeSEG1[ledIndex]);

			// Hiển thị số 2 (mode 2)
			displayWhichSEG2(1);
			displaySEG2(mode);

			// Logic quét LED
			ledIndex++;
			if (ledIndex > 1)
				ledIndex = 0;
			toogleCount--;
			if (toogleCount < 0)
				toogleCount = 12;
			break;
		}
		case 3:
		{
			// Nhấp nháy LED Vàng
			if (toogleCount == 0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
			}
			// Hiển thị thời gian Vàng tạm thời
			displayWhatSEG1(yellowTemp);
			displayWhichSEG1(ledIndex);
			displaySEG1(timeSEG1[ledIndex]);

			// Hiển thị số 3 (mode 3)
			displayWhichSEG2(1);
			displaySEG2(mode);

			// Logic quét LED
			ledIndex++;
			if (ledIndex > 1)
				ledIndex = 0;
			toogleCount--;
			if (toogleCount < 0)
				toogleCount = 12;
			break;
		}
		case 4:
		{
			// Nhấp nháy LED Xanh
			if (toogleCount == 0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
			}
			// Hiển thị thời gian Xanh tạm thời
			displayWhatSEG1(greenTemp);
			displayWhichSEG1(ledIndex);
			displaySEG1(timeSEG1[ledIndex]);

			// Hiển thị số 4 (mode 4)
			displayWhichSEG2(1);
			displaySEG2(mode);

			// Logic quét LED
			ledIndex++;
			if (ledIndex > 1)
				ledIndex = 0;
			toogleCount--;
			if (toogleCount < 0)
				toogleCount = 12;
			break;
		}
		default:
			break;
	}
}
