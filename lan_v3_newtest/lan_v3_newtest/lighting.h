/*
 * lighting.h
 *
 * Created: 5/8/2014 9:47:21 PM
 *  Author: mini me
 */ 


#ifndef LIGHTING_H_
#define LIGHTING_H_

void cycle_led_mode(void);
void initialize_lighting_mode(void);
void flicker_led(void); // may want to have input be number of times
<<<<<<< HEAD
<<<<<<< HEAD
void led_control(void);
=======
void led_control_current(uint8_t);
void run_lighting_mode(void);
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166
=======
void led_control_current(uint8_t);
void run_lighting_mode(void);
>>>>>>> 8e8623bd8ce1f83e7e9656634e1cbedb82632166

typedef enum
{
	OFF,
	DIM,
	BRIGHT,
	} lighting_mode_t;

#endif /* LIGHTING_H_ */