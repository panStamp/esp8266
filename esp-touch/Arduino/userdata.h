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
 
#ifndef _USERDATA_H
#define _USERDATA_H

#include <EEPROM.h>


#define MAX_BROKER_LENGTH  20
#define MAX_PORT_LENGTH 6
#define MAX_TOPIC_MAIN_LENGTH 64
#define MAX_SENSOR_PERIOD_LENGTH 10

/**
 * Pseudo-EEPROM addresses
 */
#define EEPROM_MAX_PARAM_LENGTH  64
#define EEPROM_WIFI_SSID  0
#define EEPROM_WIFI_PWD      EEPROM_WIFI_SSID + EEPROM_MAX_PARAM_LENGTH
#define EEPROM_MQTT_BROKER      EEPROM_WIFI_PWD + EEPROM_MAX_PARAM_LENGTH 
#define EEPROM_MQTT_PORT      EEPROM_MQTT_BROKER + MAX_BROKER_LENGTH
#define EEPROM_TOPIC_MAIN     EEPROM_MQTT_PORT  + MAX_PORT_LENGTH 
#define EEPROM_SENSOR_PERIOD     EEPROM_TOPIC_MAIN + MAX_TOPIC_MAIN_LENGTH 


/**
 * Default WiFi Access Point settings
 */
const char apPassword[] = "panstamp";
const char apName[] = "TouchPanel";


const char description[] = "panStamp Tocuh Panel";


/**
 * Class: USERDATA
 * 
 * Description:
 * USER config
 */
class USERDATA
{
  public:

    /**
     * Wifi settings
     */
    char ssid[EEPROM_MAX_PARAM_LENGTH];     // WiFi network SSID
    char password[EEPROM_MAX_PARAM_LENGTH]; // WiFi password

    /**
     * Mqtt port, broker and topic
     */
    char mqttPort[MAX_PORT_LENGTH];      // MQTT PORT    
    char broker[MAX_BROKER_LENGTH];    // MQTT BROKER       
    char topicMain[MAX_TOPIC_MAIN_LENGTH];   // Topic

    /**
     * Sensor transmition period 
     */
    char sensorPeriodTH[MAX_SENSOR_PERIOD_LENGTH];  

    /**
     * begin
     * 
     * Initialize EEPROM
     */
    inline void begin(void)
    {
      // Config pseudo-EEPROM space
      EEPROM.begin(512);
    }
    
    /**
     * readConfig
     * 
     * Read complete configuration settings from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    inline bool readConfig(void)
    {
      if (readWifiConfig())
      {
        if (readMqttConfig())
        {
          if (readSensorPeriodConfig())
            return true;
        }
      }

      return false;
    }
    
    /**
     * readWifiCondif
     *
     * Read Wifi settings from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    bool readWifiConfig(void);
  
    /**
     * readMqttConfig
     *
     * Read Mqtt broker/port/topic settings from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    bool readMqttConfig(void);


    /**
     * readSensorPeriodConfig
     *
     * Read sensor period from EEPROM
     * 
     * @return true if config found. Return false otherwise
     */
    bool readSensorPeriodConfig(void);

    /**
     * saveWifiConfig
     *
     * Save Wifi config in EEPROM
     */
    void saveWifiConfig(void);


    /**
     * saveMqttrConfig
     *
     * Save Mqtt config in EEPROM
     */
    void saveMqttConfig(void);

    /**
     * saveSensorPeriodConfig
     *
     * Save sensor period in EEPROM
     */
    void saveSensorPeriodConfig(void);
};


#endif

