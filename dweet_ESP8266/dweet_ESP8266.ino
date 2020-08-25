#include <ESP8266WiFi.h>
#include <Wire.h>

#define _SSID "TP-Link_6A88" 
#define _PASSWORD "07741203" 

#define ADDR 55

#define SCL D2
#define SDA D1

#define ERROR_SIGN_PIN D3

int8_t  dataI2C [4] = {-1,-1,-1,-1};
bool errorFlag = false;

enum dataVal{temperature,humidity,photoResistor,pot};

WiFiClient esp; 
String name = "Test_0";

void setup(void) { 

  Serial.begin(9600);  

  pinMode(ERROR_SIGN_PIN,OUTPUT);
  WiFi.begin(_SSID,_PASSWORD); // подключение ESP к WiFi 
  Serial.println("https://dweet.io/follow/" + name); //где смотреть данные   

  Wire.begin(SDA,SCL);  // Инициализируем I2C 
  
  while (WiFi.status() != WL_CONNECTED) // ожидания подключения к WiFi
  { 
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
}

void loop(void) { 

    Wire.requestFrom(ADDR,sizeof(dataI2C));
    if(Wire.available())
    {
      for(uint8_t i = 0; i < sizeof(dataI2C);i++)
      {
        dataI2C[i]= Wire.read();
      }
    }
    else
    {
      Serial.println("I2C No signal");
    }
  
     for(uint8_t i = 0; i < sizeof(dataI2C);i++)
      {
        if (dataI2C[i] < 0)
        {
          Serial.print(F("Error value "));
          Serial.print(dataI2C[i]);
          Serial.print(F(" number of parameter "));
          Serial.println(i);
          errorFlag = true;
        }
        if (i == (sizeof(dataI2C)-1))
        {
          if(errorFlag)
          {
             digitalWrite(ERROR_SIGN_PIN,true);
             errorFlag = false;
          }
          else
          {
            digitalWrite(ERROR_SIGN_PIN,false);
          }
        }
      }
      
     Serial.print(dataI2C[temperature]);
     Serial.print('\t');
     Serial.print(dataI2C[humidity]);
     Serial.print('\t');
     Serial.print(dataI2C[photoResistor]);
     Serial.print('\t');
     Serial.println(dataI2C[pot]);

    
    
   if (esp.connect("www.dweet.io", 80)) // делаем запрос на www.dweet.io , если удалось поключится к серверу
   { 
     String internetRequest = "GET /dweet/for/" + name + "?"; // создаем строку запроса  
     internetRequest += "Temperature=" + String(dataI2C[temperature]) ;  // формируем команду для отправки информации для температуры
     internetRequest += "&Humidity=" + String(dataI2C[humidity]);
     internetRequest += "&Photoresistor=" + String(dataI2C[photoResistor]);
     internetRequest += "&Potentiometer=" + String(dataI2C[pot]);
     internetRequest += " HTTP/1.1\r\n";    
     internetRequest += "Host: dweet.io\r\n\r\n";
     esp.println(internetRequest); // отправляем
   
//инфомация с датчиков в порт

     Serial.print(internetRequest);
       
  }
  else //если неудалось поключится к серверу
  {
    Serial.println("dweet io no conection");
  }
 
  delay(5000); 
}
