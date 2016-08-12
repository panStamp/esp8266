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
 * Creation date: 08/12/2016
 */
 
#include <ESP8266WebServer.h>
#include "userdata.h"

// Web server
ESP8266WebServer server(80);
String webString="";

void handle_root()
{
  server.send(200, "text/plain", "Visit https://github.com/panStamp/ESP8266 for details");
  delay(100);
}

void initWebServer(void)
{
  server.on("/", handle_root);

  server.on("/control", []()
  {
    controlOutput();
  });
 
  server.begin();
}

/**
 * Handle HTTP requests
 */
void httpHandle(void)
{
  server.handleClient();
}

/**
 * controlOutput
 * 
 * Drive binary output
 */
void controlOutput(void)
{
  uint8_t i;
  webString = "Request not supported";
  
  if (server.args() == 2)
  {
    char value[64], buf[7];
  
    if (server.argName(0) == "output")
    {
      if (server.arg(0).length() > 0)
      {
        if (server.argName(1) == "state")
        {
          if (server.arg(1).length() > 0)
          {
            // Update global variables
            uint8_t output = (uint8_t)server.arg(0).toInt();
            uint8_t state = (uint8_t)server.arg(1).toInt();
            // Drive output
            if (driveOutput(output, state))
            {
              // Return OK          
              String str = "OK";
              server.send(200, "text/plain", str);
              return;
            }
          }
        }
      }
    }
  }    

  server.send(200, "text/plain", webString);
}

