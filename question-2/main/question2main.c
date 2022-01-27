#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <esp_log.h>

SemaphoreHandle_t smf; 
TimerHandle_t tmr; 
volatile int state = 0;
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

void ISR(void *z) {
	int aux = 0;
	xSemaphoreGiveFromISR(smf, &aux); 

	if (aux) {
		portYIELD_FROM_ISR(); 
	}
	while(1)
    {
        printf("Task3 is running\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}


void app_main() {

	xTaskCreatePinnedToCore(task1, "task1", 2048, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(task2, "task2", 2048, NULL, 6, NULL, 0);
    xTaskCreatePinnedToCore(ISR, "ISR", 2048, NULL, 7, NULL, 0);

	smf = xSemaphoreCreateBinary();

	tmr = xTimerCreate("tmr_smf", pdMS_TO_TICKS(10000), true, 0, ISR); 
	xTimerStart(tmr, pdMS_TO_TICKS(100)); 
	
	esp_rom_gpio_pad_select_gpio(GPIO_NUM_2);	
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

	while (1) {
		if (xSemaphoreTake(smf, portMAX_DELAY)) {
			ESP_LOGI("Timer", "Expire!"); 
			gpio_set_level(GPIO_NUM_2, state);
			state = !state;
		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}