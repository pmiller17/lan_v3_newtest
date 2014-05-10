/*
 * lighting.c
 * 
 * Created: 5/8/2014 9:36:51 PM
 *  Author: Paul Miller
 *	All functions to do with LED control and flickering
 */ 

#include "lan.h"
#include "adc.h"
#include "lighting.h"

#define DIM_CURRENT
#define BRIGHT_CURRENT
#define BLINK_ON_TIME
#define BLINK_OFF_TIME
#define BLINK_TARGET_CURRENT

lighting_mode_t lighting_mode;

void cycle_led_mode(void)
{
	switch(lighting_mode)
	{
		case OFF:
			lighting_mode = DIM;
			break;
			
		case DIM:
			lighting_mode = BRIGHT;
			break;
			
		case BRIGHT:
			lighting_mode = OFF;
			break;
	}
}
void initialize_lighting_mode(void) // move this one to lan_v3_newtest.c
{
	LED_ENABLE;
	BUTTON_PCI_ENABLE;
	
}
void flicker_led(void) // may want to have input be number of times
{
	volatile int timewaste_count = 0;
	int led_current;
	/for(timewaste_count = 0;)
}
void led_control(void);