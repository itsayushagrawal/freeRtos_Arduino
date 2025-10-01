#include <Arduino_FreeRTOS.h>

const uint8_t Led1Pin =12;
const uint8_t Led2Pin =13;

TaskHandle_t Task1Handle;
TaskHandle_t Task2Handle;

void Task1(void * pvParameters);
void Task2(void * pvParameters);
void ChangePriorityTask(void * pvParameters);

void setup() 
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Starting now");

  xTaskCreate(Task1, "Led1 Task", 128, NULL, 1, &Task1Handle);
  xTaskCreate(Task2, "Led2 Task", 128, NULL, 1, &Task2Handle);

  xTaskCreate(ChangePriorityTask, "Changer", 128, NULL, 2, NULL);
}

void loop() 
{
  // put your main code here, to run repeatedly:

}

void Task1(void * pvParameters)
{
  pinMode(Led1Pin, OUTPUT);
  digitalWrite(Led1Pin, LOW);
  while(1)
  {
    digitalWrite(Led1Pin, digitalRead(Led1Pin)^1);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void Task2(void * pvParameters)
{
  pinMode(Led2Pin, OUTPUT);
  digitalWrite(Led2Pin, LOW);
  while(1)
  {
    digitalWrite(Led2Pin, digitalRead(Led2Pin)^1);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void ChangePriorityTask(void * pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(5000));

  Serial.println("Chnaging Task 2 Priority to higher");

  UBaseType_t priority =uxTaskPriorityGet(Task2Handle);
  Serial.print("Task 2 current priority: ");
  Serial.println(priority);

  vTaskPrioritySet(Task2Handle, 3);

  priority = uxTaskPriorityGet(Task2Handle);
  Serial.print("Task 2 New priority: ");
  Serial.println(priority);

  vTaskDelete(NULL);
}


