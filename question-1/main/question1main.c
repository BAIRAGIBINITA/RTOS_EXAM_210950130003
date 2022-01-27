#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

QueueHandle_t buffer;
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
    while(1)
    {
        printf("Task3 is running\n");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
void task4(void*arg)
{
    uint32_t send = 0;
    while(1)
    {
        if (send < 15)
        {
            xQueueSend(buffer, &send, pdMS_TO_TICKS(0));
            send++;
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(5000));
            send = 0;
        }
        printf("data waiting to be read : %d\r\n",uxQueueMessagesWaiting(buffer));
        printf("available spaces : %d\r\n",uxQueueSpacesAvailable(buffer));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void task5(void*z)
{
    uint32_t receive = 0;
    while(1)
    {
        if (xQueueReceive(buffer, &receive, pdMS_TO_TICKS(1000)) == true)
        {
            printf("DATA received: %u\n", receive);
        }
        else
        {
            printf("DATA not received, timeout!\n");
        }
    }
}

void app_main()
{
    buffer = xQueueCreate(10, sizeof(uint32_t));

    xTaskCreatePinnedToCore(task1, "task1", 2048, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(task2, "task2", 2048, NULL, 6, NULL, 0);
    xTaskCreatePinnedToCore(task3, "task3", 2048, NULL, 7, NULL, 0);
    xTaskCreatePinnedToCore(task4, "task4", 2048, NULL, 8, NULL, 0);
    xTaskCreatePinnedToCore(task5, "task5", 2048, NULL, 9, NULL, 0);
    
}