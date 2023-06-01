/*
 * ext_interrupt.c
 *
 * Created: 6/1/2023 5:25:07 AM
 *  Author: Arafa
 */ 
#include "ext_interrupt_pre_linking_cfg.h"
#include "ext_interrupt_private_reg.h"
#include "ext_interrupt.h"

ptr_function_t EXT_INT_callbacks[INVALID_EXT_INT] ={NULL};

u8_interruptError_t EXT_INTERRUPT_init(en_ext_interrupt_num_t en_ext_interrupt_num){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
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
				ret_state = EXT_INT_WRONG_EXT_NUM;
				break;
		}
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM;
	}
	return ret_state;
}
u8_interruptError_t EXT_INTERRUPT_enable(en_ext_interrupt_num_t en_ext_interrupt_num){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
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
			ret_state = EXT_INT_WRONG_EXT_NUM;
			break;
		}
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM;
	}
	return ret_state;
	
}
u8_interruptError_t EXT_INTERRUPT_disable(en_ext_interrupt_num_t en_ext_interrupt_num){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
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
			ret_state = EXT_INT_WRONG_EXT_NUM;
			break;
		}
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM;
	}
	return ret_state;
	
}
u8_interruptError_t EXT_INTERRUPT_set_callback_init(en_ext_interrupt_num_t en_ext_interrupt_num ,void(*callback)(void)){
	u8_interruptError_t ret_state = EXT_INT_E_OK;
	
	if(en_ext_interrupt_num < INVALID_EXT_INT){
		if(callback != NULL){
			EXT_INT_callbacks[en_ext_interrupt_num] = callback;
		}
		else{
			ret_state = EXT_INT_NULL_PTR;
		}
		
	}
	else{
		ret_state = EXT_INT_WRONG_EXT_NUM;
	}
	return ret_state;
}


ISR(EXT_INT0){
	if(EXT_INT_callbacks[EXT_0] != NULL){
		(*EXT_INT_callbacks[EXT_0])();
	}
	else{
		return;
	}
	
	
}
ISR(EXT_INT1){
	
	
	if(EXT_INT_callbacks[EXT_1] != NULL){
		(*EXT_INT_callbacks[EXT_1])();
	}
	else{
		return;
	}
	
	
}
ISR(EXT_INT2){
	
	if(EXT_INT_callbacks[EXT_2] != NULL){
		(*EXT_INT_callbacks[EXT_2])();
	}
	else{
		return;
	}
	
}