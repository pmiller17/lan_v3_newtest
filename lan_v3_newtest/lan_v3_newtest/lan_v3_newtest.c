/*
 * lan_v3_newtest.c
 *
 * Created: 4/5/2014 8:01:59 PM
 *  Author: mini me
 */ 

/*
#include <avr/io.h>
#include <avr/interrupt.h>
*/


//
#define LIGHTING_MODE 0
#define RUN_CHARGING 1
#define JACK_DEBOUNCE 1
#define TEST_IBATT 0

#include "lan.h"
#include "adc.h"
#include "debounce.h"
#include "charging_rough.h"
#include <avr/sleep.h>
#include "lighting.h"

int target_pwm = 0;
int adc_result = 0;
volatile int button_needs_debounce = FALSE;
int button_state = 0;
volatile int jack_needs_debounce = FALSE;
volatile int jack_state = FALSE;
int light_on = FALSE;
volatile lantern_mode_t lantern_mode = LIGHTING, previous_mode;
int jack_pin = 0;
volatile unsigned int battery_voltage;
volatile unsigned int battery_current;
uint8_t pwm_value;

int main(void)
{
    setup();
	for (;;) 
	{
		loop();
	}	
	return 0;
}


void setup(void)
{
	cli();

    int i;
	for(i = 0; i <= 7; i++)
	{
		CLRBIT(PCMSK0,i);
		CLRBIT(PCMSK1,i);
	}
  
    /*  Enable the PLL.  */
    ENABLE_PLL;
    while(PLL_IS_NOT_LOCKED)
    {
        // Waiting for PLOCK bit to be set
    }
    ENABLE_PCK;

    /*  Use a prescaler of 16 to set the system clock at 500 kHz (1 MHz?).
    */
    SET_SYS_CLK;

    /*  IO Configurations
    */
	CFG_IO_BUTTON;		
	CFG_IO_LED_ENABLE;
	CFG_IO_JACK;	
    CFG_IO_PWM;			
    
    /*  Clock 1 (PWM clock) Configurations
        Use PLL to set clock 1 at 64 MHz, asynchronous from the system clock.
        Use a prescaler of 4 and a TOP value of 256 to have FPWM frequency at 62.5 kHz.
    */
    CFG_PWM;
    FPWM_NORMAL_PORT_OP;
    TURN_OFF_PWM_CLK;
	OCR1C = 255;
    OCR1B = 0;
		
    CFG_ADC;

    ADC_ENABLE;
//	ADC_ISR_ENABLE;
    
    LED_ENABLE;

	PCI_ENABLE;
	BUTTON_PCI_ENABLE;
	JACK_PCI_ENABLE;
	
	FPWM_CLR_COMP_MATCH;
	TURN_ON_PWM_CLK;
#if 0	
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
#endif
	
	battery_current = 0;
	battery_voltage = 0;
	pwm_value = 0;
    sei();
}
void loop(void) 
{
	
	
#if TEST_IBATT

	OCR1B = pwm_value;
	battery_current = adc_read_ibatt();

#endif

	

#if JACK_DEBOUNCE


if(jack_needs_debounce)
{
	jack_state = debounce_jack();
	jack_needs_debounce = FALSE;
}

if(jack_state == TRUE)
{
	lantern_mode = CHARGING;
	initialize_charging_mode();
}

else if(jack_state == FALSE)
{
	lantern_mode = LIGHTING;
}
#endif

#if RUN_CHARGING
while(lantern_mode == CHARGING)
{
	charge_battery();
	pwm_value = OCR1B;
}
#endif

#if LIGHTING_MODE	
	if(button_needs_debounce)
	{
		button_state = debounce_button();
		button_needs_debounce = FALSE;
		
	}

	//target 102
	
	if(button_state == TRUE)
	{
		cycle_led();
	}
	
	adc_result = adc_read_iled();
	
	if(light_on == TRUE)
	{
		
		if(adc_result < target_pwm)
		{
			OCR1B++;
		}
		else if(adc_result > target_pwm)
		{
			OCR1B--;
		}
	}
#endif
}

ISR(PCINT_vect)
{

	if(JACK_PLUGGED_IN_NOW)
	{
		LED_DISABLE;
		BUTTON_PCI_DISABLE;
		jack_needs_debounce = TRUE;
//		lantern_op_mode = SAFE_OFF;
	}
	else if(JACK_PLUGGED_IN_NOW == 0)
	{
//		 
		jack_needs_debounce = TRUE;	
	}
	
}