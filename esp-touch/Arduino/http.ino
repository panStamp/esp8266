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
    htmlRoot();      
  });
      
  server.on("/setting", []()
  {
    htmlSetting();    
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


/*
 * htmlSetting
 * 
 * Html build for network setting
*/
void htmlSetting(void)
{
   char htmlSend[2964];
  if(!connectApMode)
  {
    bool changeSave = setSetting();
    
    const char html[] = "<body style='margin:0px; padding:0px;'><div style='width:100%; height:70px; background:#E5E7E9; border-bottom:0.5px #D7DBDD solid;'><img src='http://www.panstamp.com/wp/wp-content/uploads/2014/11/logo2x.png' width='150px' height='50px' style='margin-left:15px; margin-top:10px;'/></div>";
        
    const char html0[] = "<center><form method='GET'><table style='margin:50px auto 40px auto;'><tr><td><a href='/'style='text-decoration:none;'><h1 style='color:#E5E7E9; margin-left:25px;'>TH sensor</h1></a></td><td><div style='background:#D7DBDD; width:3px; height:70px; margin:-10px 30px auto 30px;'></div></td><td><h1 style='color:#EDBB99;'>Network settings</h1></td></tr></table><table><tr><td colspan='2'><h4>WIFI settings</h4></td></tr><tr><td><p align='right'>Ssid:</p></td><td><input type='text' name='ssid' size='20' maxlength='20' value='";
    const char html1[] = "'></td></tr><tr><td><p align='right'>Password:</p></td><td><input type='password' name='password' size='20' value='";
    const char html2[] = "'></td></tr><tr><td colspan='2'><hr></td></tr><tr><td colspan='2'><h4>MQTT settings</h4></td></tr><tr><td><p align='right'>Port:</p></td><td><input type='text' name='port' size='20' value='";

    const char html3[] = "'></td></tr><tr><td><p align='right'>Broker:</p></td><td><input type='text' name='broker' size='20' value='";
    const char html4[] = "'></td></tr><tr><td><p align='right'>Main topic:</p></td><td><input type='text' name='topic' size='20' value='";
    const char html5[] = "'></td></tr><tr><td colspan='2'><hr></td></tr><tr><td colspan='2'><h4>Temperature and humidity sensor</h4></td></tr><tr><td><p align='right'>Transmission period:</p></td><td><input type='text' style='margin-top:23px;' name='period' size='20' value='";
    const char html6[] = "'><div style='margin-top:-5px;'><p style='font-size:12px; color:#717D7E;'>Transimision time in milliseconds</p></div></td></tr></table>";
  
    const char html7[] = "<p style='color:green; font-size:14px;'>Changes saved successfully</p>";
  
    const char html8[] = "<p><input type='submit' name='save' value='SAVE' ></p></form></center>";
    const char html9[] = "<br><center><a href='http://www.panstamp.com' target='_blank'>www.panstamp.com</a></center>";
         
  
    if (!changeSave)
      sprintf(htmlSend, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",html, html0, config.ssid, html1, config.password, html2, config.mqttPort, html3, config.broker, html4 ,config.topicMain, html5, config.sensorPeriodTH, html6, html8, html9);  
    else
      sprintf(htmlSend, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",html, html0, config.ssid, html1, config.password, html2, config.mqttPort, html3, config.broker, html4 ,config.topicMain, html5, config.sensorPeriodTH, html6, html7, html8, html9);    
  }
   
   server.send(200, "text/html", htmlSend);
}

/*
 * htmlSensorTH
 * 
 * Html build for ApMode and if not shows temperature and humidity in the network 
*/
void htmlRoot(void)
{
  char htmlSend[2964];
  if (connectApMode)
  { 
    bool changeSave = setSetting();
            
    const char html0[] = "<center><form method='GET'><h1>Network settings</h1><table><tr><td colspan='2'><h4>WIFI settings</h4></td></tr><tr><td><p align='right'>Ssid:</p></td><td><input type='text' name='ssid' size='20' maxlength='20' value='";
    const char html1[] = "'></td></tr><tr><td><p align='right'>Password:</p></td><td><input type='password' name='password' size='20' value='";
    const char html2[] = "'></td></tr><tr><td colspan='2'><hr></td></tr><tr><td colspan='2'><h4>MQTT settings</h4></td></tr><tr><td><p align='right'>Port:</p></td><td><input type='text' name='port' size='20' value='";
  
    const char html3[] = "'></td></tr><tr><td><p align='right'>Broker:</p></td><td><input type='text' name='broker' size='20' value='";
    const char html4[] = "'></td></tr><tr><td><p align='right'>Main topic:</p></td><td><input type='text' name='topic' size='20' value='";
    const char html5[] = "'></td></tr><tr><td colspan='2'><hr></td></tr><tr><td colspan='2'><h4>Temperature and humidity sensor</h4></td></tr><tr><td><p align='right'>Transmission period:</p></td><td><input type='text' style='margin-top:23px;' name='period' size='20' value='";
    const char html6[] = "'><div style='margin-top:-5px;'><p style='font-size:12px; color:#717D7E;'>Transimision time in milliseconds</p></div></td></tr></table>";
  
    const char html7[] = "<p style='color:green; font-size:14px;'>Changes saved successfully press reset</p>";
  
    const char html8[] = "<p><input type='submit' name='save' value='SAVE' ></p></form>";
    const char html9[] = "<br><a href='http://www.panstamp.com' target='_blank'>www.panstamp.com</a></center>";
         
  
    if (!changeSave)
      sprintf(htmlSend, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s", html0, config.ssid, html1, config.password, html2, config.mqttPort, html3, config.broker, html4 ,config.topicMain, html5, config.sensorPeriodTH, html6, html8);
    else
      sprintf(htmlSend, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", html0, config.ssid, html1, config.password, html2, config.mqttPort, html3, config.broker, html4 ,config.topicMain, html5, config.sensorPeriodTH, html6, html7, html8);     
  }
  else
  {
    int nT1 = int(temperature);
    int nT2 = (temperature - nT1) * 100;
  
    int nH1 = int(humidity);
    int nH2 = (humidity - nH1) * 100;
  
    const char html[] = "<body style='margin:0px; padding:0px;'><div style='width:100%; height:70px; background:#E5E7E9; border-bottom:0.5px #D7DBDD solid;'><img src='http://www.panstamp.com/wp/wp-content/uploads/2014/11/logo2x.png' width='150px' height='50px' style='margin-left:15px; margin-top:10px;'/></div>";        
    const char html0[] = "<center><table style='margin:50px auto 40px auto;'><tr><td><h1 style='color:#EDBB99; margin-left:25px;'>TH sensor</h1></td><td><div style='background:#D7DBDD; width:3px; height:70px; margin:-10px 30px auto 30px;'></div></td><td><a href='/setting'style='text-decoration:none;'><h1 style='color:#E5E7E9;'>Network settings</h1></a></td></tr></table>";    
      
    const char html1[] = "<table stile='width:600px;'><tr><td><div style='float:left; margin-left:20px; margin-right:20px;'><h3 style='color:#2980B9;'>Temperature:<h3><h1 style='color:#AED6F1; font-size:70px; margin-left:30px; margin-top:-15px;'>";
    const char html2[] = "</h1></div><div style='float:left; margin-left:20px;'><h3 style='color:#2980B9;'>Humidity:</h3><h1 style='color:#AED6F1; font-size:70px; margin-left:30px; margin-top:-15px;'>";
    const char html3[] = "</h1></div></td></tr></table>";
  
    const char html4[] = "<br><a href='http://www.panstamp.com' target='_blank'>www.panstamp.com</a></center>";
  
    sprintf (htmlSend, "%s%s%s%i%s%i %s%s%i%s%i %s%s%s", html, html0, html1, nT1, ".", nT2, "C", html2, nH1, ".", nH2, "%", html3, html4);

  }
  
  server.send(200, "text/html", htmlSend);  
}

/**
 * setSetting
 * 
 * Set settings
 */
bool setSetting(void)
{ 
  if (server.args() == 7)
  {
    if (server.argName(0) == "ssid" && server.argName(1) == "password" && server.argName(2) == "port" && server.argName(3) == "broker" && server.argName(4) == "topic" && server.argName(5) == "period")
    {
      if (server.arg(0).length() > 0 && server.arg(1).length() > 0 && server.arg(2).length() > 0 && server.arg(3).length() > 0 && server.arg(4).length() > 0 && server.arg(5).length() > 0)
      {         
        // Update global variables
        server.arg(0).toCharArray(config.ssid, sizeof(config.ssid));
        server.arg(1).toCharArray(config.password, sizeof(config.password));

        server.arg(2).toCharArray(config.mqttPort, sizeof(config.mqttPort));
        server.arg(3).toCharArray(config.broker, sizeof(config.broker));
        server.arg(4).toCharArray(config.topicMain, sizeof(config.topicMain));

        server.arg(5).toCharArray(config.sensorPeriodTH, sizeof(config.sensorPeriodTH));
                
        
        // Save config in EEPROM
        config.saveWifiConfig();
        config.saveMqttConfig();
        config.saveSensorPeriodConfig();
                    
        return true;

      }
    }
  }
  return false;    
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



