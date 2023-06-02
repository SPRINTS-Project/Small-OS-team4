/*
 * app.h
 *
 * Created: 5/28/2023 8:04:52 PM
 *  Author: Mahmoud Sarhan
 */ 


#ifndef APP_H_
#define APP_H_
#include "../STD_LIB/std_types.h"


#define APP_ERROR_OK		0
#define APP_ERROR_NOT_OK	1


uint8_t APP_init(void);
void APP_start(void);


#endif /* APP_H_ */