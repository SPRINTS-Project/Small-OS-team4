/*
 * ext_interrupt_types.h
 *
 * Created: 5/30/2023 12:07:44 AM
 *  Author: Arafa Arafa
 */ 

/************************************************************************************************/
/*									Guard														*/
/************************************************************************************************/
#ifndef EXT_INTERRUPT_TYPES_H_
#define EXT_INTERRUPT_TYPES_H_
/************************************************************************************************/
/*									Includes														*/
/************************************************************************************************/
#include "../../STD_LIB/std_types.h"
/************************************************************************************************/
/*									User defined Data type														*/
/************************************************************************************************/
typedef void(*ptr_function_t)(void);
/**datatype to hold the state of function and it has two options  INT_E_OK || INT_E_NOK**/
typedef enum{
	EXT_INT_E_OK =0,
	EXT_INT_E_NOK,
	EXT_INT_WRONG_EXT_NUM,
	EXT_INT_NULL_PTR
} enu_ext_int_return_state_t;
typedef uint8_t u8_interruptError_t;


/* data type  of enum has three choices of interrupts 
	external interrupt 0, external interrupt 1, external interrupt 2
*/
typedef enum{
    EXT_0=0,
    EXT_1,
    EXT_2,
	INVALID_EXT_INT
}en_ext_interrupt_num_t;


/* data type  of enum has four options of interrupt detection  
	low level, logical change, falling edge, rising edge
*/
typedef enum{
    LOW_LEVEL,
	LOGICAL_CHANGE,
	FALLING_EDGE,
	RISING_EDGE,
	INVALID_EDGE
}en_edge_detection_t;

#endif /* EXT_INTERRUPT_TYPES_H_ */

/************************************************************************************************/
/*									End														*/
/************************************************************************************************/