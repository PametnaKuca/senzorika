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
#include "stm32f4xx_flash.h"
#include "misc.h"
#include "attributes.h"		
#include "tm_stm32f4_hcsr04.h"
#include "step_motor.h"



/* Definition of all the ports, pins and other peripheral devices which are used*/

/* LED port and pins ------------------------------------*/
#define LEDPORT GPIOD
#define LEDRCC RCC_AHB1Periph_GPIOD

#define LED1PIN GPIO_Pin_12
#define LED2PIN GPIO_Pin_13
#define LED3PIN GPIO_Pin_14
#define LED4PIN GPIO_Pin_15

#define BUTTONPIN GPIO_Pin_0
#define BUTTONPORT GPIOA
#define BUTTONRCC RCC_AHB1Periph_GPIOA
/* -------------------------------------------------------*/

#define USARTID USART2
#define USARTRCC RCC_APB1Periph_USART2

/* DH22 port, pins and timer --------------------------- */
#define DHT22_GPIO	GPIOA

#define DHT22_DATA_PIN1 GPIO_Pin_10
#define DHT22_DATA_PIN2 GPIO_Pin_8		// In case you want to add another sensor. This requires you to write additional code.

#define DHT22_TIM	TIM2
#define DHT22RCC RCC_APB1Periph_TIM2
#define DHT22_REFRESHRATE 10000	//refresh rate in ms
/* ------------------------------------------------------*/

/* HCSR ultrasonic sensor port, pins and timer ----------*/
#define HCSRPORT GPIOC
#define HCSR_RCC 	RCC_AHB1Periph_GPIOC
#define HCSR_ECHO_PIN	GPIO_Pin_4
#define HCSR_TRIG_PIN GPIO_Pin_5

#define TM_DELAY_TIM	TIM4
#define TM_DELAY_TIM_IRQ	TIM4_IRQn
#define TM_DELAY_TIM_IRQ_HANDLER	TIM4_IRQHandler
/* ------------------------------------------------------*/

/* Step motor port and pins -----------------------------*/
#define STEP_PORT GPIOB
#define STEP_PIN GPIO_Pin_0
#define STEPRCC RCC_AHB1Periph_GPIOB
/* ------------------------------------------------------*/

/* MFRC522 and SPI port and pins ------------------------*/
#define MFRC522_CS_RCC					RCC_AHB1Periph_GPIOA
#define MFRC522_CS_PORT					GPIOA
#define MFRC522_CS_PIN					GPIO_Pin_8

#define MFRC522_SPI						SPI1
#define MFRC522_SPI_PINSPACK			TM_SPI_PinsPack_2
/* ------------------------------------------------------*/

static char message[80];
void gpio_init(void);
void flash_init(void);
void USART2_Config(void);
