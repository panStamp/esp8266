/**
 * Copyright (c) 2016 LIBERiot/panStamp <dberenguer@panstamp.com>
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

#ifndef _USERDATA_H
#define _USERDATA_H

/**
 * Wifi settings
 * Enter here your SSID and password
 */
const char* ssid     = "Panstamp";
const char* password = "machacalabarraca";

/**
 * MQTT settings
 */
IPAddress MQTT_BROKER(192, 168, 1, 59);
const int MQTT_PORT = 1883;
const char MQTT_TOPIC_MAIN[] = "mynetwork";  // Main MQTT topic
const char description[] = "panStamp 6-relay WiFi board";
const uint32_t hBeatPeriod = 60000; // Transmit heartbeat every 60 sec

#endif

