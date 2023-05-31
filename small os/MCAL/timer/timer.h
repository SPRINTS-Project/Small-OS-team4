/*
 * TIMER_Interface.h
 *
 *  Created on: Apr 3, 2023
 *      Author: Bassel Yasser
 */

#ifndef MCAL_TIMER_TIMER_INTERFACE_H_
#define MCAL_TIMER_TIMER_INTERFACE_H_


/************************************************************************************************/
/*									MACROS														*/
/************************************************************************************************/


/************************************************************************************************/
/*									USER_DEFINED TYPES											*/
/************************************************************************************************/
typedef enum
{
	TIMER_OK,
	TIMER_NOK,

}enu_timerStatus_t;


/********************************/
/*	 TIMER 2 					*/
/********************************/
typedef enum
{
	OVF_MODE,
	PHASE_CORRECT_PWM_MODE,
	CTC_MODE,
	FAST_PWM_MODE,

	TIMER_MODE_INVALID,

}enu_timerMode_t;



typedef enum
{
	TIMER_NO_CLK_SRC,
	TIMER_PRE_1,
	TIMER_PRE_8,
	TIMER_PRE_64,
	TIMER_PRE_256,
	TIMER_PRE_1024,
	TIMER_EXT_CLK_FALLING,
	TIMER_EXT_CLK_RISING,

	TIMER_PRESCALR_INVALID,


}enu_timerPrescalar_t;



typedef enum
{
	CTC_NORMAL=0,
	CTC_TOGGLE_ON_CMP,
	CTC_CLR_ON_CMP,
	CTC_SET_ON_CMP,
	CTC_INVALID,

}enu_ctcMode_t;

typedef enum
{
	PWM_NORMAL=0,
	PWM_CLR_ON_CMP,
	PWM_SET_ON_CMP,
	PWM_INVALID,

}enu_pwmMode_t;



typedef void (*ptrFunc)(void);



/************************************************************************************************/
/*									FUNCTIONS PROTOTYPE											*/
/************************************************************************************************/

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_enuInit
 * description 	: Timer Initialization
 * input param 	: enTimerMode { OVF_MODE, PHASE_CORRECT_PWM_MODE, CTC_MODE, FAST_PWM_MODE}
 * return		: enu_timerStatus_t {TIMER_OK, TIMER_NOK}
 * */
enu_timerStatus_t TIMER2_enuInit (enu_timerMode_t enTimerMode);

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_enuSetPrescallar
 * description 	: Timer Initialization
 * input param 	: Copy_enPrescal { 	TIMER_NO_CLK_SRC,
									TIMER_PRE_1,
									TIMER_PRE_8,
									TIMER_PRE_64,
									TIMER_PRE_256,
									TIMER_PRE_1024,
									TIMER_EXT_CLK_FALLING,
									TIMER_EXT_CLK_RISING,}
 * return		: enu_timerStatus_t {TIMER_OK, TIMER_NOK}
 * */
enu_timerStatus_t TIMER2_enuSetPrescallar(enu_timerPrescalar_t Copy_enPrescal);

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_enuOVFIrqEnable
 * description 	: Timer2 Interrupt Enable
 * input param 	: void
 * return		: enu_timerStatus_t {TIMER_OK, TIMER_NOK}
 * */
enu_timerStatus_t TIMER2_enuOVFIrqEnable(void);

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_enuOVFIrqDisable
 * description 	: Timer2 Interrupt Disable
 * input param 	: void
 * return		: enu_timerStatus_t {TIMER_OK, TIMER_NOK}
 * */
enu_timerStatus_t TIMER2_enuOVFIrqDisable(void);

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_enuStart
 * description 	: Timer2 Start Counting
 * input param 	: void
 * return		: enu_timerStatus_t {TIMER_OK, TIMER_NOK}
 * */
enu_timerStatus_t TIMER2_enuStart(void);

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_vidStop
 * description 	: Timer2 Stop
 * input param 	: void
 * return		: enu_timerStatus_t {TIMER_OK, TIMER_NOK}
 * */
enu_timerStatus_t TIMER2_vidStop(void);

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_enuSetTime_ms
 * description 	: Set time in ms
 * input param 	: u32_time_ms
 * return		: enu_timerStatus_t {TIMER_OK, TIMER_NOK}
 * */
enu_timerStatus_t TIMER2_enuSetTime_ms(uint32_t u32_time_ms);

/*
 * Author		: Bassel Yasser Mahmoud
 * function		: TIMER2_vidSetcbf_OVF
 * description 	: Take pointer to function to be executed in ISR when it fires
 * input param 	: cbf  : call back function
 * return		: void
 * */
void TIMER2_vidSetcbf_OVF(ptrFunc cbf);



/***********************************************************************/



#endif /* MCAL_TIMER_TIMER_INTERFACE_H_ */
