/**
*	@file functions.c
*	@brief This file contains all functions used for user and memory management.
*	
*/

#include "FreeRTOS.h"
#include "task.h"
#include "functions.h"
#include "string.h"

void sendToUartText(char *first)
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

uint8_t initSuperUser(User *userArray)
{
		uint8_t numberOfUsers = 3;
		
		userArray[0].ID[0] = 250;
		userArray[0].ID[1] = 18;
		userArray[0].ID[2] = 68;
		userArray[0].ID[3] = 224;
		userArray[0].ID[4] = 76;
		strcpy(userArray[0].firstName,"Marko"); 
		strcpy(userArray[0].lastName,"Svec");
	
		userArray[1].ID[0] = 202;
		userArray[1].ID[1] = 210;
		userArray[1].ID[2] = 104;
		userArray[1].ID[3] = 224;
		userArray[1].ID[4] = 144;
		strcpy(userArray[1].firstName,"Adam"); 
		strcpy(userArray[1].lastName,"Sedmak");
	
		userArray[2].ID[0] = 197;
		userArray[2].ID[1] = 31;
		userArray[2].ID[2] = 252;
		userArray[2].ID[3] = 2;
		userArray[2].ID[4] = 36;
		strcpy(userArray[2].firstName,"Josko"); 
		strcpy(userArray[2].lastName,"Rogulj");
		
		return numberOfUsers;	
}

void writeSuperUser(User *superUser)
{
		uint32_t startAddress = 0x08008000;
		uint32_t currAddress = startAddress + 1;
		uint8_t numberOfUsers = 3;
	
		if(*(__IO uint8_t*)(startAddress) == 0xA5)
			return;
	
		FLASH_Unlock();
		FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3);
		FLASH_ProgramByte(startAddress,0xA5);
	
		for(uint8_t i=0; i<numberOfUsers; i++)
		{
			for(int j=0; j<5; j++)
			{
				FLASH_ProgramByte(currAddress,superUser[i].ID[j]);
				currAddress++;
			}
			for(int j=0; j<sizeof((superUser+i)->firstName); j++)
			{
				FLASH_ProgramByte(currAddress,superUser[i].firstName[j]);
				currAddress++;
			}
			for(int j=0; j<sizeof((superUser+i)->lastName); j++)
			{
				FLASH_ProgramByte(currAddress,superUser[i].lastName[j]);
				currAddress++;
			}
		}
		FLASH_Lock();	
}

void sendDHT22(float temperature, float humidity)
{
	char message[20];
	char id[] = {DHT_ID, '\0'};
	sprintf(message, "%.5f,%.5f",temperature,humidity);
	strcat(id,message);
	sendToUartText(&id[0]);
}

void sendDistance(float distance, int position)
{
		char message[20];
		char id[] = {MAP_ERROR_ID, '\0'};
		sprintf(message, "%.5f:%.5f\n\r", (float)position,distance);
		strcat(id,message);
		sendToUartText(&id[0]);
}

void sendInitialMap(float *mapArray)
{
		int number = ANGLEmax/ANGLE, i;
		float position = 0;
		char message[number*20], tempData[20];
		char id[] = {INITIAL_MAP_ID, '\0'};
		for(i=0;i<number;i++)
		{
			sprintf(tempData,"%.5f:%.5f,", position, *(mapArray+i));
			strcat(message,tempData);
			position+=ANGLE;
		}
		sprintf(tempData,"%.5f:%.5f", position, *(mapArray+i));
		strcat(message,tempData);
		strcat(id,message);
		sendToUartText(&id[0]);
}
