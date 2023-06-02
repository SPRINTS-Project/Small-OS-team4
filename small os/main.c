/*
 * small os.c
 *
 * Created: 5/28/2023 1:58:00 PM
 * Author : Mahmoud Sarhan
 */ 

#define F_CPU 8000000UL

#include "APP/app.h"


int main(void)
{
	uint8_t u8_retVal = 0;
	u8_retVal = APP_init();
	if (0 == u8_retVal)
	{
		APP_start();
	}
	
	
	
	
}