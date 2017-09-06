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
#include "functions.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_mfrc522.h"
#include "step_motor.h"

#define STACK_SIZE_MIN 500

//define servo motor positions
#define SERVO_MAX		2200 // servo max(left)
#define SERVO_MIN		1300 // servo min(right)
#define SERVO_MIDDLE	(SERVO_MIN + (SERVO_MAX - SERVO_MIN) / 2)

//define user info constants
#define MAX_USER 50

TM_PWM_TIM_t servo_timer;
volatile int servo_const=3000;

TM_HCSR04_t HCSR04;

//freeRTOS task hanlders
static TaskHandle_t tHandDHT = NULL;
static TaskHandle_t RFIDhandle = NULL;
static TaskHandle_t HCSRHhandle = NULL;

//task functions prototype
void dht_task (void *prvParams);
void rfid_task(void *prvParameters);
void space_mapping(void *prvParameters);
