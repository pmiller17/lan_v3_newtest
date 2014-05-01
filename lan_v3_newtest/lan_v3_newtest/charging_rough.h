/*
 * charging_rough.h
 *
 * Created: 4/30/2014 8:59:23 PM
 *  Author: mini me
 */ 


#ifndef CHARGING_ROUGH_H_
#define CHARGING_ROUGH_H_

#include "lan.h"

#define OVERVOLTAGE
#define BULK_CURRENT
#define TAPER_CURRENT


void charge_battery(void);

typedef enum
{
	SETUP,
	CONSTANT_CURRENT,
	CONSTANT_VOLTAGE,
	DONE_CHARGING
	} charging_mode_t;



#endif /* CHARGING_ROUGH_H_ */