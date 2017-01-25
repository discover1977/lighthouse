/*
 * main.c
 *
 *  Created on: 22 џэт. 2017 у.
 *      Author: gavrilov.iv
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "ws2812b/ws2812.h"
#include <util/delay.h>
#include "bits_macros.h"
#include "buttons.h"

volatile uint8_t ButtonCode, ButtonEvent;
volatile uint8_t Power = 1;
volatile uint8_t PowerON = 0;

void get_but()
{
	ButtonCode = BUT_GetBut();
	if ( ButtonCode )
	{
		ButtonEvent = BUT_GetBut();
	}
}

void setWhite(uint8_t number)
{
	uint8_t LedNum = 0;
	for(LedNum = 0; LedNum < LED_COUNT; LedNum++) {
		if (LedNum == number) WS2812SetRGB(LedNum, 255, 255, 255);
		else WS2812SetRGB(LedNum, 0, 0, 0);
	}
	WS2812Write(colors, LED_COUNT);
}

volatile uint8_t Temp;

void setPWMWhite(uint8_t value)
{
	uint8_t UpBright = 0, DownBright = 255;
	static uint8_t PrevLed = 0, FirstStep = 1;

	for(int i = 0; i < 256; i++) {
		_delay_ms(1);
		WS2812SetRGB(value, UpBright, UpBright, UpBright);
		if (!FirstStep) WS2812SetRGB(PrevLed, DownBright, DownBright, DownBright);
		UpBright += 1;
		DownBright -=1;
		WS2812Write(colors, LED_COUNT);
	}
	PrevLed = value;
	FirstStep = 0;
}

#define DELAY_VALUE	500

#define USER_DELAY	_delay_us(DELAY_VALUE)

void set3StatePWMLight(uint8_t CurLEDNum)
{
	static uint8_t State = 0;
	static uint8_t PrevLEDNumber = 0, PPrevLEDNumber = 0;
	uint8_t LightUP = 0, LightDown = 255, LightCnt = 0;

	switch (State) {
		case 0:
			for(LightCnt = 0; LightCnt < 255; LightCnt++) {
				USER_DELAY;
				WS2812SetRGB(CurLEDNum, LightUP, LightUP, LightUP);
				WS2812Write(colors, LED_COUNT);
				LightUP++;
			}
			PrevLEDNumber = CurLEDNum;
			State = 1;
			break;
		case 1:
			for(LightCnt = 0; LightCnt < 255; LightCnt++) {
				USER_DELAY;
				WS2812SetRGB(CurLEDNum, LightUP, LightUP, LightUP);
				WS2812Write(colors, LED_COUNT);
				LightUP++;
			}
			PPrevLEDNumber = PrevLEDNumber;
			PrevLEDNumber = CurLEDNum;
			State = 2;
			break;
		case 2:
			for(LightCnt = 0; LightCnt < 255; LightCnt++) {
				USER_DELAY;
				WS2812SetRGB(CurLEDNum, LightUP, LightUP, LightUP);
				WS2812SetRGB(PPrevLEDNumber, LightDown, LightDown, LightDown);
				WS2812Write(colors, LED_COUNT);
				LightUP++;
				LightDown--;
			}
			PPrevLEDNumber = PrevLEDNumber;
			PrevLEDNumber = CurLEDNum;
			State = 3;
			break;
		case 3:
			for(LightCnt = 0; LightCnt < 255; LightCnt++) {
				USER_DELAY;
				WS2812SetRGB(CurLEDNum, LightUP, LightUP, LightUP);
				WS2812SetRGB(PPrevLEDNumber, LightDown, LightDown, LightDown);
				WS2812Write(colors, LED_COUNT);
				LightUP++;
				LightDown--;
			}
			PPrevLEDNumber = PrevLEDNumber;
			PrevLEDNumber = CurLEDNum;
			State = 2;
			break;
		default:
			break;
	}
}

ISR(TIMER2_COMP_vect)
{
	TCNT2 = 0x00;
	BUT_Poll();
}

int main() {
	uint8_t Step = 0;
	WS2812Init();
	//BUT_Init();

	while(1) {
		get_but();

		_delay_ms(50);
		set3StatePWMLight(Step);
		//setPWMWhite(Step);
		if (++Step == LED_COUNT) Step = 0;

		/*_delay_ms(10);
		if (++StepCnt == 21) {
			setWhite(Step);
			if (++Step == LED_COUNT) Step = 0;
			StepCnt = 0;
		}*/
	}
}
