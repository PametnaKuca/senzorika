#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define ANGLEmin	0.35  	// minimal possible angle movement
#define gearratio	5.182		//gear ratio
#define ANGLE	9  					//desired angle step
#define ANGLEmax 180


void init_step(void);
void move_positive(void);
void move_negative(void);
void delay_step(uint32_t ms);
