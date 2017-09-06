#include "main.h"

int main()
{	
	gpio_init();
	DHT22_Init(DHT22_DATA_PIN1);
	TM_HCSR04_Init(&HCSR04, HCSRPORT, HCSR_ECHO_PIN, HCSRPORT, HCSR_TRIG_PIN);
	//initialize timer1 as pwm output with frequency of 50kHZ
  TM_PWM_InitTimer(TIM1,&servo_timer,50);
	//channels and pinspack defined in   "tm_stm32f4_pwm.h"
	TM_PWM_InitChannel(&servo_timer, TM_PWM_Channel_2, TM_PWM_PinsPack_1);
	//set initial postion as closed
  TM_PWM_SetChannelMicros(&servo_timer, TM_PWM_Channel_2, SERVO_MAX);
	//rfid module init
	TM_MFRC522_Init();
	USART2_Config();
	
	xTaskCreate(dht_task, "dht task", STACK_SIZE_MIN, NULL, 2, &tHandDHT);
	xTaskCreate(space_mapping, "space_mapping_task", STACK_SIZE_MIN, NULL, 2, &HCSRHhandle);
	xTaskCreate(rfid_task, "rfid_task", STACK_SIZE_MIN, NULL, 3, &RFIDhandle);
	
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
				
				//Probni ispis
				sprintf(message, "Temperature: %.5f Humidity: %.5f\n\r", temperature, humidity);
				sendToUart(&message[0]);	
				
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
				
				GPIO_SetBits(LEDPORT, LED2PIN);
				distance = TM_HCSR04_Read(&HCSR04);
				GPIO_ResetBits(LEDPORT, LED2PIN);
				vTaskDelay(10/portTICK_RATE_MS);
				
				//Piši na seriju
				sprintf(message, "Distance:\t %.5f\t Angle: %d\n\r", distance, position);
				sendToUart(&message[0]);
				
				vTaskDelay(1000/portTICK_RATE_MS);
	}
}

/**
*
*/

void rfid_task(void *prvParameters)
{
	static bool aux_flag=false;
	static uint32_t timerval=0;
	static uint8_t id[5];
	static User newUser;
	static User allUsers[MAX_USER];
	static uint8_t numberOfUsers = 0;
	
	allUsers[0].ID[0] = 250;
	allUsers[0].ID[1] = 18;
	allUsers[0].ID[2] = 68;
	allUsers[0].ID[3] = 224;
	allUsers[0].ID[4] = 76;
	strcpy(allUsers[0].firstName,"Marko"); 
	strcpy(allUsers[0].lastName,"Svec");
	numberOfUsers = 1;
	
	allUsers[1].ID[0] = 202;
	allUsers[1].ID[1] = 210;
	allUsers[1].ID[2] = 104;
	allUsers[1].ID[3] = 224;
	allUsers[1].ID[4] = 144;
	strcpy(allUsers[1].firstName,"Adam"); 
	strcpy(allUsers[1].lastName,"Sedmak");
	numberOfUsers = 2;
	
	

    while(1)
    {
        //checks if card is present
        if(TM_MFRC522_Check(id)==MI_OK){
						//Privremeni ispis
						sprintf(message, "ID: %d %d %d %d %d\n\r", id[0],id[1],id[2],id[3],id[4]);
						sendToUart(&message[0]);
					
            //check if user is valid
            if(isUserValid(&allUsers[0], numberOfUsers, &id[0])){
                GPIO_SetBits(LEDPORT,LED1PIN);
                TM_PWM_SetChannelMicros(&servo_timer, TM_PWM_Channel_2, SERVO_MIN); //grant access
                aux_flag=true;
                timerval=xTaskGetTickCount()/portTICK_RATE_MS;
            }
            else
                GPIO_SetBits(LEDPORT,LED3PIN); //red light for unauthorized user
        }
        else{
            GPIO_ResetBits(LEDPORT,LED3PIN | LED1PIN);
        }

        //prohibit enterance if timeout period expires
        if((xTaskGetTickCount()/portTICK_RATE_MS)-timerval > servo_const && aux_flag){
                TM_PWM_SetChannelMicros(&servo_timer, TM_PWM_Channel_2, SERVO_MAX);
                aux_flag=false;
        }
				
        vTaskDelay(200/portTICK_RATE_MS);
    }
}
