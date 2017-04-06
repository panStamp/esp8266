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
  client.setServer(MQTT_BROKER, MQTT_PORT);
}

/*
 * mqttReconnect
 * 
 * Function with loop until we're reconnected
 */
void mqttReconnect(void)
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection..."); // Attempt to connect
    if (client.connect(description)) // Anonymous connection to broker
    //if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) // Authenticated connection with broker
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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
 * mqttPubButton
 * 
 * Publish Button via MQTT
 * 
 * @param button number
 * @param values 0-button not touched 1-button touched
 */
void mqttPubButton( uint8_t numberButton, uint8_t touch)
{
  char tpc[32];
  char msg[4] = "ON";
  if (touch == 0)
  {
    strcpy(msg,"OFF");
  }
  sprintf(tpc, "%s%s%d", MQTT_TOPIC_MAIN, "/button",numberButton); 
  client.publish(tpc, msg);

}
/*
 * mqttPubTemp
 * 
 * Publish temperature via MQTT
 */
void mqttPubTemp(void)
{
  char tpc[32];
  sprintf(tpc, "%s%s", MQTT_TOPIC_MAIN, "/temperature");
  
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
  char tpc[32];
  sprintf(tpc, "%s%s", MQTT_TOPIC_MAIN, "/humidity");
  
  int n1 = int(humidity);
  int n2 = (humidity - n1) * 100;
  char hum[32];
  sprintf (hum, "%i%s%i %s", n1, ".", n2, "%"); 
  
  client.publish(tpc, hum);
}
