/*
 * led_cfg.h
 *
 * Created: 5/25/2023 10:08:04 PM
 *  Author: Mahmoud Sarhan
 */ 


#ifndef LED_CFG_H_
#define LED_CFG_H_

#include "led.h"




const st_ledConfig_t st_gc_ledConfig[2]={
	{
		.port = portc,
		.pin = pin7
	},
	{
		.port = portc,
		.pin = pin2
	}
};

#define LED_0		st_gc_ledConfig[0]
#define LED_1		st_gc_ledConfig[1]


#endif /* LED_CFG_H_ */