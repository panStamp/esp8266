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

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SparkFunHTU21D.h>

HTU21D myHumidity;

ESP8266WebServer server(80);
String webString="";

void handle_root()
{
  server.send(200, "text/plain", "Visit https://github.com/liberiot/ESP8266-mqtt-gateway for details");
  delay(100);
}
/*
 * initWebServer
 * 
 * Web Server
 *  
 */
void initWebServer(void)
{
  server.on("/", handle_root);

  server.on("/setnet", []()
  {
    setNetwork();
  });

  server.on("/setmqtt", []()
  {
    setMqtt();
  });

  server.on("/setinterval", []()
  {
    setTxInterval();
  });
  
  server.on("/temperature", []()
  {
    int n1 = int(temperature);
    int n2 = (temperature - n1) * 100;
    char temp[32];
    sprintf (temp, "%i%s%i", n1, ".", n2);
    server.send(200, "text/plain", temp);
  });
  
  server.on("/humidity", []()
  {
    int n1 = int(humidity);
    int n2 = (humidity - n1) * 100;
    char hum[32];
    sprintf (hum, "%i%s%i", n1, ".", n2); 
    server.send(200, "text/plain", hum);
  });
   
  server.begin(); 
}

/**
 * setNetwork
 * 
 * Set WiFi settings
 */
void setNetwork(void)
{
  webString = "Request not supported";
  
  if (server.args() == 2)
  { 
    if (server.argName(0) == "ssid")
    {
      if (server.arg(0).length() > 0)
      {
        if (server.argName(1) == "pwd")
        {
          if (server.arg(1).length() > 0)
          {
            // Update global variables
            server.arg(0).toCharArray(config.ssid, sizeof(config.ssid));
            server.arg(1).toCharArray(config.password, sizeof(config.password));
            // Save config in EEPROM
            config.saveWifiConfig();            
            String str = "OK";
            server.send(200, "text/plain", str);
            return;
          }
        }
      }
    }
  }    

  server.send(200, "text/plain", webString);
}


/**
 * setTxInterval
 * 
 * Set transmission interval for temperature and humidity
 */
void setTxInterval(void)
{
  webString = "Request not supported";
  
  if (server.args() == 1)
  { 
    if (server.argName(0) == "interval")
    {
      if (server.arg(0).length() > 0)
      {
        // Update global variables
        server.arg(0).toCharArray(config.txIntervalTH, sizeof(config.txIntervalTH));
        // Save config in EEPROM
        config.saveTxIntervalConfig();            
        String str = "OK";
        server.send(200, "text/plain", str);
        return;
      }
    }
  }    

  server.send(200, "text/plain", webString);
}

/**
 * setMqtt
 * 
 * Set MQTT settings
 */
void setMqtt(void)
{
  webString = "Request not supported";

  if (server.args() == 3)
  {
    if (server.argName(0) == "broker")
    {
      if (server.arg(0).length() > 0)
      {
        if (server.argName(1) == "port")
        {
          if (server.arg(1).length() > 0)
          {
            if (server.argName(2) == "topic")
            {
              if (server.arg(2).length() > 0)
              {
                // Update global variables
                server.arg(0).toCharArray(config.broker, sizeof(config.broker));
                server.arg(1).toCharArray(config.mqttPort, sizeof(config.mqttPort));
                server.arg(2).toCharArray(config.mainTopic, sizeof(config.mainTopic));
                // Save config in EEPROM
                config.saveMqttConfig();            
                String str = "OK";
                server.send(200, "text/plain", str);
                return;
              }
            }
          }
        }
      }
    }
  }  

  server.send(200, "text/plain", webString);
}

/*
 * initWebServer
 * 
 * Handle HTTP requests
 */
void httpHandle(void)
{
  server.handleClient();
}



