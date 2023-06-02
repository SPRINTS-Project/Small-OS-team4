/*
 * led.h
 *
 * Created: 5/25/2023 10:07:43 PM
 *  Author: Mahmoud Sarhan
 */ 


#ifndef LED_H_
#define LED_H_

#include "../../STD_LIB/std_types.h"


typedef uint8_t u8_ledError_t;

#define LED_ERROR_OK			((u8_ledError_t)0x00)
#define LED_ERROR_NOT_OK		((u8_ledError_t)0x05)

typedef struct
{
	en_dioPorts_t port;
	en_dioPins_t pin;
}st_ledConfig_t;


/************************************************************************/
/* LED_init(st_ledConfig_t led):

	This function initializes an LED based on the provided LED configuration.
	Parameters:
	led: The LED configuration structure that specifies the port and pin of the LED.
	Returns:
	LED_ERROR_OK if the initialization is successful.
	LED_ERROR_NOT_OK if the initialization fails.                                                                   */
/************************************************************************/
u8_ledError_t LED_init(st_ledConfig_t led);

/************************************************************************/
/* 
 LED_on(st_ledConfig_t led):

	 This function turns on the specified LED.
	 Parameters:
	 led: The LED configuration structure that specifies the port and pin of the LED.
	 Returns:
	 LED_ERROR_OK if the operation is successful.
	 LED_ERROR_NOT_OK if the operation fails.                                                                    */
/************************************************************************/
u8_ledError_t LED_on(st_ledConfig_t led);

/************************************************************************/
/* LED_off(st_ledConfig_t led):

	This function turns off the specified LED.
	Parameters:
	led: The LED configuration structure that specifies the port and pin of the LED.
	Returns:
	LED_ERROR_OK if the operation is successful.
	LED_ERROR_NOT_OK if the operation fails.                                                                     */
/************************************************************************/
u8_ledError_t LED_off(st_ledConfig_t led);
/************************************************************************/
/* LED_toggle(st_ledConfig_t led):

	This function toggles the state of the specified LED.
	Parameters:
	led: The LED configuration structure that specifies the port and pin of the LED.
	Returns:
	LED_ERROR_OK if the operation is successful.
	LED_ERROR_NOT_OK if the operation fails.                                                                     */
/************************************************************************/
u8_ledError_t LED_toggle(st_ledConfig_t led);

#endif /* LED_H_ */