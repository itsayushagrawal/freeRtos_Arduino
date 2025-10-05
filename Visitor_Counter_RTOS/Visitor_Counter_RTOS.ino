/*
Assuming there are 3 independent rooms and a sensor node is installed at the only door
*/


#include <Arduino_FreeRTOS.h>
#include <queue.h>

int room_count[3] = {0,0,0};

QueueHandle_t SensorQueue;

const int room_pin[3] ={2,4,6};

struct SensorEvent
{
  int room_id;
  int direction;
};

void SensorTask(void* pvParameters);
void CountTask(void* pvParameters);

void DisplayTask(void DisplayCount(int id));

void setup() 
{
  Serial.begin(9600);

  SensorQueue = xQueueCreate(10, sizeof(SensorEvent));
  if(SensorQueue == NULL)
  {
    Serial.println("Queue not created");
  }
  else
  {
    Serial.println("Queue created");
  }

  for(int i=0;i<3;i++)    //Creating separate task for each room
  {
    xTaskCreate(SensorTask, "Room", 128, &room_pin[i], 1, NULL);
  }
  xTaskCreate(CountTask, "Counter", 128, NULL, 1, NULL);
  
  vTaskStartScheduler();

}

void loop() 
{

}

void SensorTask(void* pvParameters)
{
  SensorEvent evt;
  int in_pin = *(int *)pvParameters;
  int out_pin = in_pin + 1;
  evt.room_id= (in_pin /2)-1;   // maps 2->0  4->1   6->2
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
          evt.direction = 1;
          xQueueSend(SensorQueue, &evt, portMAX_DELAY);
          break;
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
          evt.direction = -1;
          xQueueSend(SensorQueue, &evt, portMAX_DELAY);
          break;
        }
      }
    }
  }
}

void CountTask(void* pvParameters)
{
  SensorEvent evt;
  while(1)
  {
    if (xQueueReceive(SensorQueue, &evt, portMAX_DELAY) == pdPASS) 
    {
      room_count[evt.room_id] += evt.direction;
      if (room_count[evt.room_id] <0)
      { 
        room_count[evt.room_id]=0;
      }
      DisplayCount(evt.room_id);
    }
  }
}

void DisplayCount(int id)
{
  Serial.print("Room "); Serial.print(id); Serial.print(": "); Serial.println(room_count[id]);
}
