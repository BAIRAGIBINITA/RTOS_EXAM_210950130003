#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <esp_log.h>

 
TimerHandle_t tmr; 
volatile int state = 0;
uint32_t count =0;
void ISR(TimerHandle_t xTimer)
{
	esp_rom_gpio_pad_select_gpio(GPIO_NUM_2);	
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

	while (1) {
		
		printf("Timer Expire!\n"); 
		gpio_set_level(GPIO_NUM_2, state);
		state = !state;		
		vTaskDelay(pdMS_TO_TICKS(10));
	}	
	if (count == 10)
		{
		printf("Turning off the timer\n");
		xTimerStop(xTimer, 0);
		}  
}
void task1(void*arg)
{    
    while(1)
    {
        printf("Task1 is running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void task2(void*arg)
{    
    while(1)
    {        
        printf("Task2 is running\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
void task3(void*arg)
{    
	       
    printf("Task3 is running\n"); 
	
	tmr = xTimerCreate("tmr", pdMS_TO_TICKS(1000), pdFALSE, 0,ISR); 
	xTimerStart(tmr, pdMS_TO_TICKS(100)); 	
    
	while(1)
	{  	
		vTaskDelay(pdMS_TO_TICKS(10000));
	}
}

void app_main() {

	xTaskCreatePinnedToCore(task1, "task1", 2048, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(task2, "task2", 2048, NULL, 6, NULL, 0);
    xTaskCreatePinnedToCore(task3, "task3", 2048, NULL, 7, NULL, 0);

	
}