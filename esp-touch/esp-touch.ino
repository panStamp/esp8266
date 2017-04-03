
/**
 * Copyright (c) 2017 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Narciso Rubio
 * Creation date: 03/31/2017
 */
 
#include "Adafruit_MPR121.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "userdata.h"
#include "buzz.h"

#define LED 2
BUZZ buzz(15);
  
Adafruit_MPR121 touch = Adafruit_MPR121();

WiFiClient espClient;

/* 
 * Variables to control which button has been pressed and released 
 */
uint16_t currTouched = 0;
uint16_t oldTouched = 0;

/*
 * Variable temperature and humidity
 */
 float humidity, temperature;
 
/* 
 * Variables to control the time interval for reporting temperature and humidity 
 */
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;


void setup() 
{
  Serial.begin(38400);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  if (!touch.begin(0x5A)) { // Initialize MPR121 capacitive sensor
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  Serial.println(description);
  WiFi.begin(ssid, password);  // We connect to wifi 
  while (WiFi.status() != WL_CONNECTED) 
  {  
    delay(500);
    Serial.print(".");
    initWebServer();     
  }
  Serial.println("");
  Serial.println("esp-io Web Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  miHumidity();

  mqttConnect(); // We connect mqtt
}

void loop() 
{    
  currentMillis = millis();  
  mqttHandle();  

  if ((unsigned long)(currentMillis - previousMillis) >= sensorPeriodTH)
  {
    temperature = getTemperature();
    mqttPubTemp();
    humidity =  getHumidity();
    mqttPubHum();
    previousMillis = currentMillis;
  }
  
  currTouched = touch.touched();
  if (currTouched != oldTouched)
  {
    for (uint8_t i=0; i<12; i++) 
    {        
      if ((currTouched & _BV(i)) && !(oldTouched & _BV(i))) 
      {
        digitalWrite(LED, LOW);
        buzz.play(i);
        mqttPubButton(i, 1);
        
      }
      if ((oldTouched & _BV(i)) && !(currTouched & _BV(i)))
      {    
        mqttPubButton(i, 0);
      }
    }
    oldTouched = currTouched;
  }
  else if (currTouched == 0)
  {
    digitalWrite(LED, HIGH);  
    buzz.stop();
  }

  httpHandle();
}


