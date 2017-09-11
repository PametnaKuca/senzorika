#include "step_motor.h"
#include "tm_stm32f4_delay.h"

const uint16_t LEDS[] = {GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_5}; //pins: PWMB,DIRA,DIRB
const uint16_t LED = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
const uint8_t steps[4][4] = {{0, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}}; //smjer okreta motora od kanala B do kanala A
const uint8_t steps_neg[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};	//smjer okreta motora od kanala A do kanala B
uint32_t i = 0;


void init_step() 
{
    RCC_AHB1PeriphClockCmd(STEPRCC, ENABLE);

    GPIO_InitTypeDef GPIO_STEP;
    GPIO_STEP.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_STEP.GPIO_OType = GPIO_OType_PP;
    GPIO_STEP.GPIO_Pin = STEP_PIN;
		GPIO_STEP.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_STEP.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(STEP_PORT, &GPIO_STEP);
}

//preuredi ovo. To je funkcija koja vozi motor u neku od strana. Pogledaj ove LED diode kad stigne motor?

void move_positive() {									//postavljanje vrijednosti pinova kao sto je prikazano u matrici steps
	uint8_t j;
	delay_step(10);
	for (j = 0; j < 4; j++) {

		if (steps[i][j] == 0) {
			if ( j == 0 )
		{ GPIO_ResetBits(STEP_PORT, STEP_PIN); }
		else{
			GPIO_ResetBits(GPIOA, LEDS[j-1]);
		}
		}
		else {
			if ( j == 0 )
		{ GPIO_SetBits(STEP_PORT, STEP_PIN); }
		else{
			GPIO_SetBits(GPIOA, LEDS[j-1]);
		}}
	}
    i=(i+1)%4;
}

void move_negative() {								//postavljanje vrijednosti pinova kao sto je prikazano u matrici steps_neg (suprotan smjer)
	uint8_t j;
	delay_step(10);
	for (j = 0; j < 4; j++) {

		if (steps_neg[i][j] == 0) {
			if ( j == 0 )
		{ GPIO_ResetBits(STEP_PORT, STEP_PIN); }
		else{
			GPIO_ResetBits(GPIOA, LEDS[j-1]);
		}
		}
		else {
			if ( j == 0 )
		{ GPIO_SetBits(STEP_PORT, STEP_PIN); }
		else{
			GPIO_SetBits(GPIOA, LEDS[j-1]);
		}}
	}
    i=(i+1)%4;
}

void delay_step(uint32_t ms) {			//interni delay, brzina vrtnje motora
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}
