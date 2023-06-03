/*
 * Button.c
 *
 * Created: 6/1/2023 8:41:12 PM
 *  Author: Arafa
 */ 
/************************************************************************************************/
/*									Includes													*/
/************************************************************************************************/

#include "button.h"

/************************************************************************************************/
/*									Function Implementation														*/
/************************************************************************************************/

/**
 * Function: button_initializa
 * ---------------------------
 * Initializes a button.
 *
 * Parameters:
 *   ptr_str_btn: Pointer to the button configuration structure (str_button_t*)
 *
 * Returns:
 *   enu_button_return_state_t: Initialization status
 *     - BUTTON_E_NOK: Null pointer detected
 *     - BUTTON_E_OK: Button initialized successfully
 */
enu_button_return_state_t button_initializa(const str_button_t* ptr_str_btn){
	enu_button_return_state_t ret_state = BUTTON_E_OK;
	
	// Check if the pointer is NULL
	if(ptr_str_btn == NULL){
		ret_state = BUTTON_E_NOK; // Return error code BUTTON_E_NOK if NULL pointer
	}
	else{
		// Initialize the button by configuring the port and pin as input
		DIO_init(ptr_str_btn->port_config, ptr_str_btn->pin_config, STD_INPUT);
	}
	
	return ret_state; // Return the initialization status
}

/**
 * Function: button_read_state
 * ---------------------------
 * Reads the state of a button.
 *
 * Parameters:
 *   ptr_str_btn: Pointer to the button configuration structure (str_button_t*)
 *   ptr_enu_btn_state: Pointer to the button state variable (button_state_t*)
 *
 * Returns:
 *   enu_button_return_state_t: Read state status
 *     - BUTTON_NULL_PTR: Null pointer detected
 *     - BUTTON_E_OK: Button state read successfully
 */
enu_button_return_state_t button_read_state( str_button_t *ptr_str_btn, button_state_t *ptr_enu_btn_state){
	enu_button_return_state_t ret_state = BUTTON_E_OK;
	
	// Check if either pointer is NULL
	if((ptr_str_btn == NULL)||(ptr_enu_btn_state == NULL)){
		ret_state = BUTTON_NULL_PTR; // Return error code BUTTON_NULL_PTR if any pointer is NULL
	}
	else{
		u8_dioLevel_t temp_logic=0;
		
		// Read the current logic level of the button
		DIO_readPIN (ptr_str_btn->port_config, ptr_str_btn->pin_config, &temp_logic);
		
		// Check if the button is pressed based on the active logic level
		if(ptr_str_btn->button_active == temp_logic){
			*ptr_enu_btn_state = BUTTON_PRESSED ;
			ptr_str_btn->button_state = BUTTON_PRESSED ;
		}else{
			*ptr_enu_btn_state = BUTTON_RELEASED ;
			ptr_str_btn->button_state = BUTTON_RELEASED ;
		}
	}
	
	return ret_state; // Return the read state status
}

/**
 * Function: button_with_INT
 * ---------------------------
 * Sets up the button with interrupts.
 *
 * Parameters:
 *   ptr_str_btn: Pointer to the button configuration structure (str_button_t*)
 *   func: Pointer to the callback function to be called on interrupt (void (*)(void))
 *
 * Returns:
 *   enu_button_return_state_t: Setup status
 *     - BUTTON_NULL_PTR: Null pointer detected
 *     - BUTTON_E_NOK: Invalid port or pin configuration
 *     - BUTTON_E_OK: Button set up successfully with interrupts
 */
enu_button_return_state_t button_with_INT( str_button_t *ptr_str_btn, void (*func)(void)){
	enu_button_return_state_t ret_state = BUTTON_E_OK;
	
	// Check if either pointer is NULL
	if((ptr_str_btn == NULL)||(func == NULL)){
		ret_state = BUTTON_NULL_PTR; // Return error code BUTTON_NULL_PTR if any pointer is NULL
	}
	else{
		if(ptr_str_btn->port_config == portd){
			
			// Set up interrupt for pin 2
			if(ptr_str_btn->pin_config == pin2){
				EXT_INTERRUPT_init(EXT_0);
				EXT_INTERRUPT_set_callback_init(EXT_0,func);
			}
			// Set up interrupt for pin 3
			else if(ptr_str_btn->pin_config == pin3){
				EXT_INTERRUPT_init(EXT_1);
				EXT_INTERRUPT_set_callback_init(EXT_1,func);
			}
			else{
				ret_state = BUTTON_E_NOK; // Return error code BUTTON_E_NOK for invalid pin configuration
			}
		}
		else{
			ret_state = BUTTON_E_NOK; // Return error code BUTTON_E_NOK for invalid port configuration
		}
	}
	
	return ret_state; // Return the setup status
}

/**
 * Function: button_enable_INT
 * ---------------------------
 * Enables interrupts for the button.
 *
 * Parameters:
 *   ptr_str_btn: Pointer to the button configuration structure (str_button_t*)
 *
 * Returns:
 *   enu_button_return_state_t: Enable interrupt status
 *     - BUTTON_NULL_PTR: Null pointer detected
 *     - BUTTON_E_NOK: Invalid port or pin configuration
 *     - BUTTON_E_OK: Interrupt enabled successfully for the button
 */
enu_button_return_state_t button_enable_INT( str_button_t *ptr_str_btn){
	enu_button_return_state_t ret_state = BUTTON_E_OK;
	
	// Check if the pointer is NULL
	if(ptr_str_btn == NULL){
		ret_state = BUTTON_NULL_PTR; // Return error code BUTTON_NULL_PTR if NULL pointer
	}
	else{
		if(ptr_str_btn->port_config == portd){
			
			// Enable interrupt for pin 2
			if(ptr_str_btn->pin_config == pin2){
				EXT_INTERRUPT_enable(EXT_0);
			}
			// Enable interrupt for pin 3
			else if(ptr_str_btn->pin_config == pin3){
				EXT_INTERRUPT_enable(EXT_1);
			}
			else{
				ret_state = BUTTON_E_NOK; // Return error code BUTTON_E_NOK for invalid pin configuration
			}
		}
		else{
			ret_state = BUTTON_E_NOK; // Return error code BUTTON_E_NOK for invalid port configuration
		}
	}
	
	return ret_state; // Return the enable interrupt status
}

/**
 * Function: button_disable_INT
 * ---------------------------
 * Disables interrupts for the button.
 *
 * Parameters:
 *   ptr_str_btn: Pointer to the button configuration structure (str_button_t*)
 *
 * Returns:
 *   enu_button_return_state_t: Disable interrupt status
 *     - BUTTON_NULL_PTR: Null pointer detected
 *     - BUTTON_E_NOK: Invalid port or pin configuration
 *     - BUTTON_E_OK: Interrupt disabled successfully for the button
 */
enu_button_return_state_t button_disable_INT( str_button_t *ptr_str_btn){
	enu_button_return_state_t ret_state = BUTTON_E_OK;
	
	// Check if the pointer is NULL
	if(ptr_str_btn == NULL){
		ret_state = BUTTON_NULL_PTR; // Return error code BUTTON_NULL_PTR if NULL pointer
	}
	else{
		if(ptr_str_btn->port_config == portd){
			
			// Disable interrupt for pin 2
			if(ptr_str_btn->pin_config == pin2){
				EXT_INTERRUPT_disable(EXT_0);
			}
			// Disable interrupt for pin 3
			else if(ptr_str_btn->pin_config == pin3){
				EXT_INTERRUPT_disable(EXT_1);
			}
			else{
				ret_state = BUTTON_E_NOK; // Return error code BUTTON_E_NOK for invalid pin configuration
			}
		}
		else{
			ret_state = BUTTON_E_NOK; // Return error code BUTTON_E_NOK for invalid port configuration
		}
	}
	
	return ret_state; // Return the disable interrupt status
}
/************************************************************************************************/
/*									End													*/
/************************************************************************************************/
