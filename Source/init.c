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
}


void USART2_Config(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
				NVIC_InitTypeDef NVIC_InitStructure;

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);


        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // PA.2 = TX
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // PA.3 = RX

        USART_InitStructure.USART_BaudRate            = 115200;
        USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits            = USART_StopBits_1;
        USART_InitStructure.USART_Parity              = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

        USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
        USART_Init(USARTID, &USART_InitStructure);
        USART_Cmd(USARTID, ENABLE);

        USART_ITConfig(USARTID, USART_IT_RXNE, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel	=	USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
        NVIC_InitStructure.NVIC_IRQChannelCmd 	=	ENABLE;
        NVIC_Init(&NVIC_InitStructure);
}

void TIM2_Init(void) {
  uint16_t PrescalerValue = 0;

  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535 - 1;      //in uSecs
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_Cmd(TIM2, ENABLE);
}
