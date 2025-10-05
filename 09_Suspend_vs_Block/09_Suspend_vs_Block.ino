/*
Blocking (vTaskDelay) is temporary and automatically recovers.
Suspending (vTaskSuspend) is indefinite until another task explicitly wakes it up.

*/


#include <Arduino_FreeRTOS.h>

const uint8_t LedBlockPin = 12;
const uint8_t LedSuspendPin =13;

TaskHandle_t SuspendHandle;

void TaskBlock(void* pvParameters);
void TaskSuspend(void* pvParanmeters);
void ControllerTask(void* pvParameters);

void setup() 
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Starting program ");

  xTaskCreate(TaskBlock, "Block", 128, NULL, 1, NULL);
  xTaskCreate(TaskSuspend, "Suspend", 128, NULL, 1, &SuspendHandle);
  xTaskCreate(ControllerTask, "Controller", 128, NULL, 2, NULL);
}

void loop() 
{
  // put your main code here, to run repeatedly:

}

void TaskBlock(void* pvParameters)
{
  pinMode(LedBlockPin, OUTPUT);
  while(1)
  {
    digitalWrite(LedBlockPin, digitalRead(LedBlockPin)^1);
    vTaskDelay(pdMS_TO_TICKS(100));   //blocked for 100ms
  }
}

void TaskSuspend(void* pvParanmeters)
{
  pinMode(LedSuspendPin, OUTPUT);
  uint8_t count=500;
  while (1) 
  {
    while(count--)      //runs for 500 cycles
    {
      digitalWrite(LedSuspendPin, digitalRead(LedSuspendPin)^1);
    }
  
    vTaskSuspend(NULL);   //task suspended itself, will not continue until another task continues it
  }
}

void ControllerTask(void* pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(5000)); // delay for 5 sec
  vTaskResume(SuspendHandle);
  vTaskDelete(NULL);
}