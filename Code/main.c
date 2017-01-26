/*
 * main.c
 *
 *  Created on: 22 џэт. 2017 у.
 *      Author: gavrilov.iv
 */

#include <avr/io.h>
#include "ws2812b/ws2812.h"
#include <util/delay.h>

#define BRIGHTNESS_COUNT	50
#define DELAY_VALUE			10
#define USER_DELAY			_delay_ms(DELAY_VALUE)

void set3StatePWMLight(uint8_t CurLEDNum)
{
	static uint8_t State = 0;
	static uint8_t PrevLEDNumber = 0, PPrevLEDNumber = 0;
	int8_t LightUP = 0, LightDown = (BRIGHTNESS_COUNT);
	uint8_t LightCnt = 0;

	switch (State) {
		case 0:
			for(LightCnt = 0; LightCnt < BRIGHTNESS_COUNT; LightCnt++) {
				USER_DELAY;
				WS2812SetRGB(CurLEDNum, LightUP, LightUP, LightUP);
				WS2812Write(colors, LED_COUNT);
				LightUP++;
			}
			PrevLEDNumber = CurLEDNum;
			State = 1;
			break;
		case 1:
			for(LightCnt = 0; LightCnt < BRIGHTNESS_COUNT; LightCnt++) {
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
			for(LightCnt = 0; LightCnt < BRIGHTNESS_COUNT; LightCnt++) {
				USER_DELAY;
				WS2812SetRGB(CurLEDNum, LightUP, LightUP, LightUP);
				WS2812SetRGB(PPrevLEDNumber, LightDown, LightDown, LightDown);
				WS2812Write(colors, LED_COUNT);
				LightUP++;
				if (LightCnt % 2 == 0) LightDown -= 2;
			}
			PPrevLEDNumber = PrevLEDNumber;
			PrevLEDNumber = CurLEDNum;
			State = 3;
			break;
		case 3:
			for(LightCnt = 0; LightCnt < BRIGHTNESS_COUNT; LightCnt++) {
				USER_DELAY;
				WS2812SetRGB(CurLEDNum, LightUP, LightUP, LightUP);
				WS2812SetRGB(PPrevLEDNumber, LightDown, LightDown, LightDown);
				WS2812Write(colors, LED_COUNT);
				LightUP++;
				if (LightCnt % 2 == 0) LightDown -= 2;
			}
			PPrevLEDNumber = PrevLEDNumber;
			PrevLEDNumber = CurLEDNum;
			State = 2;
			break;
		default:
			break;
	}
}

int main() {
	uint8_t Step = 0;
	WS2812Init();

	while(1) {
		_delay_ms(1);
		set3StatePWMLight(Step);
		if (++Step == LED_COUNT) Step = 0;

	}
}
