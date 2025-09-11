/*
🕐 vTaskDelay(ticks)
Think of it as: “sleep for X time from now.”
Example: If you call vTaskDelay(1000), the task pauses for 1000 ticks starting from the moment you called it.
Problem: If your task takes some extra time before calling delay, the actual timing drifts.

⏱️ vTaskDelayUntil(&lastWakeTime, ticks)
Think of it as: “wake up at fixed intervals, no matter how long the code took (as long as it’s less than the interval).”
You give it a reference time (lastWakeTime) and an interval.
FreeRTOS automatically calculates the next exact wake-up time.
Benefit: Keeps tasks running periodically (like every 1 second) without drift.
*/

/*
 * The xLastWakeTime variable needs to be initialized with the current tick
 * count. Note that this is the only time the variable is written to
 * explicitly. After this xLastWakeTime is automatically updated within
 * vTaskDelayUntil().
 */

#include <Arduino_FreeRTOS.h>

void periodicTask(void *pvParameters)
{
  pinMode(LED_BUILTIN, OUTPUT);
  TickType_t LastWakeTime = xTaskGetTickCount();
  while(1)
  {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN)^1);
    Serial.println("Task Running");
    vTaskDelayUntil(&LastWakeTime, pdMS_TO_TICKS(500));
  }

}

void setup() 
{
  xTaskCreate(periodicTask, "Periodic Task 1", 128, NULL, 1, NULL);
  Serial.begin(9600);
  vTaskStartScheduler();
}

void loop() 
{

}
