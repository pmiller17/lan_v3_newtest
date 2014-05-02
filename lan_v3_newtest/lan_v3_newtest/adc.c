/*
 * adc.c
 *
 * Created: 4/9/2014 5:39:29 PM
 *  Author: mini me
 */ 

#include "adc.h"
#include "lan.h"

int adc_read_iled()
{
	CFG_ADC_ILED_GAIN_20;
	ADC_TRIGGER_MEASUREMENT;
//	ADC_TRIGGER_MEASUREMENT;
	
	return ADCH;
}

int adc_read_vbatt()
{
	CFG_ADC_VBATT_GAIN_1;
	ADC_TRIGGER_MEASUREMENT;
	
	return ADCH;
}

int adc_read_ibatt()
{
	CFG_ADC_IBATT_GAIN_32;
	ADC_TRIGGER_MEASUREMENT;
	
	return ADCH;
}