/*
 * sos.c
 *
 * Created: 6/1/2023 5:17:15 PM
 *  Author: Mahmoud Sarhan
 */ 
/************************************************************************************************/
/*									Includes														*/
/************************************************************************************************/
#include "sos.h"
#include "../../MCAL/timer/timer.h"
/************************************************************************************************/
/*									Macros														*/
/************************************************************************************************/
#define NOT_INIT				0
#define INIT					1
#define DISABLE					2
#define TICK_TIME_1_MS			1
#define DATABASE_FIRST_INDEX	1
#define INVALID_TASK_ID			100
#define INVALID_TASK_PERIORITY	100
#define INVALID_TASK_PERIOD		65536
#define WAITING_STACK_SIZE		5


/************************************************************************************************/
/*									Static and global variables 						*/
/************************************************************************************************/
static st_task_parameters_t arr_st_gs_task_parameters[INVALID_TASK_ID]		= {NULL};
static uint8_t u8_gs_arr_index_id[INVALID_TASK_ID]							= {NOT_INIT};
static uint8_t u8_gs_arr_index_periority[INVALID_TASK_PERIORITY]			= {NOT_INIT};
static uint8_t u8_gs_max_count_of_tasks										= DATABASE_FIRST_INDEX;
static uint8_t u8_gs_sos_module_state										= NOT_INIT;
static uint16_t u16_sos_task_tick[INVALID_TASK_ID]							= {NOT_INIT};
static uint16_t u8_gs_rearrange_flag										= INIT;
static ptr_function_name_t ptr_function_disable_ext_interrupt						= NULL;



/************************************************************************************************/
/*									Static functions prototype														*/
/************************************************************************************************/
static enu_system_status_t SOS_token_config_param(ptr_function_name_t ptr_function_name , uint8_t u8_task_id , uint8_t u8_task_periority);
void SOS_tick_cbf(void);
static void SOS_rearrange_tasks(void);

/************************************************************************************************/
/*									Functions implementation														*/
/************************************************************************************************/
/*
* Desc : SOS_init to inti the sos module
* Input : void
* Output : SOS_STATUS_SUCCESS if done successfully
*		   SOS_STATUS_INVALID_STATE if anything wrong
*/
enu_system_status_t SOS_init(void)
{
	enu_system_status_t enu_system_status_retVal = SOS_STATUS_SUCCESS;
	if (NOT_INIT == u8_gs_sos_module_state)
	{
		enu_system_status_retVal = TIMER2_enuInit(OVF_MODE);
		enu_system_status_retVal = TIMER2_enuSetPrescallar(TIMER_PRE_64);
		enu_system_status_retVal = TIMER2_enuSetTime_ms(TICK_TIME_1_MS);
		enu_system_status_retVal = TIMER2_enuOVFIrqEnable();
		TIMER2_vidSetcbf_OVF(SOS_tick_cbf);
		if (SOS_STATUS_SUCCESS == enu_system_status_retVal)
		{
			u8_gs_sos_module_state = INIT;
		}
	}
	else
	{
		enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
	}
	if (enu_system_status_retVal)
	{
		enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
	}
	else
	{
		// do nothing
	}
	return enu_system_status_retVal;
}


/*
* Desc : SOS_deinit to deinti the sos module
* Input : void
* Output : SOS_STATUS_SUCCESS if done successfully
*		   SOS_STATUS_INVALID_STATE if anything wrong
*/
enu_system_status_t SOS_deinit (void)
{
	enu_system_status_t enu_system_status_retVal = SOS_STATUS_SUCCESS;
	if (INIT == u8_gs_sos_module_state)
	{
		for(uint8_t i = DATABASE_FIRST_INDEX ; i < u8_gs_max_count_of_tasks ; i++)
		{
			//delete task parameters
			u8_gs_arr_index_periority[arr_st_gs_task_parameters[i].u8_task_periority]		= NOT_INIT;
			arr_st_gs_task_parameters[i].ptr_function_name									= NULL;
			arr_st_gs_task_parameters[i].u8_task_periority									= NOT_INIT;
			arr_st_gs_task_parameters[i].u16_task_period									= NOT_INIT;
			
			// delete task index
			u8_gs_arr_index_id[arr_st_gs_task_parameters[i].u8_task_id]						= NOT_INIT;
			
			// delete task ID
			arr_st_gs_task_parameters[i].u8_task_id											= NOT_INIT;
			
			// delete task tick counter
			u16_sos_task_tick[i]															= NOT_INIT;
		}
		
		// set max count of tasks to the initial value
		u8_gs_max_count_of_tasks	= DATABASE_FIRST_INDEX;
		
		// change module state
		u8_gs_sos_module_state		= NOT_INIT;
	}
	else
	{
		enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
	}
	return enu_system_status_retVal;
}



/*
* Desc : SOS_create_task to create a task in the sos module
* Input :	ptr_function_name : pointer to the task
*			u8_task_id : task ID
*			u8_task_periority : task priority
*			u16_task_period : task period
* Output : SOS_STATUS_SUCCESS if done successfully
*		   SOS_STATUS_INVALID_STATE if anything wrong
*/
enu_system_status_t SOS_create_task(ptr_function_name_t ptr_function_name , uint8_t u8_task_id , uint8_t u8_task_periority,uint16_t u16_task_period)
{
	enu_system_status_t enu_system_status_retVal = SOS_STATUS_SUCCESS;
	
		//  if not null pointer
	if ((NULL != ptr_function_name)														&&
		
		// if valid task ID 
		(INVALID_TASK_ID > u8_task_id)													&&
		
		// if the ID below the first index in the database 
		(DATABASE_FIRST_INDEX-1 != u8_task_id)											&&
		
		// if valid priority
		(INVALID_TASK_PERIORITY > u8_task_periority)									&&
		
		// if valid period 
		(INVALID_TASK_PERIOD > u16_task_period)											&&
		
		// if the required database index is empty 
		(NULL == arr_st_gs_task_parameters[u8_gs_max_count_of_tasks].ptr_function_name) &&
		
		// if the module state is init
		(INIT == u8_gs_sos_module_state)												&&
		
		// if any of the parameters was taken before 
		(SOS_STATUS_INVALID_STATE != SOS_token_config_param(ptr_function_name,u8_task_id,u8_task_periority)))
	{
		// store the task parameters
		arr_st_gs_task_parameters[u8_gs_max_count_of_tasks].ptr_function_name		= ptr_function_name;
		arr_st_gs_task_parameters[u8_gs_max_count_of_tasks].u8_task_periority		= u8_task_periority;
		arr_st_gs_task_parameters[u8_gs_max_count_of_tasks].u16_task_period			= u16_task_period;
		arr_st_gs_task_parameters[u8_gs_max_count_of_tasks].u8_task_id				= u8_task_id;
		
		
		// store priority index
		u8_gs_arr_index_periority[u8_task_periority]								= u8_gs_max_count_of_tasks;
		
		// store task index by using task id
		u8_gs_arr_index_id[u8_task_id]												= u8_gs_max_count_of_tasks;
		
		
		// increment task counter
		u8_gs_max_count_of_tasks++;
	}
	else
	{
		enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
	}
	return enu_system_status_retVal;
}


/*
* Desc : SOS_delete_task to delete a task from the sos module
* Input : u8_task_id : task ID
* Output : SOS_STATUS_SUCCESS if done successfully
*		   SOS_STATUS_INVALID_STATE if anything wrong
*/
enu_system_status_t SOS_delete_task(uint8_t u8_task_id)
{
	enu_system_status_t enu_system_status_retVal = SOS_STATUS_SUCCESS;
	
	
		// If the module state was init
	if ((INIT == u8_gs_sos_module_state)						&&
		
		// if the ID below the first index in the database 
		(DATABASE_FIRST_INDEX-1 != u8_task_id)					&&
		
		// if valid task ID
		(INVALID_TASK_ID > u8_task_id)							&&
		
		// If there is a task with the given ID 
		(NOT_INIT != u8_gs_arr_index_id[u8_task_id]))
	{
		
		// clear task parameters
		u8_gs_arr_index_periority[arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u8_task_periority]	= NOT_INIT;
		arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].ptr_function_name								= NULL;
		arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u8_task_periority								= NOT_INIT;
		arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u16_task_period								= NOT_INIT;
		arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u8_task_id									= NOT_INIT;
		u8_gs_arr_index_id[u8_task_id]																			= NOT_INIT;
		
		
		// if it is not the final task rearrange tasks
		if (u8_gs_max_count_of_tasks-DATABASE_FIRST_INDEX != u8_gs_arr_index_id[u8_task_id])
		{
			SOS_rearrange_tasks();
		}
		else
		{
			// do nothing
		}
		
		u8_gs_max_count_of_tasks--;
	}
	else
	{
		enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
	}
	return enu_system_status_retVal;
}



/*
* Desc : SOS_modify_task to modify a task in the sos module
* Input :	u8_task_id : task ID
*			u8_task_periority : task priority
*			u16_task_period : task period
* Output : SOS_STATUS_SUCCESS if done successfully
*		   SOS_STATUS_INVALID_STATE if anything wrong
*/
enu_system_status_t SOS_modify_task(uint8_t u8_task_id , uint8_t u8_task_periority,uint16_t u16_task_period)
{
	enu_system_status_t enu_system_status_retVal = SOS_STATUS_SUCCESS;
		
		// if the module state is init 
	if ((INIT == u8_gs_sos_module_state)						&&
	
		// if valid task priority 
		(INVALID_TASK_PERIORITY > u8_task_periority)			&&
		
		// if valid task ID 
		(INVALID_TASK_ID > u8_task_id)							&&
		
		// if the ID below the first index in the database
		(DATABASE_FIRST_INDEX-1 != u8_task_id)					&&
		
		// if valid period
		(INVALID_TASK_PERIOD > u16_task_period)					&&
		
		// if the place in the database is not empty and stored before
		(NOT_INIT != u8_gs_arr_index_id[u8_task_id]))
	{
		// if it is a different priority
		if (arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u8_task_periority != u8_task_periority)
		{
			if (NOT_INIT == u8_gs_arr_index_periority[u8_task_periority])
			{
				u8_gs_arr_index_periority[arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u8_task_periority] = NOT_INIT;
				
				// modify task parameters
				arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u8_task_periority		= u8_task_periority;
				arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u16_task_period		= u16_task_period;
				u8_gs_arr_index_periority[u8_task_periority]									= u8_gs_arr_index_id[u8_task_id];
				SOS_rearrange_tasks();
			}
			else
			{
				enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
			}
			
		}
		else
		{
			// modify task parameters
			//arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u8_task_periority		= u8_task_periority;
			arr_st_gs_task_parameters[u8_gs_arr_index_id[u8_task_id]].u16_task_period		= u16_task_period;
			u8_gs_arr_index_periority[u8_task_periority]									= u8_gs_arr_index_id[u8_task_id];
		}
	}
	else
	{
		enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
	}
	return enu_system_status_retVal;
}

/*
* Desc : SOS_run to run the sos module
* Input : void
* Output : void
*/
void SOS_run(void)
{
	// arrange tasks for 1 time only in the first start
	if (INIT == u8_gs_rearrange_flag)
	{
		SOS_rearrange_tasks();
		u8_gs_rearrange_flag = NOT_INIT;
	}
	else
	{
		// do nothing
	}
	
	
	// disable start external interrupt
	// enable stop external interrupt
	if (NULL != ptr_function_disable_ext_interrupt)
	{
		// run handler
		(ptr_function_disable_ext_interrupt)();
		
		// clear the pointer
		ptr_function_disable_ext_interrupt = NULL;
	}
	else
	{
		// do nothing
	}
	
	
	// start timer 
	TIMER2_enuStart();
	
	
	// loop till the state change 
	while(INIT == u8_gs_sos_module_state)
	{
		
		// loop on tasks based on priority and execution time
		for(uint8_t i = DATABASE_FIRST_INDEX ; i  < u8_gs_max_count_of_tasks ; i++)
		{
			// if its the time to be executed the task will run
			if (arr_st_gs_task_parameters[i].u16_task_period == u16_sos_task_tick[i])
			{
				// run the task
				(arr_st_gs_task_parameters[i].ptr_function_name)();
				
				// clear the task tick counter
				u16_sos_task_tick[i] = 0;
			}
		}
	}
}


/*
* Desc : SOS_disable to disable the sos module
* Input : void
* Output : void
*/
void SOS_disable(void)
{
	// stop timer
	TIMER2_vidStop();
	
	// disable stop external interrupt
	// enable start external interrupt
	if (NULL != ptr_function_disable_ext_interrupt)
	{
		// run handler
		(ptr_function_disable_ext_interrupt)();
		
		// clear the pointer
		ptr_function_disable_ext_interrupt = NULL;
	}
	while(DISABLE == u8_gs_sos_module_state)
	{
		// stay here till the state changes
	}
}

/************************************************************************************************/
/*									Static function Implementation														*/
/************************************************************************************************/
/*
* Desc : SOS_token_config_param to check if any of the parameters token before
* Input :	u8_task_id : task ID
*			u8_task_periority : task priority
*			u16_task_period : task period
* Output : SOS_STATUS_SUCCESS if done successfully
*		   SOS_STATUS_INVALID_STATE if anything wrong
*/
static enu_system_status_t SOS_token_config_param(ptr_function_name_t ptr_function_name , uint8_t u8_task_id , uint8_t u8_task_periority)
{
	enu_system_status_t enu_system_status_retVal = SOS_STATUS_SUCCESS;
	
	
	// The task ID was taken before
	if ((NOT_INIT != u8_gs_arr_index_id[u8_task_id]) ||
		// The task priority was taken before 
		(NOT_INIT != u8_gs_arr_index_periority[u8_task_periority]))
	{
		enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
	}
	else
	{
		// check if the function name (pointer) was taken before
		for (uint8_t i = DATABASE_FIRST_INDEX ; i < u8_gs_max_count_of_tasks ; i++)
		{
			if (arr_st_gs_task_parameters[i].ptr_function_name == ptr_function_name)
			{
				enu_system_status_retVal = SOS_STATUS_INVALID_STATE;
				break;
			}
			else
			{
				// do nothing
			}
		}
	}
	return enu_system_status_retVal;
}

/*
* Desc : SOS_rearrange_tasks to rearrange tasks in the sos module
* Input : void
* Output : void
*/
static void SOS_rearrange_tasks(void)
{
	static st_task_parameters_t st_task_config;
	static uint8_t u8_s_tick_index = 0;
	for(uint8_t i = DATABASE_FIRST_INDEX ; i < u8_gs_max_count_of_tasks ; i++)
	{
		for(uint8_t j = i+1 ; j < u8_gs_max_count_of_tasks ; j++)
		{
			if ((arr_st_gs_task_parameters[i].u8_task_periority > arr_st_gs_task_parameters[j].u8_task_periority))
			{
				// switch function name (pointer)
				st_task_config.ptr_function_name				= arr_st_gs_task_parameters[i].ptr_function_name;
				arr_st_gs_task_parameters[i].ptr_function_name	= arr_st_gs_task_parameters[j].ptr_function_name;
				arr_st_gs_task_parameters[j].ptr_function_name	= st_task_config.ptr_function_name;
				st_task_config.ptr_function_name				= NULL;
				
				// switch task tick counter
				u8_s_tick_index			= u16_sos_task_tick[i];
				u16_sos_task_tick[i]	= u16_sos_task_tick[j];
				u16_sos_task_tick[j]	= u8_s_tick_index;
				u8_s_tick_index			= NOT_INIT;
				
				// switch task period
				st_task_config.u16_task_period					= arr_st_gs_task_parameters[i].u16_task_period;
				arr_st_gs_task_parameters[i].u16_task_period	= arr_st_gs_task_parameters[j].u16_task_period;
				arr_st_gs_task_parameters[j].u16_task_period	= st_task_config.u16_task_period;
				st_task_config.u16_task_period					= NOT_INIT;
				
				// switch task ID
				st_task_config.u8_task_id					= arr_st_gs_task_parameters[i].u8_task_id;
				arr_st_gs_task_parameters[i].u8_task_id		= arr_st_gs_task_parameters[j].u8_task_id;
				arr_st_gs_task_parameters[j].u8_task_id		= st_task_config.u8_task_id;
				
				// update task ID index
				u8_gs_arr_index_id[arr_st_gs_task_parameters[i].u8_task_id] = i;
				u8_gs_arr_index_id[arr_st_gs_task_parameters[j].u8_task_id] = j;
				st_task_config.u8_task_id									= NOT_INIT;
				
				// switch task priority
				st_task_config.u8_task_periority				= arr_st_gs_task_parameters[i].u8_task_periority;
				arr_st_gs_task_parameters[i].u8_task_periority	= arr_st_gs_task_parameters[j].u8_task_periority;
				arr_st_gs_task_parameters[j].u8_task_periority	= st_task_config.u8_task_periority;
				st_task_config.u8_task_periority				= NOT_INIT;
				
			}
		}
	}
}


void SOS_tick_cbf(void)
{
	// loop and increment tasks tick counter
	for (uint8_t i = 1 ; i < u8_gs_max_count_of_tasks; i++)
	{
		/****************************************************************************************************************/
		/*                       increment to the task period only														*/
		/* to insure tasks will take action and not skipped in the case of the equaled priority in same execution time	*/
		/****************************************************************************************************************/  
		if (u16_sos_task_tick[i] != arr_st_gs_task_parameters[i].u16_task_period)
		{
			u16_sos_task_tick[i]++;
		}
	}
}

void SOS_change_state(uint8_t u8_state)
{
	u8_gs_sos_module_state = u8_state;
}

/***********************************************************************************/
/* Desc : save the callback function locally to call it in SOS_run and SOS_disable */
/***********************************************************************************/
void SOS_disable_ext_interrupt(ptr_function_name_t ptr_function_name)
{
	ptr_function_disable_ext_interrupt = ptr_function_name;
}

/************************************************************************************************/
/*									End														*/
/************************************************************************************************/

