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

#include "lan.h"
#include "adc.h"
#include "debounce.h"


int target_pwm = 0;
int adc_result = 0;
volatile int needs_debounce = FALSE;
int button_state = 0;
int light_on = FALSE;
static int glitch_counter = 0;

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
	
	FPWM_CLR_COMP_MATCH;
	TURN_ON_PWM_CLK;
	
    sei();
}
void loop(void) 
{


	
	if(needs_debounce)
	{
		button_state = debounce_button();
		needs_debounce = FALSE;
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
			target_pwm = 61;
			light_on = TRUE;
			button_state = FALSE;
		}
	}
	if(light_on == TRUE)
	{
		adc_result = adc_read_iled();
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

ISR(PCINT_vect)
{

	if(BUTTON_PRESSED_NOW)
	{
		needs_debounce = TRUE;
	}
	
}

ISR(ADC_vect)
{
	;
}