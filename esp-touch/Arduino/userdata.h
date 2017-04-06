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

/**
 * Wifi settings
 * Enter here your SSID and password
 */
const char* ssid     = "Your_ssid";
const char* password = "Your_password";
/**
 * MQTT settings
 */
 IPAddress MQTT_BROKER(192, 168, 1, 12); // MQTT broker IP address

/*  
 *   #define mqtt_user "your_username"  
 *   #define mqtt_password "your_password"   
 */

const int MQTT_PORT = 1883; // MQTT broker port
const char MQTT_TOPIC_MAIN[] = "mynetwork";  // Main MQTT topic
const char description[] = "panStamp Tocuh Panel";
const uint32_t sensorPeriodTH = 30000; // Transmit heartbeat every 30 sec

#endif

