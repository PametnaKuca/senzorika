#include "main.h"

int main()
{	
	gpio_init();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
	USART2_Config();
	TIM2_Init();
	DHT22_Init(DHT22_DATA_PIN1);
	//Not accessible for now.
	//DHT22_Init(DHT22_DATA_PIN2);
	
	xTaskCreate(dht_task, "dht task", STACK_SIZE_MIN, NULL, 2, &tHandDHT);
	
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
			
				vTaskPrioritySet(NULL, 3);
        //Read the data from sensor 2. Not accessible for now.
        //DHT22_Read(DHT22_DATA_PIN2);

        temperature+=DHT22getTemperature();
        humidity+=DHT22getHumidity();

        temperature/=NUMBER_OF_SESNORS;
        humidity/=NUMBER_OF_SESNORS;
			
        vTaskPrioritySet(NULL, 2);

        GPIO_ResetBits(LEDPORT, LED4PIN);

        vTaskDelay(3000/portTICK_RATE_MS);
	}
}
