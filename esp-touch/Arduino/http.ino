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


/*
 * initWebServer
 * 
 * Web Server
 *  
 */
void initWebServer(void)
{
  server.on("/", []()
  {
    server.send(200, "text/plain", "");
  });
  
  server.on("/temperature", []()
  {
    int n1 = int(temperature);
    int n2 = (temperature - n1) * 100;
    char temp[32];
    sprintf (temp, "%i%s%i %s", n1, ".", n2, "C");
    server.send(200, "text/plain", temp);
  });
  
  server.on("/humidity", []()
  {
    int n1 = int(humidity);
    int n2 = (humidity - n1) * 100;
    char hum[32];
    sprintf (hum, "%i%s%i %s", n1, ".", n2, "%"); 
    server.send(200, "text/plain", hum);
  });
  
  server.begin(); 
}

/*
 * httpHandle
 * 
 * We initialize
 */
void miHumidity(void)
{
  myHumidity.begin();
}

/*
 * httpHandle
 * 
 * Handle HTTP requests
 */
void httpHandle(void)
{
  server.handleClient();
}


