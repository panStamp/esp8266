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

#include "userdata.h"
   
/**
 * readWifiConfig
 *
 * Read Wifi settings from EEPROM
 * 
 * @return true if config found. Return false otherwise
 */
bool USERDATA::readWifiConfig(void)
{
  bool found = false;
  
  // WiFi network SSID
  for(int i=0 ; i<EEPROM_MAX_PARAM_LENGTH ; i++)
  {
    ssid[i] = EEPROM.read(EEPROM_WIFI_SSID + i);
    if (ssid[i] == 0)
    {
      found = true;
      break;
    }
  }

  if (found)
  {
    found = false;
    // WiFi password
    for(int i=0 ; i<EEPROM_MAX_PARAM_LENGTH ; i++)
    {
      password[i] = EEPROM.read(EEPROM_WIFI_PWD + i);
      if (password[i] == 0)
      {
        found = true;
        break;
      }
    }
  }
  
  return found;
}

/**
 * readMqttConfig
 *
 * Read Mqtt broker/port/topic settings from EEPROM
 * 
 * @return true if config found. Return false otherwise
 */
bool USERDATA::readMqttConfig(void)
{
  bool found = false;
  
  // MQTT BROKER
  for(int i=0 ; i<MAX_BROKER_LENGTH ; i++)
  {
    broker[i] = EEPROM.read(EEPROM_MQTT_BROKER + i);
    if (broker[i] == 0)
    {
      found = true;
      break;
    }
  }

  if (found)
  {
    found = false;
    // MQTT PORT
    for(int i=0 ; i<MAX_PORT_LENGTH ; i++)
    {
      mqttPort[i] = EEPROM.read(EEPROM_MQTT_PORT + i);
      if (mqttPort[i] == 0)
      {
        found = true;
        break;
      }
    }
  }
  if (found)
  {
    found = false;
    // MQTT TOPIC
    for(int i=0 ; i<MAX_TOPIC_MAIN_LENGTH ; i++)
    {
      mainTopic[i] = EEPROM.read(EEPROM_TOPIC_MAIN + i);
      if (mainTopic[i] == 0)
      {
        found = true;
        break;
      }
    }      
  }
  return found;
}

/**
 * readTxIntervalConfig
 *
 * Read sensor period from EEPROM
 * 
 * @return true if config found. Return false otherwise
 */
bool USERDATA::readTxIntervalConfig(void)
{
  bool found = false;
  
  // Sensor period temperature and humidity
  for(int i=0 ; i<MAX_SENSOR_PERIOD_LENGTH ; i++)
  {
    txIntervalTH[i] = EEPROM.read(EEPROM_SENSOR_PERIOD + i);
    if (txIntervalTH[i] == 0)
    {
      found = true;
      break;
    }

  }
  
  return found;
}

/**
 * saveWifiConfig
 *
 * Save Wifi config in EEPROM
 */
void USERDATA::saveWifiConfig(void)
{
  uint8_t i;
  
  for(i=0 ; i<strlen(ssid) ; i++)
    EEPROM.write(EEPROM_WIFI_SSID + i, ssid[i]);
  EEPROM.write(EEPROM_WIFI_SSID + i, 0);

  for(i=0 ; i<strlen(password) ; i++)
    EEPROM.write(EEPROM_WIFI_PWD + i, password[i]);
  EEPROM.write(EEPROM_WIFI_PWD + i, 0);

  EEPROM.commit();
}

/**
 * saveMqttrConfig
 *
 * Save Mqtt config in EEPROM
 */
void USERDATA::saveMqttConfig(void)
{
  uint8_t i;


  for(i=0 ; i< strlen(broker) ; i++)
    EEPROM.write(EEPROM_MQTT_BROKER + i, broker[i]);
  EEPROM.write(EEPROM_MQTT_BROKER + i, 0);
  
  for(i=0 ; i<strlen(mqttPort) ; i++)
    EEPROM.write(EEPROM_MQTT_PORT + i, mqttPort[i]);
  EEPROM.write(EEPROM_MQTT_PORT + i, 0);  
  
  for(i=0 ; i<strlen(mainTopic) ; i++)
    EEPROM.write(EEPROM_TOPIC_MAIN + i, mainTopic[i]);
  EEPROM.write(EEPROM_TOPIC_MAIN + i, 0);
  
  EEPROM.commit();
}

/**
 * saveTxIntervalConfig
 *
 * Save sensor period in EEPROM
 */
void USERDATA::saveTxIntervalConfig(void)
{
  uint8_t i;
  
  for(i=0 ; i<strlen(txIntervalTH) ; i++)
    EEPROM.write(EEPROM_SENSOR_PERIOD + i, txIntervalTH[i]);
  EEPROM.write(EEPROM_SENSOR_PERIOD + i, 0);

  EEPROM.commit();
}

