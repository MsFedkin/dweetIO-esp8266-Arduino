#include <Wire.h> // I2C Библиотека
#include <dht11.h>

#define DHT_PIN 26 //5
#define POT_PIN 2//A0
#define PR_PIN 4//A1

dht11 dht;

uint8_t  dataI2C [4]; // массив данных
enum dataVal{temperature,humidity,photoResistor,pot}; // перечисление 

void setup() {
  Serial.begin(9600);
  Wire.begin(55); // задаем I2C адресс 55
  Wire.onRequest(requestEvent); // регистрируем запрошенное событие 
}

void loop() {
    dht.read(DHT_PIN);
   // delay(100);

}

void requestEvent () // функция для передачи данных по I2C
{
    //dht.read(DHT_PIN);
    dataI2C [temperature] = dht.temperature;
    dataI2C [humidity] = dht.humidity;
    dataI2C [photoResistor] = analogRead(PR_PIN)/10.24f;
    dataI2C [pot] = analogRead(POT_PIN)/10.24f;
    
  for(uint8_t i = 0; i < sizeof(dataI2C);i++)
  {
    Wire.write(dataI2C[i]);
  }  
}
