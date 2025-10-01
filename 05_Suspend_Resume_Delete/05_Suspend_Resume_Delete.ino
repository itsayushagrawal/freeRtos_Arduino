/*
In FreeRTOS, once a task is created, it runs forever unless you manage its lifecycle:
  Suspend (vTaskSuspend(handle))
    Temporarily stops a task.
    Suspended task does not run until explicitly resumed.

  Resume (vTaskResume(handle))
    Brings a suspended task back into the scheduler.

  Delete (vTaskDelete(handle))
    Permanently removes a task from the system.
    Frees its stack and memory.

If a task deletes itself (vTaskDelete(NULL)), it never runs again.

This is useful for tasks you only need sometimes (e.g., start a task when a sensor is active, stop it otherwise).

We’ll create:
  Task A (Blink Task) → Blinks LED.
  Task B (Controller Task) → Suspends Task A after 5s, resumes after 5s, and then deletes it.
*/

#include <Arduino_FreeRTOS.h>

TaskHandle_t blink_handle;

void blinkTask(void *pvParameters) 
{
  pinMode(LED_BUILTIN, OUTPUT);
  while (1) 
  {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN)^1);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void controlTask(void *pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(5000));
  Serial.println("Suspending blink task");
  vTaskSuspend(blink_handle);

  vTaskDelay(pdMS_TO_TICKS(5000));
  Serial.println("Resuming Blink Task");
  vTaskResume(blink_handle);

  vTaskDelay(pdMS_TO_TICKS(5000));
  Serial.println("Deleting Blink Task");
  vTaskDelete(blink_handle);

  Serial.println("Control task complete, deleting itself");
  vTaskDelete(NULL);  //NULL used to suspend/delete itself

}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  xTaskCreate(blinkTask, "Blink", 128, NULL, 1, &blink_handle);
  xTaskCreate(controlTask, "Control", 128, NULL, 2, NULL);  //Higher priority so that it is not pre empted by Blink Task

  vTaskStartScheduler();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
