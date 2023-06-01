/*
 * ext_interrupt_private_reg.h
 *
 * Created: 5/30/2023 12:10:06 AM
 *  Author: Mahmoud Sarhan
 */ 


#ifndef EXT_INTERRUPT_PRIVATE_REG_H_
#define EXT_INTERRUPT_PRIVATE_REG_H_
#include "../../STD_LIB/std_types.h"

#define GICR_ADD			 (*((volatile uint8_t *) 0x5B))
#define INT0_Globle		6
#define INT1_Globle		7
#define INT2_Globle		5

#define MCUCR_ADD			(*((volatile uint8_t *) 0x55))
#define INT0_MODE_INDEX	0
#define INT1_MODE_INDEX	2

#define MCUCSR_ADD			(*((volatile uint8_t *) 0x54))
#define INT2_MODE_INDEX	6

#define SREG_ADD			(*((volatile uint8_t *) 0x5F))
#define Globle_INT		7


#endif /* EXT_INTERRUPT_PRIVATE_REG_H_ */