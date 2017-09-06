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

#define NAME_LEN 15

typedef struct UserStruct {
	uint8_t ID[5];
	char firstName[NAME_LEN+1];
	char lastName[NAME_LEN+1];
} User;

/**
*	This function is used to send a string of chars to the serial port.
*	@param	first	Pointer to the first element of the input char array.
*/
void sendToUart(char *);
/**
*	This function compares the input card ID with every users' card ID to see if the card belongs to a valid user.
*	@param	User list and ID	Pointer to the first element of the input array.
*/
bool isUserValid(User *userList,uint8_t numUser, uint8_t* ID);
/**
* Function compares two ID arrays.
* @param id1 and id2 arrays.
*/
bool compareID(uint8_t *id1, uint8_t *id2);
