/*
  Normally, each task needs its own function.
  But FreeRTOS lets you reuse the same function for many tasks by passing a parameter pointer when creating the task.
  Inside the task, you read this pointer (cast it to the right type) → each task can behave differently.
  Example: You can create one “blink task function” but tell one instance to blink LED1, and another to blink LED2, just by passing the pin number as a parameter.

We can pass any number of variables for a task by passing pointer to a array or struct in pvParameter

void * -> generic pointer, can be used to pass any type of data
*/

#include <Arduino_FreeRTOS.h>

#define Red_LED 6
#define Green_LED 7
#define Yellow_LED 8

void LED_Blink(void * pvParameters)
{
  uint8_t pin= (uint8_t)pvParameters;
  pinMode(pin, OUTPUT);;
  while(1)
  {
    digitalWrite(pin, digitalRead(pin)^1);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setup() 
{
  xTaskCreate(LED_Blink, "Red LED", 128, Red_LED, 1, NULL);
  xTaskCreate(LED_Blink, "Green LED", 128, Green_LED, 2, NULL);
  xTaskCreate(LED_Blink, "Yellow LED", 128, Yellow_LED, 3, NULL);
  vTaskStartScheduler();
}

void loop() 
{

}
