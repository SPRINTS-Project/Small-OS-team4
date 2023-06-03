/*
 * ext_interrupt.c
 *
 * Created: 6/1/2023 5:25:07 AM
 *  Author: Arafa
 */ 
/************************************************************************************************/
/*									Includes														*/
/************************************************************************************************/
#include "ext_interrupt_pre_linking_cfg.h"
#include "ext_interrupt_private_reg.h"
#include "ext_interrupt.h"


/************************************************************************************************/
/*									Function Implementation														*/
/************************************************************************************************/
/**
 * Variable: EXT_INT_callbacks
 * -------------------_--------
 * Array of function pointers to store callback functions for external interrupts.
 * Initialized with NULL for all entries.
 */
ptr_function_t EXT_INT_callbacks[INVALID_EXT_INT] = {NULL};

/**
 * Function: EXT_INTERRUPT_init
 * ----------------------------
 * Initializes the specified external interrupt.
 *
 * Parameters:
 *   en_ext_interrupt_num: External interrupt number (en_ext_interrupt_num_t)
 *
 * Returns:
 *   u8_interruptError_t: Initialization status
 *     - EXT_INT_E_OK: External interrupt initialized successfully
 *     - EXT_INT_WRONG_EXT_NUM: Invalid external interrupt number
 */
u8_interruptError_t EXT_INTERRUPT_init(en_ext_interrupt_num_t en_ext_interrupt_num){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
	// Check if the external interrupt number is within valid range
	if(en_ext_interrupt_num < INVALID_EXT_INT){
		switch(en_ext_interrupt_num){
			case EXT_0:
				sei();	
				DIO_init (portd, pin2, STD_INPUT);
				MCUCR_ADD |= ( arr_en_gc_edge_detection[EXT_0]<<INT0_MODE_INDEX);
				break;
			case EXT_1:
				sei();
				DIO_init (portd, pin3, STD_INPUT);
				MCUCR_ADD |= ( arr_en_gc_edge_detection[EXT_1]<<INT1_MODE_INDEX);
				break;
			case EXT_2:
				sei();
				DIO_init (portb, pin2, STD_INPUT);
				MCUCSR_ADD |= ( arr_en_gc_edge_detection[EXT_2]<<INT2_MODE_INDEX);
				break;
			default:
				ret_state = EXT_INT_WRONG_EXT_NUM; // Return error code EXT_INT_WRONG_EXT_NUM for invalid external interrupt number
				break;
		}
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM; // Return error code EXT_INT_WRONG_EXT_NUM for invalid external interrupt number
	}
	
	return ret_state; // Return the initialization status
}

/**
 * Function: EXT_INTERRUPT_enable
 * ------------------------------
 * Enables the specified external interrupt.
 *
 * Parameters:
 *   en_ext_interrupt_num: External interrupt number (en_ext_interrupt_num_t)
 *
 * Returns:
 *   u8_interruptError_t: Enable status
 *     - EXT_INT_E_OK: External interrupt enabled successfully
 *     - EXT_INT_WRONG_EXT_NUM: Invalid external interrupt number
 */
u8_interruptError_t EXT_INTERRUPT_enable(en_ext_interrupt_num_t en_ext_interrupt_num){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
	// Check if the external interrupt number is within valid range
	if(en_ext_interrupt_num < INVALID_EXT_INT){
		switch(en_ext_interrupt_num){
			case EXT_0:
				SET_BIT(GICR_ADD,INT0_Globle);
				break;
			case EXT_1:
				SET_BIT(GICR_ADD,INT1_Globle);
				break;
			case EXT_2:
				SET_BIT(GICR_ADD,INT2_Globle);
				break;
			default:
				ret_state = EXT_INT_WRONG_EXT_NUM; // Return error code EXT_INT_WRONG_EXT_NUM for invalid external interrupt number
				break;
		}
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM; // Return error code EXT_INT_WRONG_EXT_NUM for invalid external interrupt number
	}
	
	return ret_state; // Return the enable status
}

/**
 * Function: EXT_INTERRUPT_disable
 * -------------------------------
 * Disables the specified external interrupt.
 *
 * Parameters:
 *   en_ext_interrupt_num: External interrupt number (en_ext_interrupt_num_t)
 *
 * Returns:
 *   u8_interruptError_t: Disable status
 *     - EXT_INT_E_OK: External interrupt disabled successfully
 *     - EXT_INT_WRONG_EXT_NUM: Invalid external interrupt number
 */
u8_interruptError_t EXT_INTERRUPT_disable(en_ext_interrupt_num_t en_ext_interrupt_num){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
	// Check if the external interrupt number is within valid range
	if(en_ext_interrupt_num < INVALID_EXT_INT){
		switch(en_ext_interrupt_num){
			case EXT_0:
				CLEAR_BIT(GICR_ADD,INT0_Globle);
				break;
			case EXT_1:
				CLEAR_BIT(GICR_ADD,INT1_Globle);
				break;
			case EXT_2:
				CLEAR_BIT(GICR_ADD,INT2_Globle);
				break;
			default:
				ret_state = EXT_INT_WRONG_EXT_NUM; // Return error code EXT_INT_WRONG_EXT_NUM for invalid external interrupt number
				break;
		}
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM; // Return error code EXT_INT_WRONG_EXT_NUM for invalid external interrupt number
	}
	
	return ret_state; // Return the disable status
}

/**
 * Function: EXT_INTERRUPT_set_callback_init
 * ----------------------------------------
 * Sets the callback function for the specified external interrupt.
 *
 * Parameters:
 *   en_ext_interrupt_num: External interrupt number (en_ext_interrupt_num_t)
 *   callback: Callback function pointer
 *
 * Returns:
 *   u8_interruptError_t: Set callback status
 *     - EXT_INT_E_OK: Callback set successfully
 *     - EXT_INT_NULL_PTR: Null pointer provided for the callback
 *     - EXT_INT_WRONG_EXT_NUM: Invalid external interrupt number
 */
u8_interruptError_t EXT_INTERRUPT_set_callback_init(en_ext_interrupt_num_t en_ext_interrupt_num, void(*callback)(void)){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
	// Check if the external interrupt number is within valid range
	if(en_ext_interrupt_num < INVALID_EXT_INT){
		if(callback != NULL){
			EXT_INT_callbacks[en_ext_interrupt_num] = callback; // Set the callback function pointer for the specified external interrupt
		}
		else{
			ret_state = EXT_INT_NULL_PTR; // Return error code EXT_INT_NULL_PTR for null pointer provided for the callback
		}
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM; // Return error code EXT_INT_WRONG_EXT_NUM for invalid external interrupt number
	}
	
	return ret_state; // Return the set callback status
}

/**
 * Interrupt Service Routine: EXT_INT0
 * -----------------------------------
 * Executes the callback function for external interrupt 0.
 * If the callback function is not set, it returns immediately.
 */
ISR(EXT_INT0){
	if(EXT_INT_callbacks[EXT_0] != NULL){
		(*EXT_INT_callbacks[EXT_0])(); // Call the callback function for external interrupt 0
	}
	else{
		return; // If callback function is not set, return immediately
	}
}

/**
 * Interrupt Service Routine: EXT_INT1
 * -----------------------------------
 * Executes the callback function for external interrupt 1.
 * If the callback function is not set, it returns immediately.
 */
ISR(EXT_INT1){
	if(EXT_INT_callbacks[EXT_1] != NULL){
		(*EXT_INT_callbacks[EXT_1])(); // Call the callback function for external interrupt 1
	}
	else{
		return; // If callback function is not set, return immediately
	}
}

/**
 * Interrupt Service Routine: EXT_INT2
 * -----------------------------------
 * Executes the callback function for external interrupt 2.
 * If the callback function is not set, it returns immediately.
 */
ISR(EXT_INT2){
	if(EXT_INT_callbacks[EXT_2] != NULL){
		(*EXT_INT_callbacks[EXT_2])(); // Call the callback function for external interrupt 2
	}
	else{
		return; // If callback function is not set, return immediately
	}
}

/************************************************************************************************/
/*									End														*/
/************************************************************************************************/
