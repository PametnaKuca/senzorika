/**
*	@file main.h
*	@brief This file contains user constants and definitions
*
*/

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stdbool.h>
#include "stdint.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "semphr.h"
#include "dht22.h"
#include "init.h"

#define STACK_SIZE_MIN 500

//freeRTOS task hanlders
static TaskHandle_t tHandDHT = NULL;

//task functions prototype
void dht_task (void *prvParams);