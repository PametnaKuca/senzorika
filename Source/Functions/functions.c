/**
*	@file functions.c
*	@brief This file contains all functions used for user and memory management.
*	
*/

#include "FreeRTOS.h"
#include "task.h"
#include "functions.h"

void sendToUart(char *first)
{
	while(*(first)!=0)
	{
		while(!USART_FLAG_TXE);
		USART_SendData(USARTID, *(first));
		while(!USART_GetFlagStatus(USARTID, USART_FLAG_TC));
		first++;
	}
}

bool compareID(uint8_t *id1, uint8_t *id2)
{
	if(*id1 == *id2 &&
			*(id1+1) == *(id2+1) &&
			*(id1+2) == *(id2+2) &&
			*(id1+3) == *(id2+3) &&
			*(id1+4) == *(id2+4))
			return true;
	return false;
}

bool isUserValid(User *userList,uint8_t numUser, uint8_t* ID)
{
	uint8_t i;
	for (i=0; i<numUser; i++)
	{
		if(compareID((userList+i)->ID,ID))
			return true;
	}
	return false;
}
