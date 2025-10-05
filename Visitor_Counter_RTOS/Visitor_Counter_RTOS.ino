#include <Arduino_FreeRTOS.h>
#include <queue.h>

int current_count=0;

QueueHandle_t sensorQueue;

const uint8_t in_pin =2;
const uint8_t out_pin =3;

void SensorTask(void* pvParameters);
void CountTask(void* pvParameters);
void DisplayTask(void* pvParameters);

void setup() 
{
  Serial.begin(9600);

  pinMode(in_pin, INPUT);
  pinMode(out_pin, INPUT);
  Serial.println("Staring program");

  sensorQueue = xQueueCreate(10, sizeof(int));
  if(sensorQueue == NULL)
  {
    Serial.println("Queue not created");
  }
  else
  {
    Serial.println("Queue created");
  }

  xTaskCreate(SensorTask, "Sensor", 128, NULL, 1, NULL);
  xTaskCreate(CountTask, "Counter", 128, NULL, 1, NULL);
  
  vTaskStartScheduler();

}

void loop() 
{

}

void SensorTask(void* pvParameters)
{
  int direction=0;
  int timeout =500;
  while(1)
  {
    if(digitalRead(in_pin)==1)
    {
      unsigned long startTimeIN= millis();
      while(millis()-startTimeIN < timeout)
      {
        if(digitalRead(out_pin)==1)
        {
          direction = 1;
          xQueueSend(sensorQueue, &direction, portMAX_DELAY);
        }
      }
    }
    if(digitalRead(out_pin)==1)
    {
      unsigned long startTimeOUT= millis();
      while(millis()-startTimeOUT < timeout)
      {
        if(digitalRead(in_pin)==1)
        {
          direction =-1;
          xQueueSend(sensorQueue, &direction, portMAX_DELAY);
        }
      }
    }
  }
}

void CountTask(void* pvParameters)
{
  int direction;
  while(1)
  {
    if (xQueueReceive(sensorQueue, &direction, portMAX_DELAY) == pdPASS) 
    {
      current_count += direction;
      if (current_count < 0)
      { 
        current_count = 0;
      }
      DisplayTask();
    }
  }
}

void DisplayTask()
{
    Serial.println(current_count);
}
