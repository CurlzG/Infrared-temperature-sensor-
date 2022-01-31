/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-http-get-open-weather-map-thingspeak-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  Code compatible with ESP8266 Boards Version 3.0.0 or above 
  (see in Tools > Boards > Boards Manager > ESP8266)
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Adafruit_MLX90614.h>
#include <FirebaseArduino.h>
#include <Wire.h>
#include <Arduino_JSON.h>
#define FIREBASE_HOST "FIREBASE HOST "
#define FIREBASE_AUTH "FIREBASE AUTH"
const char* ssid = "WIFI NAME";
const char* password = "WIFI PASSWORD";
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
const int capacity = JSON_OBJECT_SIZE(12);
StaticJsonBuffer<capacity> jb;
JsonObject& obj = jb.createObject();
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

String jsonBuffer;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

 mlx.begin();
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
int n = 0;
String text = "Weather";
String textT = "WeatherCheck";
String check = "-";
String go = "-";
void loop() {
check =  Firebase.getString("Check");
go = Firebase.getString("Go");
//When the user sets it to auto
if(check == "Auto"){
String blank;
  Serial.print(mlx.readObjectTempC());
  obj["Concrete"] = mlx.readObjectTempC();

    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "API LINK HERE";
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      //Serial.print("JSON object = ");
     // 
      //Serial.print("Temperature: ");
      int temp = myObject["main"]["temp"];
      int hum  = myObject["main"]["humidity"];
      int tempMax = myObject["main"]["temp_max"];
      int feelsLike = myObject["main"]["feels_like"];
      String wea = JSON.stringify(myObject["weather"][0]["main"]);
      int clouds = myObject["clouds"]["all"];
      long ti = myObject["dt"];
      double windspeed = myObject["wind"]["speed"];
      obj["Temp"] = temp;
      obj["Humidity"] = hum;
      obj["TempMax"] = tempMax;
      obj["FeelsLike"] = feelsLike;
      obj["Weather"] = wea;
      obj["Clouds"] = clouds;
      obj["WindSpeed"] = windspeed;
      obj["Time"] = ti;
    }
    String text1 = text + n;
   Firebase.set(text1,obj);
    delay(20*60*1000);
    n++;
}else {
  if(go == "Now"){
  Serial.print(mlx.readObjectTempC());
  obj["Concrete"] = mlx.readObjectTempC();
  
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "API LINK ";
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      //Serial.print("JSON object = ");
     // 
      //Serial.print("Temperature: ");
      int temp = myObject["main"]["temp"];
      int hum  = myObject["main"]["humidity"];
      int tempMax = myObject["main"]["temp_max"];
      int feelsLike = myObject["main"]["feels_like"];
      String wea = JSON.stringify(myObject["weather"][0]["main"]);
      long ti = myObject["dt"];
      int clouds = myObject["clouds"]["all"];
      double windspeed = myObject["wind"]["speed"];
      obj["Temp"] = temp;
      obj["Humidity"] = hum;
      obj["TempMax"] = tempMax;
      obj["FeelsLike"] = feelsLike;
      obj["Weather"] = wea;
      obj["Clouds"] = clouds;
      obj["WindSpeed"] = windspeed;
      obj["Time"] = ti;
    }
   Firebase.set(textT,obj);
   Firebase.set("Go","No");
    delay(10000);
  }
}
   
  //delay(1*60*1000);
}
//Copy and pasted from somewhere else not my code
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

return payload;
}
