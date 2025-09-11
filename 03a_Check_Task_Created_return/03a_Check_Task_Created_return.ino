/*
When you call xTaskCreate(), it tries to allocate memory and register the new task with the RTOS kernel.
  If successful → it returns pdPASS.
  If it fails → it returns errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY (internally, just not pdPASS).

Why can it fail?
  Not enough heap memory for the task’s stack.
  Stack size too small (task crashes later).
  Too many tasks already created (depends on system).
*/

#include <Arduino_FreeRTOS.h>

void LED_Blink(void* pvParameters)
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

  BaseType_t result = xTaskCreate(LED_Blink, "Blink LED", 128, NULL, 1, NULL);

  if(result != pdPASS)
    Serial.println("Task Creation Failed");
  else
  {
    Serial.println("Task Created Successfully");
    vTaskStartScheduler();
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
