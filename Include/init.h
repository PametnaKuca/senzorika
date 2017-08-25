/**
*	@file init.h
*	@brief This file contains constans and init function prototypes
*	
*/

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"


/* Definition of all the ports, pins and other peripheral devices which are used*/

/* LED port and pins ------------------------------------*/
#define LEDPORT GPIOD
#define LEDRCC RCC_AHB1Periph_GPIOD

#define LED1PIN GPIO_Pin_12
#define LED2PIN GPIO_Pin_13
#define LED3PIN GPIO_Pin_14
#define LED4PIN GPIO_Pin_15
/* -------------------------------------------------------*/

#define BUTTONPORT GPIOA
#define BUTTONRCC RCC_AHB1Periph_GPIOA
#define BUTTONPIN GPIO_Pin_0


#define USARTID USART2
#define USARTRCC RCC_APB1Periph_USART2

/* DH22 port, pins and timer --------------------------- */
#define DHT22_GPIO	GPIOA

#define DHT22_DATA_PIN1 GPIO_Pin_10
#define DHT22_DATA_PIN2 GPIO_Pin_8

#define DHT22_TIM	TIM2
#define DHT22_REFRESHRATE 3000	//refresh rate in ms
/* ------------------------------------------------------*/

/* Vanjski prekidi */

#define EXTIPORT EXTI_PortSourceGPIOA
#define SYSRCC RCC_APB2Periph_SYSCFG
#define EXTIPIN EXTI_PinSource0


static char buffer[80];
void gpio_init(void);
void exti_init(void);
void nvic_init(void);
void USART2_Config(void);
void sendToUart(char *);
void TIM2_Init(void);
