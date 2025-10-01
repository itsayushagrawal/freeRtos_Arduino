/*
1. What is taskYIELD()?
  A FreeRTOS function that tells the scheduler: “I’m done for now, switch to another task at the same priority immediately.”
  Unlike vTaskDelay(), it does not block the task — it simply gives up the CPU for a moment. 
  The task is still READY and will run again when its turn comes.

2. Comparison
  vTaskDelay(ms) → Task is put into Blocked state for a duration. It won’t run again until the delay expires.
  taskYIELD() → Task is still READY, but lets another task of the same priority run right now.
  
*/

#include <Arduino_FreeRTOS.h>

const uint8_t LED1_PIN = 12;
const uint8_t LED2_PIN = 13;

void setup() 
{
  // put your setup code here, to run once:

}

void loop() 
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Starting ");

  // Both tasks have same priority
  xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
  xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);

}

void Task1(void *pvParameters) 
{
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);

  while(1) 
  {
    digitalWrite(LED1_PIN, HIGH);   
    vTaskDelay(pdMS_TO_TICKS(50));
    digitalWrite(LED1_PIN, LOW);

    taskYIELD();
  }
}

void Task2(void *pvParameters)
{
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED2_PIN, LOW);

  while(1) 
  {
    digitalWrite(LED2_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(50));
    digitalWrite(LED2_PIN, LOW);

    taskYIELD();
  }
}

