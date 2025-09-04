#include <Arduino_FreeRTOS.h>

#define redLed 7
#define greenLed 8

void setup() 
{
  Serial.begin(9600);

  xTaskCreate(redLedTask, "red Led Task", 128, NULL, 1, NULL);
  xTaskCreate(greenLedTask, "green Led Task", 128, NULL, 2,NULL);
  
  vTaskStartScheduler();
}

void loop()
{
    // This remains empty
}

void redLedTask(void *pvParameters)
{
  pinMode(redLed, OUTPUT);
  
  while(1)
  {
    digitalWrite(redLed, HIGH);
    vTaskDelay(10);
    digitalWrite(redLed, LOW);
    vTaskDelay(10);
    Serial.println("Red Led Task Running");
  }
}

void greenLedTask(void *pvParameters)
{
  pinMode(greenLed, OUTPUT);
  while(1)
  {
    digitalWrite(greenLed, HIGH);
    vTaskDelay(10);
    digitalWrite(greenLed, LOW);
    vTaskDelay(10);
    Serial.println("Green Led Task Running");
  }
}
