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
 
#include <PubSubClient.h>

/*
 * MQTT client
 */
PubSubClient client(espClient);

/*
 * mqttConnect
 * 
 * Function connecting to mqtt
 */
void mqttConnect(void)
{
  int port = atoi(config.mqttPort);
  
  client.setServer(config.broker, port);
}

/*
 * mqttReconnect
 * 
 * Function with loop until we're reconnected
 */
void mqttReconnect(void)
{
  uint8_t attempts = 0; 
  while (!client.connected() && !enterApMode)
  {
    #ifdef DEBUG_ENABLED
    Serial.print("Attempting MQTT connection..."); // Attempt to connect
    #endif
    if (client.connect(description)) // Anonymous connection to broker
    //if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) // Authenticated connection with broker
    { 
      #ifdef DEBUG_ENABLED
      Serial.println("connected");
      #endif
    }
    else
    {
      #ifdef DEBUG_ENABLED
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      #endif

      attempts++;
      if(attempts == 5)
        enterApMode = true;
        
      
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}

/* 
 * mqttHandle
 * 
 * Handle MQTT client connectivity
 */
void mqttHandle(void)
{
  if (!client.connected())
    mqttReconnect();
        
  client.loop();
}

/*
 * getTemperature
 * 
 * Read temeperature value from HTU sensor
 */
float getTemperature(void)
{
  return myHumidity.readTemperature();
}

/*
 * getHumidity
 * 
 * Read humidity value from HTU sensor
 */
float getHumidity(void)
{
  return myHumidity.readHumidity();
}

/*
 * mqttPubStateButton
 * 
 * Publish Button ON or OFF via MQTT
 * 
 * @param button number
 * @param values 0-button not touched 1-button touched
 */
void mqttPubStateButton( uint8_t numberButton, uint8_t touch)
{
  char tpc[64];
  char msg[4] = "ON";
  if (touch == 0)
  {
    strcpy(msg,"OFF");
  }

  if (numberButton == 65 || numberButton== 66)
    sprintf(tpc, "/%s/esp-touch/%s/button/%s", config.topicMain, deviceKey, String(char(numberButton)).c_str());
  else
    sprintf(tpc, "/%s/esp-touch/%s/button/%d", config.topicMain, deviceKey, numberButton);
       
  client.publish(tpc, msg);

}

/*
 * mqttPubPressedButton
 * 
 * Publish Button pressed via MQTT
 * 
 * @param button number
 */
void mqttPubPressedButton( uint8_t numberButton)
{
  char tpc[64];  
  sprintf(tpc, "/%s/esp-touch/%s/keypad", config.topicMain, deviceKey);

  if (numberButton == 65 || numberButton== 66) 
    client.publish(tpc, String(char(numberButton)).c_str());        
  else
    client.publish(tpc, String(numberButton).c_str());  
}

/*
 * mqttPubTemp
 * 
 * Publish temperature via MQTT
 */
void mqttPubTemp(void)
{
  char tpc[64];
  sprintf(tpc, "/%s/esp-touch/%s/temperature", config.topicMain, deviceKey);
  
  int n1 = int(temperature);
  int n2 = (temperature - n1) * 100;
  char temp[32];
  sprintf (temp, "%i%s%i %s", n1, ".", n2, "C");
  
  client.publish(tpc, temp);
}

/**
 * mqttPubHum
 * 
 * Publish humidity via MQTT
 */
void mqttPubHum(void)
{
  char tpc[64];
  sprintf(tpc, "/%s/esp-touch/%s/humidity", config.topicMain, deviceKey);
  
  int n1 = int(humidity);
  int n2 = (humidity - n1) * 100;
  char hum[32];
  sprintf (hum, "%i%s%i %s", n1, ".", n2, "%"); 
  
  client.publish(tpc, hum);
}
