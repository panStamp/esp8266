
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
#include <EEPROM.h>
#include "userdata.h"
#include "buzz.h"

/**
 * Debug option
 */
#define DEBUG_ENABLED  1

#define LED 2
BUZZ buzz(15);
  
Adafruit_MPR121 touch = Adafruit_MPR121();

WiFiClient espClient;

// If true, enter Wifi AP mode
volatile bool enterApMode = false;
bool  connectApMode = false;

/**
 * User config
 */
USERDATA config;

char deviceId[256];

// Unique device key based on MAC
char deviceKey[WL_MAC_ADDR_LENGTH * 2 + 1]; 

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



/**
 * enterWifiApMode
 * 
 * Enter WiFi AP mode
 */
void enterWifiApMode(void)
{
  enterApMode = true;
}

void setup() 
{
  #ifdef DEBUG_ENABLED
  Serial.begin(38400);
  #endif
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  // ESP8266 GPIO0 pin
  pinMode(0, INPUT_PULLUP);
  attachInterrupt(0, enterWifiApMode, FALLING);

  // Initialize config (pseudo-EEPROM) space
  config.begin();

  // Append last two bytes of the MAC to the device ID
  //WiFi.begin();
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);

  char buf[3];
  // Set Device key from MAC address
  memset((uint8_t*)deviceKey, 0, sizeof(deviceKey));

  for(int i=0; i<WL_MAC_ADDR_LENGTH ; i++)
  {
    if (mac[i] < 0x10)
      sprintf(buf, "0%X", mac[i]);
    else
      sprintf(buf, "%X", mac[i]);

    deviceKey[i*2] = buf[0];
    deviceKey[i*2+1] = buf[1];
  }

  // Set device ID
  sprintf(deviceId, "%s %s", apName, deviceKey);

  // Initialize MPR121 capacitive sensor
  if (!touch.begin(0x5A)) {
    #ifdef DEBUG_ENABLED
    Serial.println("MPR121 not found, check wiring?");
    #endif
    
    while (1);
  }
  #ifdef DEBUG_ENABLED
  Serial.println("MPR121 found!");
  #endif

  // Read config from EEPROM
  if (config.readConfig())
  {
    // We connect to wifi   
    WiFi.begin(config.ssid, config.password);

    uint8_t attempts = 0;
    while (WiFi.status() != WL_CONNECTED && !enterApMode) 
    {  
      delay(500);
      #ifdef DEBUG_ENABLED
      Serial.print(".");
      #endif
      attempts++;

      if(attempts == 20)
        enterApMode = true;
    }

    if(!enterApMode)
    {
      #ifdef DEBUG_ENABLED
      Serial.println("");
      Serial.println(deviceId);
      Serial.println("esp-touch Web Server");
      Serial.print("Connected to ");
      Serial.println(config.ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      #endif        

      // We connect mqtt
      mqttConnect();

      if(!enterApMode)
      { 
        connectApMode = false;
         
        touch.setThresholds(15, 36);
        initWebServer();
      }
    }
  }
  else
    enterApMode = true;
}

void loop() 
{
  if (enterApMode)
  {
    enterApMode = false;
    connectApMode = true; 
    initWebServer();
         
    WiFi.mode(WIFI_AP);  
    WiFi.softAP(deviceId, apPassword);
    
    digitalWrite(LED, LOW);  
     
    #ifdef DEBUG_ENABLED
    Serial.println("Entering AP mode");
    #endif    
  }
  
  if(!connectApMode)
  {
    digitalWrite(LED, HIGH); 
    currentMillis = millis();  
  
    if ((unsigned long)(currentMillis - previousMillis) >= (atoi(config.txIntervalTH))*1000)
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
          uint8_t KeyButton = keypadButton(i);
          digitalWrite(LED, LOW);
          buzz.play(i);
          mqttPubPressedButton(KeyButton);
          mqttPubStateButton(KeyButton, 1);        
          
        }
        if ((oldTouched & _BV(i)) && !(currTouched & _BV(i)))
        {  
          uint8_t KeyButton = keypadButton(i);  
          mqttPubStateButton(KeyButton, 0);
        }
      }
      oldTouched = currTouched;
    }
    else if (currTouched == 0)
    {
      digitalWrite(LED, HIGH);  
      buzz.stop();
    }

    mqttHandle();  
  }
  
  httpHandle();
}

/*
 * keypadButton
 * 
 * Assign number in keypad
 * 
 * @param button number 
 * 
 * @return number keypad
 */
uint8_t keypadButton( uint8_t touchedButton)
{
  uint8_t button;
  switch(touchedButton)
  {
    case 0:
       button = 7;
       break;
    case 1:
       button = 4;
       break;
    case 2:
       button = 1;
       break;
    case 3:
       button = 65;
       break;
    case 4:
       button = 8;
       break;
    case 5:
       button = 5;
       break;
    case 6:
       button = 2;
       break;
    case 7:
       button = 0;
       break;
    case 8:
       button = 9;
       break;
    case 9:
       button = 6;
       break;
    case 10:
       button = 3;
       break;
    case 11:
       button = 66;
                                                                            
  }
  return button;
}


