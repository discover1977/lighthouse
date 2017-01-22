/*
 * main.c
 *
 *  Created on: 22 џэт. 2017 у.
 *      Author: gavrilov.iv
 */


#include <avr/io.h>
#include "ws2812b/ws2812.h"
#include <util/delay.h>


void setWhite(uint8_t number)
{
	uint8_t LedNum = 0;
	for(LedNum = 0; LedNum < LED_COUNT; LedNum++) {
		if (LedNum == number) WS2812SetRGB(LedNum, 255, 255, 255);
		else WS2812SetRGB(LedNum, 0, 0, 0);
	}
	WS2812Write(colors, LED_COUNT);
}


int main() {
	uint8_t Step = 0;
	WS2812Init();

	while(1) {
		_delay_ms(210);
		setWhite(Step);
		if (++Step == LED_COUNT) Step = 0;
	}
}
