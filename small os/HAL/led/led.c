/*
 * led.c
 *
 * Created: 6/1/2023 3:10:51 PM
 *  Author: Youssef Abbas
 */ 

#include "./led.h"
//#include "../../MCAL/dio/dio.h"

/**
 * @brief Initializes an LED based on the provided configuration.
 *
 * @param led The LED configuration structure.
 *
 * @return LED_ERROR_OK if the initialization is successful, otherwise LED_ERROR_NOT_OK.
 */
u8_ledError_t LED_init(st_ledConfig_t led){
	if (DIO_init(led.port,led.pin,STD_OUTPUT) == DIO_E_OK)
	{
		return LED_ERROR_OK;
	}
	return LED_ERROR_NOT_OK;
}/**
 * @brief Turns on the specified LED.
 *
 * @param led The LED configuration structure.
 *
 * @return LED_ERROR_OK if the operation is successful, otherwise LED_ERROR_NOT_OK.
 */
u8_ledError_t LED_on(st_ledConfig_t led){
	if (DIO_writePIN(led.port,led.pin,STD_HIGH) == DIO_E_OK)
	{
		return LED_ERROR_OK;
	}
	return LED_ERROR_NOT_OK;
}
/**
 * @brief Turns off the specified LED.
 *
 * @param led The LED configuration structure.
 *
 * @return LED_ERROR_OK if the operation is successful, otherwise LED_ERROR_NOT_OK.
 */
u8_ledError_t LED_off(st_ledConfig_t led){
	if (DIO_writePIN(led.port,led.pin,STD_HIGH) == DIO_E_OK)
	{
		return LED_ERROR_OK;
	}
	return LED_ERROR_NOT_OK;
}
/**
 * @brief Toggles the state of the specified LED.
 *
 * @param led The LED configuration structure.
 *
 * @return LED_ERROR_OK if the operation is successful, otherwise LED_ERROR_NOT_OK.
 */
u8_ledError_t LED_toggle(st_ledConfig_t led){
	if (DIO_togglePIN(led.port,led.pin) == DIO_E_OK)
	{
		return LED_ERROR_OK;
	}
	return LED_ERROR_NOT_OK;
}
