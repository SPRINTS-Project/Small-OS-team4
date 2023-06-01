/*
 * dio.h
 *
 * Created: 5/25/2023 10:09:18 PM
 *  Author: Mahmoud Sarhan
 */ 


#ifndef DIO_H_
#define DIO_H_

#include "../../STD_LIB/std_types.h"


typedef enum 
{
	porta, portb, portc, portd
} en_dioPorts_t;
typedef enum 
{
	pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7
} en_dioPins_t;


// DIO Level type ENUM
typedef uint8_t u8_dioLevel_t;

#define STD_LOW					((u8_dioLevel_t)0x00)
#define STD_HIGH				((u8_dioLevel_t)0x01)


// DIO Errors 
typedef uint8_t u8_dioErrors_t;

#define DIO_E_OK				((u8_dioErrors_t)0x00)
#define DIO_InvalidPin			((u8_dioErrors_t)0x01)
#define DIO_InvalidPort			((u8_dioErrors_t)0x02)


// DIO DIR type 
typedef uint8_t u8_dioDir_t;

#define STD_INPUT					((u8_dioDir_t)0x00)
#define STD_OUTPUT					((u8_dioDir_t)0x01)
/************************************************************************/
/* `DIO_init(en_dioPorts_t port, en_dioPins_t pin, u8_dioDir_t dir)`:
	- This function initializes a specific pin of a given port with the specified direction.
	- Parameters:
	- `port`: The port to which the pin belongs (porta, portb, portc, or portd).
	- `pin`: The pin number to be initialized.
	- `dir`: The direction of the pin (STD_INPUT or STD_OUTPUT).
	- Returns:
	- `DIO_E_OK` if the initialization is successful.
	- `DIO_InvalidPort` if an invalid port is specified.                                                                     */
/************************************************************************/
u8_dioErrors_t DIO_init (en_dioPorts_t port, en_dioPins_t pin, u8_dioDir_t direction);

/************************************************************************/
/*`DIO_writePIN(en_dioPorts_t port, en_dioPins_t pin, u8_dioLevel_t state)`:
	- This function writes a specified state to the specified pin of a given port.
	- Parameters:
	- `port`: The port to which the pin belongs (porta, portb, portc, or portd).
	- `pin`: The pin number to be written.
	- `state`: The state to be written to the pin (STD_HIGH or STD_LOW).
	- Returns:
	- `DIO_E_OK` if the write operation is successful.
	- `DIO_InvalidPort` if an invalid port is specified.                                                                   */
/************************************************************************/
u8_dioErrors_t DIO_writePIN (en_dioPorts_t port, en_dioPins_t pin, u8_dioLevel_t state);
/************************************************************************/
/* `DIO_readPIN(en_dioPorts_t port, en_dioPins_t pin, u8_dioLevel_t* value)`:
	- This function reads the state of the specified pin of a given port.
	- Parameters:
	- `port`: The port to which the pin belongs (porta, portb, portc, or portd).
	- `pin`: The pin number to be read.
	- `value`: A pointer to a variable to store the read value.
	- Returns:
	- `DIO_E_OK` if the read operation is successful.
	- `DIO_InvalidPort` if an invalid port is specified.                                                                     */
/************************************************************************/
u8_dioErrors_t DIO_readPIN (en_dioPorts_t port, en_dioPins_t pin, u8_dioLevel_t* value);
/************************************************************************/
/* `DIO_togglePIN` that toggles the state of the specified pin of a given port. Here is a summary of the function:

	- Parameters:
	- `port`: The port to which the pin belongs (porta, portb, portc, or portd).
	- `pin`: The pin number to be toggled.
	- Behavior:
	- The function uses a switch-case statement to determine the port and perform the toggle operation accordingly.
	- Inside each case, the `TOGGLE_BIT` macro is used to toggle the state of the specified pin.
	- Returns:
	- `DIO_E_OK` if the toggle operation is successful.
	- `DIO_InvalidPort` if an invalid port is specified.                                                                     */
/************************************************************************/
u8_dioErrors_t DIO_togglePIN (en_dioPorts_t port, en_dioPins_t pin);
/************************************************************************/
/* DIO_readPORT(en_dioPorts_t port, u8_dioLevel_t* data):

	This function reads the state of all pins of a given port.
	Parameters:
	port: The port to be read (porta, portb, portc, or portd).
	data: A pointer to a variable to store the read data.
	Returns:
	DIO_E_OK if the read operation is successful.
	DIO_InvalidPort if an invalid port is specified.                                                                     */
/************************************************************************/
u8_dioErrors_t DIO_readPORT (en_dioPorts_t port,u8_dioLevel_t *value);
/************************************************************************/
/* 
	DIO_writePORT(en_dioPorts_t port, u8_dioLevel_t state):

	This function writes the specified state to all pins of a given port.
	Parameters:
	port: The port to which the state will be written (porta, portb, portc, or portd).
	state: The state to be written to the port.
	Returns:
	DIO_E_OK if the write operation is successful.
	DIO_InvalidPort if an invalid port is specified.                                                                     */
/************************************************************************/
u8_dioErrors_t DIO_writePORT (en_dioPorts_t port,u8_dioLevel_t state);



#endif /* DIO_H_ */