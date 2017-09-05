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

#define nameLen 15

typedef struct UserStruct {
	uint8_t ID[5];
	char firstName[nameLen];
	char lastName[nameLen];
} User;

//User list atom structure
typedef struct at{
	User element;
	struct at *next;
} atom;

//List definition
typedef struct{
	atom *top;
} List;

/**
*	This function is used to send a string of chars to the serial port.
*	@param	first	Pointer to the first element of the input char array.
*/
void sendToUart(char *);
/**
*	This function compares the input card ID with every users' card ID to see if the card belongs to a valid user.
*	@param	ID	Pointer to the first element of the input array.
*/
bool isUserValid(uint8_t* ID);
/**
* Function compares two ID arrays.
* @param id1 and id2 arrays.
*/
bool compareID(uint8_t *id1, uint8_t *id2)
/**
* This function is used to initialize user list.
* @param List pointer is sent in order to inizialize its top pointer.
*/
void initList(List *list);
/**
* This function searches through the list for the User with specific id.
* @param List pointer to know the top of the list and the pointer to wanted id array.
*/
atom *search(List *list, uint8_t *id);
/**
* Function adds user to the top of the list. The list itself acts like a stack. The only difference
* is that stack is LIFO system, whereas from this list it is possible to delete any atom( with specified ID
* if requested so.
* @param List pointer to know the top of the list and new user data.
*/
bool addToList(List *list, User element);
