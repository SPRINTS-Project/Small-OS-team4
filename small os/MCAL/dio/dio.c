/**

	@file dio.c
	@brief Digital Input/Output (DIO) functions implementation.
	@author Youssef Abbas
*/

#include "dio.h"
#include "dio_private_reg.h"
/**

	@brief Initializes the specified pin of a given port with the specified direction.
	@param port The port to which the pin belongs (porta, portb, portc, or portd).
	@param pin The pin number to be initialized.
	@param dir The direction of the pin (STD_INPUT or STD_OUTPUT).
	@return DIO_E_OK if the initialization is successful, otherwise DIO_InvalidPort.
*/
u8_dioErrors_t DIO_init (en_dioPorts_t port, en_dioPins_t pin, u8_dioDir_t dir){
	switch(port){
		case porta:
		(dir == STD_INPUT) ? CLEAR_BIT(DATA_DIRECTION_PORTA,pin) :SET_BIT(DATA_DIRECTION_PORTA,pin);
		break;
		case portb:
		(dir == STD_INPUT) ? CLEAR_BIT(DATA_DIRECTION_PORTB,pin) :SET_BIT(DATA_DIRECTION_PORTB,pin);
		break;
		case portc:
		(dir == STD_INPUT) ? CLEAR_BIT(DATA_DIRECTION_PORTC,pin) :SET_BIT(DATA_DIRECTION_PORTC,pin);
		break;
		case portd:
		(dir == STD_INPUT) ? CLEAR_BIT(DATA_DIRECTION_PORTD,pin) :SET_BIT(DATA_DIRECTION_PORTD,pin);
		break;
		default:
		return DIO_InvalidPort;
		break;
	}
	return DIO_E_OK;
}

/**

	@brief Writes the specified state to the specified pin of a given port.
	@param port The port to which the pin belongs (porta, portb, portc, or portd).
	@param pin The pin number to be written.
	@param state The state to be written to the pin (STD_HIGH or STD_LOW).
	@return DIO_E_OK if the write operation is successful, otherwise DIO_InvalidPort.
*/
u8_dioErrors_t DIO_writePIN (en_dioPorts_t port, en_dioPins_t pin, u8_dioLevel_t state){
		switch(port){
			case porta:
			(state == STD_HIGH) ? CLEAR_BIT(WR_PORT_A,pin) :SET_BIT(WR_PORT_A,pin);
			break;
			case portb:
			(state == STD_HIGH) ? CLEAR_BIT(WR_PORT_B,pin) :SET_BIT(WR_PORT_B,pin);
			break;
			case portc:
			(state == STD_HIGH) ? CLEAR_BIT(WR_PORT_C,pin) :SET_BIT(WR_PORT_C,pin);
			break;
			case portd:
			(state == STD_HIGH) ? CLEAR_BIT(WR_PORT_D,pin) :SET_BIT(WR_PORT_D,pin);
			break;
			default:
			return DIO_InvalidPort;
			break;
		}
		return DIO_E_OK;
}
/**

	@brief Reads the state of the specified pin of a given port.
	@param port The port to which the pin belongs (porta, portb, portc, or portd).
	@param pin The pin number to be read.
	@param value A pointer to a variable to store the read value.
	@return DIO_E_OK if the read operation is successful, otherwise DIO_InvalidPort.
*/
u8_dioErrors_t DIO_readPIN (en_dioPorts_t port, en_dioPins_t pin, u8_dioLevel_t* value){
		switch(port){
			case porta:
			*value = READ_BIT(RE_PORT_A,pin);
			break;
			case portb:
			*value = READ_BIT(RE_PORT_B,pin);
			break;
			case portc:
			*value = READ_BIT(RE_PORT_C,pin);
			break;
			case portd:
			*value = READ_BIT(RE_PORT_D,pin);
			break;
			default:
			return DIO_InvalidPort;
			break;
		}
		return DIO_E_OK;
}

/**

	@brief Toggles the state of the specified pin of a given port.
	@param port The port to which the pin belongs (porta, portb, portc, or portd).
	@param pin The pin number to be toggled.
	@return DIO_E_OK if the toggle operation is successful, otherwise DIO_InvalidPort.
*/
u8_dioErrors_t DIO_togglePIN (en_dioPorts_t port, en_dioPins_t pin){
	switch(port){
		case porta:
			TOGGLE_BIT(WR_PORT_A,pin);
			break;
		case portb:
			TOGGLE_BIT(WR_PORT_B,pin);
		break;
		case portc:
			TOGGLE_BIT(WR_PORT_C,pin);
		break;
		case portd:
			TOGGLE_BIT(WR_PORT_D,pin);
		break;
		default:
		return DIO_InvalidPort;
		break;
	}
	return DIO_E_OK;
}

/**
	 * @brief Reads the state of all pins of a given port.
	 *
	 * @param port The port to be read (porta, portb, portc, or portd).
	 * @param data A pointer to a variable to store the read data.
	 *
	 * @return DIO_E_OK if the read operation is successful, otherwise DIO_InvalidPort.
 */
u8_dioErrors_t DIO_readPORT (en_dioPorts_t port,u8_dioLevel_t *data){
		switch(port){
			case porta:
			*data =	RE_PORT_A;
			break;
			case portb:
			*data =	RE_PORT_B;
			break;
			case portc:
			*data =	RE_PORT_C;
			break;
			case portd:
			*data =	RE_PORT_D;
			break;
			default:
			return DIO_InvalidPort;
			break;
		}
		return DIO_E_OK;
}
/**
	 * @brief Writes the specified state to all pins of a given port.
	 *
	 * @param port The port to which the state will be written (porta, portb, portc, or portd).
	 * @param state The state to be written to the port.
	 *
	 * @return DIO_E_OK if the write operation is successful, otherwise DIO_InvalidPort.
 */
u8_dioErrors_t DIO_writePORT (en_dioPorts_t port,u8_dioLevel_t state){
		switch(port){
			case porta:
			WR_PORT_A = state;
			break;
			case portb:
			WR_PORT_B = state;
			break;
			case portc:
			WR_PORT_C = state;
			break;
			case portd:
			WR_PORT_D = state;
			break;
			default:
			return DIO_InvalidPort;
			break;
		}
		return DIO_E_OK;
}
