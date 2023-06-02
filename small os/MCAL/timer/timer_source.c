#include "../../STD_LIB/std_types.h"
#include "../../STD_LIB/bit_math.h"
#include "../../STD_LIB/interrupt.h"
#include "timer_private_reg.h"
#include "timer.h"

/*************************************************************************************************************
 * 								Macros
 ************************************************************************************************************/
#define PRESCALLER_CLR_MSK		(0b11111000)
#define PRESCALLER_OFFSET		CS00
#define F_CPU			8000000UL

#define STOPPED			0
#define RUNNING			1

#define OVF_TICKS		256UL
#define MAX_TIM_MS		5000UL
/*************************************************************************************************************
 * 								Global/Static Variables
 ************************************************************************************************************/
static enu_timerPrescalar_t gl_enu_prescaller;
static uint8_t gl_u8_timerStatus;
static uint8_t gl_u8_ovf = 0;
static uint8_t gl_u8_preload = 0;
static ptrFunc  gl_CallFunc = NULL;


static uint16_t prescaller_map[6] = {
		0,
		1,
		8,
		32,
		64,
		128,
		256,
		1024
};

/*************************************************************************************************************
 * 							Function Implementation
 ************************************************************************************************************/
enu_timerStatus_t TIMER2_enuInit (enu_timerMode_t enTimerMode)
{
	enu_timerStatus_t errorStatus = TIMER_OK;

	if(enTimerMode < TIMER_MODE_INVALID)
	{
		TCNT2 = 0;
		switch (enTimerMode)
		{
		case OVF_MODE 				: CLEAR_BIT(TCCR2, WGM01);CLEAR_BIT(TCCR2, WGM00); break;
		case PHASE_CORRECT_PWM_MODE : CLEAR_BIT(TCCR2, WGM01);SET_BIT(TCCR2, WGM00); break;
		case CTC_MODE 				: SET_BIT(TCCR2, WGM01);CLEAR_BIT(TCCR2, WGM00); break;
		case FAST_PWM_MODE 			: SET_BIT(TCCR2, WGM01);SET_BIT(TCCR2, WGM00); break;
		default 					: errorStatus = TIMER_NOK; break;
		}
	}
	else
	{
		errorStatus = TIMER_NOK;
	}
	return errorStatus;
}


/*************************************************************************************************************/
enu_timerStatus_t TIMER2_enuSetPrescallar(enu_timerPrescalar_t Copy_enPrescal)
{
	enu_timerStatus_t errorStatus = TIMER_OK;
	uint8_t temp_reg;
	if(Copy_enPrescal < TIMER_PRESCALR_INVALID)
	{
		gl_enu_prescaller = Copy_enPrescal;
		if (gl_u8_timerStatus == RUNNING)
		{
			temp_reg = TCCR2;		 // read
			temp_reg &= PRESCALLER_CLR_MSK;  // mod
			temp_reg |=  gl_enu_prescaller << PRESCALLER_OFFSET; // mod
			TCCR2 |= temp_reg;		// write
		}
		else
		{
			/* do nothing */
		}
	}
	else
	{
		errorStatus = TIMER_NOK;
	}


	return errorStatus;

}

/*************************************************************************************************************/
enu_timerStatus_t TIMER2_enuOVFIrqEnable(void)
{
	enu_timerStatus_t errorStatus = TIMER_OK;

	SET_BIT(TIFR, 6);
	SET_BIT(TIMSK, TOIE2);
//	SET_BIT(SREG_REG, 7);

	return errorStatus;
}

/*************************************************************************************************************/
enu_timerStatus_t TIMER2_enuOVFIrqDisable(void)
{
	enu_timerStatus_t errorStatus = TIMER_OK;

	CLEAR_BIT(TIMSK, TOIE2);

	return errorStatus;
}

/*************************************************************************************************************/
enu_timerStatus_t TIMER2_enuStart(void)
{
	enu_timerStatus_t errorStatus = TIMER_OK;

	TCCR2 |=  gl_enu_prescaller << PRESCALLER_OFFSET;
	gl_u8_timerStatus  = RUNNING;

	return errorStatus;
}

/*************************************************************************************************************/
enu_timerStatus_t TIMER2_vidStop(void)
{
	enu_timerStatus_t errorStatus = TIMER_OK;

	TCCR2 &=  PRESCALLER_CLR_MSK;
	gl_u8_timerStatus = STOPPED;

	return errorStatus;
}
/*************************************************************************************************************/
enu_timerStatus_t TIMER2_enuSetTime_ms(uint8_t u32_time_ms)
{
	enu_timerStatus_t errorStatus = TIMER_OK;
	uint8_t desired_ticks;
	float32_t tick_time_ms;
	if( u32_time_ms < MAX_TIM_MS )
	{
		tick_time_ms = ((float32_t)prescaller_map[gl_enu_prescaller]) / ((uint32_t)F_CPU / 1000U)  ;
		desired_ticks = u32_time_ms / tick_time_ms;

		// Compare ticks with OVF_ticks 
		if(desired_ticks == OVF_TICKS)
		{
			gl_u8_ovf = 1;
		}
		else if(desired_ticks < OVF_TICKS)
		{
			gl_u8_preload = OVF_TICKS - desired_ticks;
			TCNT2 = gl_u8_preload;
			gl_u8_ovf = 1;
		}
		else if(desired_ticks > OVF_TICKS)
		{
			gl_u8_ovf = (uint8_t)(desired_ticks / OVF_TICKS);
			gl_u8_preload = (uint16_t)(OVF_TICKS - (desired_ticks % OVF_TICKS ));
			TCNT2 = gl_u8_preload;
			if (gl_u8_preload > 0U)
			{
				gl_u8_ovf++;
			}
		}
	}
		else
		{
			errorStatus = TIMER_NOK;
		}


		return errorStatus;
	}

/*************************************************************************************************************/
void TIMER2_vidSetcbf_OVF(ptrFunc cbf)
{
	if (cbf != NULL)
	{
		gl_CallFunc = cbf;
	}
	return;
}

/*************************************************************************************************************
 * 							ISR Interrupt Handling
 ************************************************************************************************************/
ISR(TIMER2_OVF)
{
	if (gl_CallFunc != NULL)
	{
		TCNT2 = gl_u8_preload;
		gl_CallFunc();
	}
	//static uint8_t counter = 0;
	//counter++;
	/*if(counter == gl_u8_ovf)
	{
		if (gl_CallFunc != NULL)
		{
			counter = 0;
			TCNT2 = gl_u8_preload;
			gl_CallFunc();	
			
		}
	}*/
}
