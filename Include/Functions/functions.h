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
void sendToUart(char character);
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
/**
*	Functions returns array of super users throught the userArray pointer
*	@param userArray pointer
*/
uint8_t initSuperUser(User *userArray);
/**
*	Functions writes hardcoded super users to flash.
*	@param superUser array.
*/
void writeSuperUser(User *superUser);
/**
*	The function packs temperature and humidtiy into requested data form
* and sends it over serial port.
* @param temperature and humidity
*/
void sendDHT22(float temperature, float humidity);
/**
*	The functions packs distance and position into requested data form
* and sends it over serial port.
*	@param distance and position
*/
void sendDistance(float distance, int position);
/**
* The function sends initial space map over serial port.
* @param map array
*/
void sendInitialMap(float *mapArray);
