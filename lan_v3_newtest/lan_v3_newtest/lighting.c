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
#define FLICKER_ON_TIME
#define FLICKER_OFF_TIME
#define FLICKER_TARGET_CURRENT

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
	int i; //for wasting time in a loop	
	for(i = 0; i < FLICKER_ON_TIME; i++)
		led_control_current(FLICKER_TARGET_CURRENT)
	
	OCR1B = 0;
	
	for(i = 0; i < FLICKER_OFF_TIME; i++)
		;
	
}

void led_control_current(uint8_t target_current)
{
	
	uint8_t led_current;
	led_current = adc_read_iled();
	if(led_current < target_current)
		OCR1B++;
	else if(led_current > target_current)
		OCR1B--;
	
}

void run_lighting_mode(void)
{
	
	switch(lighting_mode)
	{
		case OFF:
			
			OCR1B = 0;
			FPWM_NORMAL_PORT_OP;
			//add the <avr/sleep.h> code here
		break;
		
		case DIM:
			//add wakeup code here
			FPWM_CLR_COMP_MATCH;
			led_control_current(DIM_CURRENT);
		break;
		
		case BRIGHT:
			led_control_current(BRIGHT_CURRENT);
		break;
	}
}