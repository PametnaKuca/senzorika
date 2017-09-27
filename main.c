#include "main.h"

int main()
{	
		gpio_init();
		flash_init();
		init_step();
		DHT22_Init(DHT22_DATA_PIN1);
		TM_HCSR04_Init(&HCSR04, HCSR_RCC, HCSRPORT, HCSR_ECHO_PIN, HCSRPORT, HCSR_TRIG_PIN); //ultrasound sensor init
		TM_PWM_InitTimer(SERVO_TIM, &servo_timer, 50);			//initialize SERVO_TIM as pwm output with frequency of 50kHZ
		TM_PWM_InitChannel(&servo_timer, SERVO_CHANNEL, SERVO_PINSPACK);
		TM_PWM_SetChannelMicros(&servo_timer, SERVO_CHANNEL, SERVO_MAX);			//set initial postion as closed
		TM_MFRC522_Init(); 		//rfid module init
		USART_Config();
	
		xTaskCreate(space_mapping, "space_mapping_task", STACK_SIZE_MIN, NULL, 2, &HCSRHhandle);
		xTaskCreate(rfid_task, "rfid_check_task", STACK_SIZE_MIN, NULL, 3, &RFIDhandle);
		xTaskCreate(dht_task, "dht task", STACK_SIZE_MIN, NULL, 2, &tHandDHT);
	
		vTaskStartScheduler();
		while(1);
}

/**
*	The DHT task executes periodically every DHT22_REFRESHRATE milliseconds.
*	While the task is reading data from the sensor, which is an extremely time critical operation, the task raises its own priority
*	in order to prevent being halted either by another task or an ISR.
*/

void dht_task(void *prvParams)
{
	while(1)
    {
        bool checksumValid;
        GPIO_SetBits(LEDPORT, LED4PIN);
				
				// For every defined pin, read again, but after get functions.
        vTaskPrioritySet(NULL, 3);
        checksumValid=DHT22_Read(DHT22_DATA_PIN1);
        vTaskPrioritySet(NULL, 2);
				
				// Check whether the reading is valid. If so, send new data.
				// Otherwise send last recorded data.
				if(checksumValid)
				{
        temperature=DHT22getTemperature();
        humidity=DHT22getHumidity();
				}
			
        GPIO_ResetBits(LEDPORT, LED4PIN);
				
				sendDHT22(temperature,humidity);	
				
        vTaskDelay(DHT22_REFRESHRATE/portTICK_RATE_MS);
		}
}

/**
* The space_mapping task executes periodically and performs two actions. It moves the step motor in 180° range. On top of that motor
* there is ultrosonic sensor which reads the distance from the nearest object for every step of the rotation. Step of the rotation is 
* defined in step_motor.h.
**/

void space_mapping(void *prvParameters)
{
	static int i, direction=1;
	static int number=(int) ( gearratio * (ANGLE/ANGLEmin) );
	static float distance, position = 0.0;
	
	//Initial space mapping. Measure distance for every angle in both directions
	//and take the mean value as a measured data. During that time all other tasks
	//are suspended. After initial scan position and direction are set accordingly.
	int positionNum=ANGLEmax/ANGLE;
	float initial_map[positionNum+1];
	
	vTaskSuspendAll ();
	while(position<=ANGLEmax)
	{
		GPIO_SetBits(LEDPORT, LED2PIN);
		initial_map[(int)(position/ANGLE)] = TM_HCSR04_Read(&HCSR04);
		GPIO_ResetBits(LEDPORT, LED2PIN);
		for (i=0;i<number;i++) move_positive();
		position+=ANGLE;
	}
	position = ANGLEmax;
	direction = -1;
	sendInitialMap(&initial_map[0], positionNum);
	xTaskResumeAll ();
	
	while(1)
	{		
			
			GPIO_SetBits(LEDPORT, LED2PIN);
			distance = TM_HCSR04_Read(&HCSR04);
			GPIO_ResetBits(LEDPORT, LED2PIN);
				
			vTaskDelay(10/portTICK_RATE_MS);
			if(abs(distance-initial_map[(int)(position/ANGLE)]) > DISTANCE_ERROR)
				sendDistance(distance,position);
			vTaskDelay(HCSR_REFRESHRATE/portTICK_RATE_MS);
		
			if(direction > 0)
			{
				for(i=0;i<number;i++) move_positive();  //Move the sensor in positive direction.
				position+=ANGLE;									
			}
			else
			{
				for(i=0;i<number;i++) move_negative();  //Move the sensor in negative direction.
				position-=ANGLE;									 
			}
				
			if(position >= ANGLEmax) //In final position change the direction of the motor.
			{
				direction=-direction;
				position=ANGLEmax;   
			}
			else if(position <= 0) //In final position change the direction of the motor.
			{
				direction=-direction;
				position=0;     
			}
	}
}

/**
*	RFID check task performs RFID user check, sends data throug serial and decides whether
* to let the user in( to open a ramp) or not.
*/

void rfid_task(void *prvParameters)
{
	static bool aux_flag=false;
	static uint32_t timerval=0;
	static uint8_t id[5];
	static User newUser;
	static User allUsers[MAX_USER];
	static uint8_t numberOfUsers = 0;

	numberOfUsers = initSuperUser(allUsers);
	writeSuperUser(allUsers);
	
  while(1)
    {
        //checks if card is present
        if(TM_MFRC522_Check(id)==MI_OK){			
            //check if user is valid
            if(isUserValid(&allUsers[0], numberOfUsers, &id[0])){
                GPIO_SetBits(LEDPORT,LED1PIN);
                TM_PWM_SetChannelMicros(&servo_timer, SERVO_CHANNEL, SERVO_MIN); //grant access
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
                TM_PWM_SetChannelMicros(&servo_timer, SERVO_CHANNEL, SERVO_MAX);
                aux_flag=false;
        }
				
        vTaskDelay(RFID_REFRESHRATE/portTICK_RATE_MS);
    }
}
