/**
*	@file init.c
*	@brief This file contains init functions
*	
*/



#include "init.h"

/* add startup functions (GPIO, RCC, UART...) here*/
void gpio_init(void)
{
	RCC_AHB1PeriphClockCmd(LEDRCC, ENABLE);

	GPIO_InitTypeDef GPIO_LEDS;
	GPIO_LEDS.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_LEDS.GPIO_OType=GPIO_OType_PP;
	GPIO_LEDS.GPIO_Pin= LED1PIN | LED2PIN | LED3PIN | LED4PIN;
	GPIO_LEDS.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_LEDS.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(LEDPORT, &GPIO_LEDS);		
	
	GPIO_InitTypeDef GPIO_BUTTON;
	GPIO_BUTTON.GPIO_Mode=GPIO_Mode_IN;
	GPIO_BUTTON.GPIO_OType=GPIO_OType_PP;
	GPIO_BUTTON.GPIO_Pin=BUTTONPIN;
	GPIO_BUTTON.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_BUTTON.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(BUTTONPORT, &GPIO_BUTTON);
}

void flash_init(void)
{
	FLASH_SetLatency(FLASH_Latency_1);
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_InstructionCacheCmd(ENABLE);
	FLASH_DataCacheCmd(ENABLE);
}

void USART_Config(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
				NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(USARTRCC, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);


        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | USART_RX_PIN;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_Init(USARTPORT, &GPIO_InitStructure);

        GPIO_PinAFConfig(USARTPORT, TX_PIN_SOURCE, USART_AF); // PA.2 = TX
        GPIO_PinAFConfig(USARTPORT, RX_PIN_SOURCE, USART_AF); // PA.3 = RX

        USART_InitStructure.USART_BaudRate            = 115200;
        USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits            = USART_StopBits_1;
        USART_InitStructure.USART_Parity              = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

        USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
        USART_Init(USARTID, &USART_InitStructure);
        USART_Cmd(USARTID, ENABLE);

        USART_ITConfig(USARTID, USART_IT_RXNE, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel	=	USART_IRQ;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
        NVIC_InitStructure.NVIC_IRQChannelCmd 	=	ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}
