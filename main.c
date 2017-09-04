#include "main.h"

int main()
{	
	gpio_init();
	DHT22_Init(DHT22_DATA_PIN1);
	TM_HCSR04_Init(&HCSR04, HCSRPORT, HCSR_ECHO_PIN, HCSRPORT, HCSR_TRIG_PIN);
	USART2_Config();
	
	xTaskCreate(dht_task, "dht task", STACK_SIZE_MIN, NULL, 2, &tHandDHT);
	xTaskCreate(space_mapping, "space_mapping_task", STACK_SIZE_MIN, NULL, 2, &HCSRHhandle);
	
	vTaskStartScheduler();
	while(1);
}

/**
*	The DHT task executes periodically every DHT22_REFRESHRATE milliseconds.
*	While the task is reading data from the sensor, which is an extremely time critical operation, the task raises its own priority
*	in order to prevent being halted either by another task or an ISR.
*/

//Iskoristi checksumValid kao provjeru ima li smisla slati podatke ili grešku
void dht_task(void *prvParams)
{
	while(1)
    {
        bool checksumValid;
        temperature = humidity = 0;
        GPIO_SetBits(LEDPORT, LED4PIN);
        vTaskPrioritySet(NULL, 3);
			
        //Read the data from sensor 1
        checksumValid=DHT22_Read(DHT22_DATA_PIN1);
        vTaskPrioritySet(NULL, 2);
			
        temperature+=DHT22getTemperature();
        humidity+=DHT22getHumidity();
			
        GPIO_ResetBits(LEDPORT, LED4PIN);

        vTaskDelay(DHT22_REFRESHRATE/portTICK_RATE_MS);
		}
}

/**
* The space_mapping task executes periodically and performs two actions. It moves the step motor in 360° range. On top of that motor
* there is ultrosonic sensor which reads the distance from the nearest object for every step of the rotation. Step of the rotation is 
* defined in step_motor.h.
**/

void space_mapping(void *prvParameters)
{
	static int i,position=0, direction=-1;
	static int number=( gearratio * (ANGLE/ANGLEmin) );
	static float distance;
	while(1)
	{		
			if (position >= ANGLEmax ) //In final position change the direction of the motor.
				{
					direction=-direction;
					position=ANGLEmax;   
				}

				else if ( position <= 0 ) //In final position change the direction of the motor.
				{
					direction=-direction;
					position=0;     
				}

				if ( direction > 0)
				{
					for (i=0;i<number;i++) move_positive();  //Move the sensor in positive direction.
					position+=ANGLE;									
				}

				else
				{
					for (i=0;i<number;i++) move_negative();  //Move the sensor in negative direction.
					position-=ANGLE;									 
				}

				distance = TM_HCSR04_Read(&HCSR04);
				Delayms(10);
				
				//Piši na seriju
				sprintf(message, "Distance:\t %.5f\t Angle: %d\n\r", distance, position);
				sendToUart(&message[0]);
				
				Delayms(1000);
	}
}
