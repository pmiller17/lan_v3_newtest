/*lan_v3_switch case hierarchy with generic names*/

#include "lan.h"
#include "lightning.h"
#include "charging.h"
#include "

#define TRUE 1
#define FALSE 0

volatile lantern_mode_t lantern_mode, previous_mode;


int main(void)
{
	setup();
	for(;;)
	{
		loop();
	}
}

void setup(void)
{
	/*insert macros and initializations here*/
}

void loop(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	switch(lantern_op_mode)
=======
	switch(lantern_mode)
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
=======
	switch(lantern_mode)
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
	{
		case LIGHTING:
			if(task.debounce_button)
			{	
				button_pressed = debounce_button();
				task.debounce = FALSE;
			}
			
			if(button_pressed)
			{
<<<<<<< HEAD
<<<<<<< HEAD
				cycle_lighting();
				button_pressed = FALSE;
			}
			
			/*need function to say "light LED"?*/
			lighting_mode_control();	//will sleep if "OFF" and control light otherwise
			
			break;
=======
=======
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
				cycle_led_mode();
				button_pressed = FALSE;
			}
			
			if(task.update_runtime)
			{
				battery_usage = calculate_lantern_usage();
				task.update_runtime = FALSE;
			}
			
			if(battery_usage > BATTERY_USAGE_LIMIT)
				lantern_mode = NEEDS_CHARGE;
			
			/*need function to say "light LED"?*/
			run_lighting_mode();	//will sleep if "OFF" and control light otherwise
			
		break;
<<<<<<< HEAD
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
=======
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
			
		case CHARGING:
			charge_battery();
			
<<<<<<< HEAD
<<<<<<< HEAD
			break;
=======
		break;
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
=======
		break;
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
		
		case NEEDS_CHARGE:
			if(task.debounce_button)
			{
				button_pressed = debounce_button();
			}
			
			if(button_pressed)
			{
				flicker_led();
				button_pressed = FALSE;
			}
<<<<<<< HEAD
<<<<<<< HEAD
			break;
=======
		break;
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
=======
		break;
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
			
		case SAFE_OFF:
			
			if(task.debounce_jack)
			{
				jack_plugged_in = debounce_jack();
				task.debounce_jack = FALSE;
			}
			
			if(jack_plugged_in)
			{
				lantern_mode = CHARGING;
			}
			else
			{
				lantern_mode = previous_mode;
				set_up_noncharging(); 
				/*	re-enable stuff, set light to OFF?
					button & jack PCI enables.
				*/
			}
<<<<<<< HEAD
<<<<<<< HEAD
=======
		break;
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
=======
		break;
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
	}

}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
ISR(TIMER0_OVF_vect)
{
	task.update_runtime = TRUE;
}

<<<<<<< HEAD
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
=======
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
ISR(PCINT_vect)
{
	if(JACK_PLUGGED_IN_NOW)
	{
		LED_DISABLE;
		/*other safety disables*/
		previous_mode = lantern_mode;
		lantern_mode = SAFE_OFF;
		task.debounce_jack = TRUE; 
	}
	
	else if(BUTTON_PRESSED_NOW)
	{
		task.debounce_button = TRUE;
	}
	
	else if(JACK_PLUGGED_IN_NOW == FALSE)
	{
		previous_mode = lantern_mode;
		lantern_mode = SAFE_OFF;
	}
}