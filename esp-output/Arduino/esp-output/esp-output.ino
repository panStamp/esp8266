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
 * Creation date: 08/11/2016
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "userdata.h"

/**
 * Description string
 */
char deviceId[64];

// Wifi client
WiFiClient espClient;

// On-board LED
#define LED  15

// Unique device key based on MAC
char deviceKey[WL_MAC_ADDR_LENGTH * 2 + 1];   // Device key

/*
 * Output pins
 */
const uint8_t binout[] = {16, 14, 12, 13, 4, 5};

/**
 * Output states
 */
uint8_t outputState[sizeof(binout)];

/**
 * str2Hex
 * 
 * Convert 2-char strings into hex number
 * 
 * @param hexByte Hex byte in string format
 */
uint8_t str2Hex(char *hexByte)
{
  uint8_t res = 0;

  for(int i=0 ; i<2 ; i++)
  {
    res <<= 4;
    
    if (hexByte[i] >= 0x30 && hexByte[i] < 0x3A)  // 0 .. 9
      res |= hexByte[i] - 0x30;
    else if (hexByte[i] >= 0x41 && hexByte[i] < 0x47) // A .. F
      res |= hexByte[i] - 0x37;
    else if (hexByte[i] >= 0x61 && hexByte[i] < 0x67) // a .. f
      res |= hexByte[i] - 0x57;
  }

  return res;
}

/**
 * Drive output
 * 
 * @param output Output number
 * @param state Output state
 * 
 * @return true in case of command successfully executed
 *         false otherwise
 */
bool driveOutput(uint8_t output, uint8_t state)
{
  if ((output >= 0) && (output < sizeof(binout)))
  {
    digitalWrite(binout[output], state);
    outputState[output] = state;
    mqttSendStatus(output, state);
    return true;
  }
  return false;
}

/*
 * Initial setup
 */
void setup()
{
  int i;
  
  // Initialize on-board LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  // Initialize output pins
  for(i=0 ; i < sizeof(binout) ; i++)
  {
    pinMode(binout[i], OUTPUT);
    digitalWrite(binout[i], LOW);
    outputState[i] = 0;
  }

  // Append last two bytes of the MAC to the device ID
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);

  char buf[3];
  // Set Device key from MAC address
  memset((uint8_t*)deviceKey, 0, sizeof(deviceKey));
  for(i=0; i<WL_MAC_ADDR_LENGTH ; i++)
  {
    if (mac[i] < 0x10)
      sprintf(buf, "0%X", mac[i]);
    else
      sprintf(buf, "%X", mac[i]);

    deviceKey[i*2] = buf[0];
    deviceKey[i*2+1] = buf[1];
  }

  // Set device ID
  sprintf(deviceId, "%s %s", description, deviceKey);
    
  Serial.begin(38400);
  Serial.println(deviceId);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED, !digitalRead(LED));
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED, LOW);

  Serial.println("");
  Serial.println(deviceId);
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT server
  mqttConnect();
  mqttHandle();
  
  // Initialize web interface
  initWebServer();
}

/**
 * Endless loop
 */
void loop()
{
  mqttHandle();
  httpHandle();
}

