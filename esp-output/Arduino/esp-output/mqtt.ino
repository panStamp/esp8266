/**
 * Copyright (c) 2016 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 01/02/2016
 */

#include <PubSubClient.h>

// Last time this gateway sent a periodic heartbeat
uint32_t hBeatTime = 0;

/**
 * MQTT connection
 */
#define mqtt_user "your_username"
#define mqtt_password "your_password"

#define MAX_MQTT_TOPIC_LENGTH  128   // Max topic length
char MQTT_TOPIC_NETWORK[MAX_MQTT_TOPIC_LENGTH];
char MQTT_TOPIC_CONTROL[MAX_MQTT_TOPIC_LENGTH];
char MQTT_TOPIC_SYSTEM[MAX_MQTT_TOPIC_LENGTH];

// MQTT client
PubSubClient client(espClient);

/**
 * Initialize MQTT topics
 */
void initTopics(void)
{
  sprintf(MQTT_TOPIC_NETWORK, "%s/%s/network", MQTT_TOPIC_MAIN, deviceKey);
  sprintf(MQTT_TOPIC_CONTROL, "%s/%s/control", MQTT_TOPIC_MAIN, deviceKey);
  sprintf(MQTT_TOPIC_SYSTEM, "%s/%s/system", MQTT_TOPIC_MAIN, deviceKey);
}

/**
 * Connect to MQTT broker
 */
void mqttConnect(void)
{
  initTopics();
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(mqttReceive);
}

/**
 * Reconnect to MQTT broker
 */
void mqttReconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(deviceId)) // Anonymous connection to broker
    //if (client.connect(deviceId, mqtt_user, mqtt_password)) // Authenticated connection with broker
    {
      Serial.println("connected");

      client.publish(MQTT_TOPIC_SYSTEM, "CONNECTED");
      
      // Subscribe to the following topics
      char topic[MAX_MQTT_TOPIC_LENGTH];
      sprintf(topic, "%s/#", MQTT_TOPIC_CONTROL);
      client.subscribe(topic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      digitalWrite(LED, HIGH);
      delay(2500);
      digitalWrite(LED, LOW);
      delay(2500);
    }
  }
}

/**
 * mqttHandle
 * 
 * Handle MQTT client connectivity
 */
void mqttHandle(void)
{
  if (!client.connected())
    mqttReconnect();
  
  uint32_t currentTime = millis();
  if (currentTime - hBeatTime > hBeatPeriod)
  {
    client.publish(MQTT_TOPIC_SYSTEM, "RUNNING");
    hBeatTime = currentTime;
  }
  
  client.loop();
}

/**
 * mqttReceive
 * 
 * Function called whenever a MQTT packet is received through MQTT_TOPIC_CONTROL
 * 
 * @param topic MQTT topic
 * @param payload message
 * @param len message length
 */
void mqttReceive(char* topic, byte* payload, unsigned int len)
{
  // Check topic
  if (!strncmp(topic, MQTT_TOPIC_CONTROL, strlen(MQTT_TOPIC_CONTROL)))
  {
    // Check topic length
    if (strlen(topic) == strlen(MQTT_TOPIC_CONTROL) + 2)
    {
      // Get output number
      char *ptr = topic + strlen(MQTT_TOPIC_CONTROL) + 1;
      int outputNb = atoi(ptr);

      // Read payload and drive output
      if (!strncmp((char*)payload, "0", len))
        driveOutput((uint8_t)outputNb, LOW);
      else if (!strncmp((char*)payload, "1", len))
        driveOutput((uint8_t)outputNb, HIGH);
    }
  }
}

/**
 * mqttSendStatus
 * 
 * Send status through the network topic
 * 
 * @param output Output number
 * @param state Output state
 */
void mqttSendStatus(uint8_t output, uint8_t state)
{
  char tpc[MAX_MQTT_TOPIC_LENGTH], msg[2];
  sprintf(tpc, "%s/%d", MQTT_TOPIC_NETWORK, output);
  // Pubñlish current RGB value
  sprintf(msg, "%d", state);
  // Publish current output values
  client.publish(tpc, msg);
}

