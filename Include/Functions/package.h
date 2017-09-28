/**
*	@file package.h
*	@brief This file contains all function declartions used for creating protocol package.
*	
*/

#include<string.h>
#include<stdio.h>
#include "init.h"

#define MAX_PACK_LEN 999

short stringLength(char *str);
char* createPackage(uint8_t ID, uint8_t subID, uint8_t ctrl, char *data);
