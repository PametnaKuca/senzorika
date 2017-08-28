/**
*	@file functions.h
*	@brief This file contains all function declartions used for user and memory management.
*	
*/

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "init.h"

/**
*	This function is used to send a string of chars to the serial port.
*	@param	first	Pointer to the first element of the input char array.
*/
void sendToUart(char *);
