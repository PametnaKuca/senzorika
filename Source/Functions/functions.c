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

void initList(List *list)
{
	list->top = NULL;
}

atom *search(List *list, uint8_t *id)
{
	atom *p;
	for (p = list->top; p!=NULL; p=p->next)
	{
		if(compareID(id,&(p->element.ID[0])))
			return p;
	}
	return NULL;
}

bool addToList(List *list, User element)
{
	atom *newUser;
	if((newUser = (atom*) malloc(sizeof(atom))) != NULL)
	{
		newUser->element = element;
		newUser->next = list->top;
		list->top = newUser;
		return true;
	}
	return false;
}
		
bool deleteFromList(List *list, uint8_t *id)
{
	atom *p, *trash;
	for (p=list->top; p!=NULL; p=p->next)
	{
		if(p==list->top && compareID(id,&(p->element.ID[0])))
		{

				list->top = p->next;
				free(p);
				return true;
		}else{
				
		}
	}
	return false;
}
