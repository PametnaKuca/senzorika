#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define ANGLEmin	0.35  //step angle 
#define gearratio	5.182			//gear ratio
#define ANGLE	9  //angle step
#define ANGLEmax 180


#define STEP_PORT GPIOB
#define STEP_PIN GPIO_Pin_0
#define STEPRCC RCC_AHB1Periph_GPIOB

void init_step(void);
void move_positive(void);
void move_negative(void);
void delay_step(uint32_t ms);
