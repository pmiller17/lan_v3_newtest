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

#define LIGHTING_MODE 0

#include "lan.h"
#include "adc.h"
#include "debounce.h"
#include <avr/sleep.h>

int target_pwm = 0;
int adc_result = 0;
volatile int button_needs_debounce = FALSE;
int button_state = 0;
volatile int jack_needs_debounce = FALSE;
int jack_state = 0;
int light_on = FALSE;
lantern_op_mode_t lantern_op_mode = LIGHTING;

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
    ADC_ISR_ENABLE;
    
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

    sei();
}
void loop(void) 
{

switch lantern_op_mode
{
	case LIGHTING:
		if(button_needs_debounce)
		{
			button_state = debounce_button();
			button_needs_debounce = FALSE;	
		}
		//target 102
		if(button_state == TRUE)
		{
			if(light_on == TRUE)
			{
				OCR1B = 0;
				light_on = FALSE;
				button_state = FALSE;
			}
			else
			{
				TURN_ON_PWM_CLK;
				FPWM_CLR_COMP_MATCH;
				target_pwm = 81;
				light_on = TRUE;
				button_state = FALSE;
			}
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
}
if(jack_needs_debounce)
{
	jack_plugged_in = debounce_jack();
	jack_needs_debouce = FALSE;
}

if(jack_plugged_in == TRUE)
{
	lantern_op_mode = CHARGING;
}

else if(jack_plugged_in == FALSE)
{
	SETUP_LIGHTING_MODE;
	lantern_op_mode = LIGHTING;
}

while(lantern_op_mode == CHARGING)
{
	charge_battery();
}

#if LIGHTING_MODE	
	if(button_needs_debounce)
	{
		button_state = debounce_button();
		button_needs_debounce = FALSE;
		
	}

	//target 102
	
	if(button_state == TRUE)
	{
		cli();
		if(light_on == TRUE)
		{
			OCR1B = 0;
			light_on = FALSE;
			button_state = FALSE;
			sleep_enable();
			sleep_cpu();
		}
		
		else
		{
			sleep_disable();
			TURN_ON_PWM_CLK;
			FPWM_CLR_COMP_MATCH;
			target_pwm = 81;
			light_on = TRUE;
			button_state = FALSE;
		}
		sei();
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
	sleep_disable();
	if(JACK_PLUGGED_IN_NOW)
	{
		SETUP_SAFE_OFF_MODE;
		jack_needs_debounce = TRUE;
//		lantern_op_mode = SAFE_OFF;
	}
	else if(JACK_PLUGGED_IN_NOW == 0)
	{
//		 
		jack_needs_debounce = TRUE;	
	}
	
	if(BUTTON_PRESSED_NOW)
	{
		
		button_needs_debounce = TRUE;
	}
	
}

ISR(ADC_vect)
{
	;
}