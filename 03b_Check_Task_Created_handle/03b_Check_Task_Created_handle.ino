/*
What is a Task Handle?
  A task handle (TaskHandle_t) is like an ID card or reference to a task in FreeRTOS.
  When you create a task using xTaskCreate(), you can ask FreeRTOS to give you back a handle.
  This handle uniquely identifies that task inside the RTOS.

Why do we need it?

  1.Some tasks run forever, but sometimes you need to control them:
      Suspend / Resume → pause and restart a task.
      Delete → completely remove a task.
      Check state → find if it’s running, suspended, etc.

  2.The handle tells FreeRTOS which task you are talking about.
    Without a handle, you can’t specifically target a task later.

Type: TaskHandle_t (usually a pointer inside RTOS).
Created by: xTaskCreate(..., &myHandle);
If the task creation fails, the handle stays NULL.
*/

#include <Arduino_FreeRTOS.h>

TaskHandle_t blink_handle = NULL;

void LED_Blink(void *pvParameters)
{
  pinMode(LED_BUILTIN, OUTPUT);
  while(1)
  {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN)^1);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setup() 
{
  Serial.begin(9600);
  xTaskCreate(LED_Blink, "Blink LED", 128, NULL, 1, &blink_handle);
  
  if(blink_handle != NULL)
  {
    Serial.println("task created successfully");
    vTaskStartScheduler();
  }
  else
    Serial.println("task creation failed");
    
}

void loop()
{

}
