/*
 * app.c
 *
 * Created: 6/1/2023 5:17:33 PM
 *  Author: Mahmoud Sarhan
 */ 

/************************************************************************************************/
/*									Includes														*/
/************************************************************************************************/

#include "app.h"
#include "../SERV/sos/sos.h"
#include "../HAL/led/led_cfg.h"
#include "../STD_LIB/interrupt.h"
//#include "../SERV/ext_interrupt_manager/ext_interrupt_manager.h"
#include "../HAL/button/button.h"
 
//#include "../HAL/button/button.h"
/************************************************************************************************/
/*									MACROS														*/
/************************************************************************************************/

#define NOT_INIT	0
#define INIT		1
#define DISABLE		2
/************************************************************************************************/
/*									Initialization														*/
/************************************************************************************************/

static str_button_t st_gs_button_0 = {
	.port_config = portd,
	.pin_config = pin2,
	.button_state = BUTTON_RELEASED,
	.button_active = BUTTON_ACTIVE_LOW
};

static str_button_t st_gs_button_1 = {
	.port_config = portd,
	.pin_config = pin3,
	.button_state = BUTTON_RELEASED,
	.button_active = BUTTON_ACTIVE_LOW
};
/************************************************************************************************/
/*									User defined datat type														*/
/************************************************************************************************/

typedef void (* APP_runing_task_t) (void);
/************************************************************************************************/
/*									Static and Global Variables													*/
/************************************************************************************************/

static APP_runing_task_t APP_runing_task = SOS_run;

/************************************************************************************************/
/*									Static functions Prototype														*/
/************************************************************************************************/

void APP_ext_int0_cbf(void);
void APP_wake_up_cbf(void);

/************************************************************************************************/
/*									Tasks Function														*/
/************************************************************************************************/

void TASK_1(void)
{
	LED_toggle(LED_0);
}

void TASK_2(void)
{
	LED_toggle(LED_1);
}

void TASK_3(void)
{
	//SOS_deinit();
	SOS_delete_task(2);
	LED_off(LED_1);
}
/************************************************************************************************/
/*									Function Implementation														*/
/************************************************************************************************/

uint8_t APP_init(void)
{
	uint8_t u8_retVal = APP_ERROR_OK;
	
	// Init leds
	if ((LED_ERROR_OK == LED_init(LED_0)) && 
		(LED_ERROR_OK == LED_init(LED_1)))
	{
		// Init the small SOS
		if (SOS_STATUS_SUCCESS == SOS_init())
		{
			// create task 1
			if (SOS_STATUS_SUCCESS == SOS_create_task(
														TASK_1,					  	    // Function name
															 1,							// ID
															 1,							// priority
															300							// period
													 )															&&
				// create task 2
				SOS_STATUS_SUCCESS == SOS_create_task(
														TASK_2,					  	    // Function name
															 2,							// ID
															 3,							// priority
															500							// period
													))
			{
				// Init external interrupts
				if ((BUTTON_E_OK ==  button_with_INT(&st_gs_button_0,APP_ext_int0_cbf)) &&
				    (BUTTON_E_OK ==  button_with_INT(&st_gs_button_1,APP_wake_up_cbf)))
				{
					// enable global interrupts
					sei();
					
					if ((BUTTON_E_OK == button_enable_INT(&st_gs_button_0)))
					{
						// finished Initialization
					}
					else
					{
						u8_retVal = APP_ERROR_NOT_OK;
					}
				}
				else
				{
					u8_retVal = APP_ERROR_NOT_OK;
				}
			}
			else
			{
				u8_retVal = APP_ERROR_NOT_OK;
			}
		}
		else
		{
			u8_retVal = APP_ERROR_NOT_OK;
		}
		
	}
	else
	{
		u8_retVal = APP_ERROR_NOT_OK;
	}
	return u8_retVal;
}

void APP_start(void)
{
	while(1)
	{
		(APP_runing_task)();
	}
}



/************************************************************************************************/
/*									Static Function Implementation														*/
/************************************************************************************************/

// to avoid disabling interrupts in the ISR 
// disable start external interrupt
// enable stop external interrupt
void APP_btn_start_pressed(void)
{
	button_enable_INT(&st_gs_button_0);
	button_disable_INT(&st_gs_button_1);
}


// to avoid disabling interrupts in the ISR 
// disable stop external interrupt
// enable start external interrupt
void APP_btn_stop_pressed(void)
{
	button_disable_INT(&st_gs_button_0);
	button_enable_INT(&st_gs_button_1);
}



// stop button external interrupt handler
void APP_ext_int0_cbf(void)
{
	// switch the running task to SOS_disable
	APP_runing_task = SOS_disable;
	
	// change SOS state to stop to exit from SOS_run
	SOS_change_state(DISABLE);
	
	// send external interrupt control handler to SOS_disable
	SOS_disable_ext_interrupt(APP_btn_stop_pressed);
	
	// Turn leds off
	LED_off(LED_0);
	LED_off(LED_1);
}



// start button external interrupt handler
void APP_wake_up_cbf(void)
{
	// switch the running task to SOS_run
	APP_runing_task = SOS_run;
	
	// change SOS state to stop to exit from SOS_disable
	SOS_change_state(INIT);
	
	// send external interrupt control handler to SOS_disable
	SOS_disable_ext_interrupt(APP_btn_start_pressed);
}
