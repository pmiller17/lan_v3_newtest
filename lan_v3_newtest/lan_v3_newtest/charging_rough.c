/*
 * charging_rough.c
 *
 * Created: 4/30/2014 8:59:05 PM
 *  Author: mini me
 */ 

#include "lan.h"
#include "adc.h"
#include "charging_rough.h"

charging_mode_t charging_mode = CHARGE_SETUP;
unsigned long int overvoltage_thresh_count;
unsigned int stop_charging;
unsigned long int enough_bulk_current_counter = 0;
unsigned int bulk_charge_reached = 0;
unsigned long int trickle_charge = 0;
unsigned long int not_enough_volts = 0;
unsigned int go_back_bulk = 0;
unsigned int current_offset = 0;

void initialize_charge(void)
{
	overvoltage_thresh_count = 0;
	enough_bulk_current_counter = 0;
	bulk_charge_reached = 0;
			
	ADC_ENABLE;
	ADC_ISR_ENABLE;
	FPWM_CLR_COMP_MATCH;
	TURN_ON_PWM_CLK;
}

void charge_batt(void)
{
	
	static int battery_current = 0;
	static int battery_voltage = 0;
	
	switch(charging_mode)
	{

		case CONSTANT_CURRENT:
		
			if(battery_current < 242)
			{
				OCR1B++;
			}
			else if(battery_current > 245)
			{
				OCR1B--;
			}
		
			if(battery_current >= 242)
			{			
				enough_bulk_current_counter++;
				if(enough_bulk_current_counter > 2000)
				{
					bulk_charge_reached = TRUE;
				}
			}
		
			if((battery_current < 234) & (battery_current > 0))
			{
				enough_bulk_current_counter = 0;
			}
		
			current_offset = battery_current/32;
			battery_voltage = adc_read_vbatt();
			
			if(bulk_charge_reached == TRUE)
			{
				if(battery_voltage - current_offset > 220)
				{
					overvoltage_thresh_count++;
				}
			
				if(battery_voltage - current_offset < 220)
				{
					overvoltage_thresh_count = 0;
				}
			}
		
			if(overvoltage_thresh_count >= 2000)
			{
				charging_mode = CONSTANT_VOLTAGE;
			}
			
		break;

		case CONSTANT_VOLTAGE:
		
			if(battery_voltage - current_offset > 220)
			{
				OCR1B -= 1;
			}
		
			else if(battery_voltage - current_offset < 220)
			{
				OCR1B += 3;
			}
		
			if(battery_voltage - current_offset < 208)
			{
				not_enough_volts++;
				if(not_enough_volts >= 1000)
				{
					go_back_bulk = TRUE;
				}
			}
		
			if(battery_voltage - current_offset > 216)
			{
				not_enough_volts = 0;
			}
		
			if(battery_voltage > 240)
			{
				OCR1B = 160;
			}
		
			if(battery_voltage < 170)
			{
				OCR1B = 130;
			}
		
			if(go_back_bulk)
			{
				charging_mode = CONSTANT_CURRENT;
				go_back_bulk = 0;
			}	
		
		
			case CONST_VOLTAGE_IREAD:
		
		adc_reading_mode = READ_IBATT;
		CFG_ADC_IBATT_GAIN_32;
		
		still_taking_readings = 1;
		while(still_taking_readings)
		{
			ADC_TRIGGER_MEASUREMENT;
		}
		
		current_offset = ibatt_adc_conversion/32;
		
		if(ibatt_adc_conversion < 10)
		{
			trickle_charge++;
		}
		
		if(ibatt_adc_conversion > 30)
		{
			trickle_charge = 0;
		}
		
		if(trickle_charge >= 2000)
		{
			charging_mode = TRICKLE_CHARGE;
		}
		else charging_mode = CONST_VOLTAGE_VREAD;
		
		break;
		
		case TRICKLE_CHARGE:
		adc_reading_mode = READ_VBATT;
		CFG_ADC_VBATT_GAIN_1;
		//		ADC_TRIGGER_MEASUREMENT;	//garbage reading
		//		ADC_TRIGGER_MEASUREMENT;
		
		still_taking_readings = 1;
		while(still_taking_readings)
		{
			ADC_TRIGGER_MEASUREMENT;
		}
		
		if(vbatt_adc_conversion - current_offset > 220)
		{
			OCR1B -= 1;
			
		}
		
		else if(vbatt_adc_conversion - current_offset < 220)
		{
			OCR1B += 3;
			
		}
		
		break;
	}
}