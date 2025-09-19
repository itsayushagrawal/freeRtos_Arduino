#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t dataQueue;

const int analogPin = A0;

void readAnalogTask(void *pvParameters);
void printTask(void *pvParameters);

void setup() {
  Serial.begin(9600);
  dataQueue = xQueueCreate(10, sizeof(int));

  if (dataQueue == NULL) 
  {
    Serial.println("Error creating the queue");
    while (1);
  }

  xTaskCreate(readData, "ReadAnalog", 128, NULL, 1, NULL);
  xTaskCreate(printData, "PrintData", 128, NULL, 1, NULL);
}

void loop() 
{

}

void readData(void *pvParameters) 
{
  int sensorValue;

  while(1) 
  {
    sensorValue = analogRead(analogPin);
    xQueueSend(dataQueue, &sensorValue, (TickType_t)10);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void printData(void *pvParameters) 
{
  int receivedValue;
  while(1) 
  {
    if (xQueueReceive(dataQueue, &receivedValue, portMAX_DELAY) == pdPASS) 
    {
      Serial.print("Analog Value: ");
      Serial.println(receivedValue);
    }
  }
}
