/*
 * button.h
 *
 * Created: 6/1/2023 8:41:31 PM
 *  Author: Arafa
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

/* section : includes*/
#include "../../MCAL/DIO/DIO.h"
#include "../../MCAL/ext_interrupt/ext_interrupt.h"


/* section : Macro Declaration*/


/* section : Macro Functions Declarations*/

/* section : Data Types Declarations*/


/**
 * Enumeration: enu_button_return_state_t
 * --------------------------------------
 * Represents the return states of button-related functions.
 * 
 * Values:
 *   - BUTTON_E_NOK: Button error, something went wrong.
 *   - BUTTON_E_OK: Button operation completed successfully.
 *   - BUTTON_WRONG_EXT_NUM: Invalid external interrupt number for button operation.
 *   - BUTTON_NULL_PTR: Null pointer provided for button-related function.
 */
typedef enum{
	BUTTON_E_OK = 0,
	BUTTON_E_NOK,
	BUTTON_WRONG_EXT_NUM,
	BUTTON_NULL_PTR
} enu_button_return_state_t;

/**
 * Enumeration: button_state_t
 * ---------------------------
 * Represents the states of a button.
 * 
 * Values:
 *   - BUTTON_RELEASED: Button is in the released state.
 *   - BUTTON_PRESSED: Button is in the pressed state.
 */
typedef enum {
	BUTTON_RELEASED = 0,
	BUTTON_PRESSED
} button_state_t;

/**
 * Enumeration: button_active_t
 * ----------------------------
 * Represents the active level of a button.
 * 
 * Values:
 *   - BUTTON_ACTIVE_LOW: Button is active low (low level indicates button press).
 *   - BUTTON_ACTIVE_HIGH: Button is active high (high level indicates button press).
 */
typedef enum {
	BUTTON_ACTIVE_LOW = 0,
	BUTTON_ACTIVE_HIGH
} button_active_t;

/**
 * Struct: str_button_t
 * --------------------
 * Represents a button configuration.
 * 
 * Members:
 *   - port_config: The port configuration of the button (en_dioPorts_t).
 *   - pin_config: The pin configuration of the button (en_dioPins_t).
 *   - button_state: The current state of the button (button_state_t).
 *   - button_active: The active level of the button (button_active_t).
 */
typedef struct {
	en_dioPorts_t port_config;
	en_dioPins_t pin_config;
	button_state_t button_state;
	button_active_t button_active;
} str_button_t;



/* section : Function Declaration*/

enu_button_return_state_t button_initializa(const str_button_t* ptr_str_btn);
enu_button_return_state_t button_read_state( str_button_t *ptr_str_btn, button_state_t *ptr_enu_btn_state);
enu_button_return_state_t button_with_INT( str_button_t *ptr_str_btn , void (*func)(void));
enu_button_return_state_t button_enable_INT( str_button_t *ptr_str_btn);
enu_button_return_state_t button_disable_INT( str_button_t *ptr_str_btn);





#endif /* BUTTON_H_ */