![Programming pinout](http://panstamp.org/pictures/esp-output_01.jpg)

esp-output is designed and manufactured by [panStamp](http://www.panstamp.com) based on the popular ESP8266 WiFi IC, to create a simple but powerful device with six relay outputs and a WiFi interface. This device is controllable via MQTT and HTTP by means of a very simple packet structure. This board is programmable from the Arduino IDE

# Arduino IDE

This sketch requires the ESP8266 board to be installed from Tools->Board->Boards Manager. After this, select "Generic ESP8266 Module" from Tools->Board. The on-board ESP8266 IC can be programed from the Arduino IDE through a simple USB-UART(3.3V) converter following this pinout:

![Programming pinout](http://panstamp.org/pictures/esp-output_progpinout.jpg)

The above diagram is valid whenever the esp-output board is not powered from the main 12V input. Otherwise, remove the Vcc connection between USB converter and relay board.

Like any other ESP8266 board, esp-output needs the following sequence of RESET-GPIO0 button presses in orde rto put the board in programing mode:

1. Press and hold GPIO0
2. Press and release RESET
3. Release GPIO0

Quite simple, isn't it? Hehe... Well we messed up with the silkscreened labels on the current version (1.0) so the switch named as "RESET" is "GPIO0" in fact and the opposite. We will fix this mistake in the next revision.

## Customization

This board needs to be attached to a WiFi network so either the WiFi SSID and password needs to be entered into userdata.h before compiling the application.

```C++
/**
 * Wifi settings
 * Enter here your SSID and password
 */
const char* ssid     = "myssid";
const char* password = "mypassword";
```

You will also want to set address to your MQTT broker and the name of your network, which will be used as the main MQTT topic

```C++
/**
 * MQTT settings
 */
IPAddress MQTT_BROKER(192, 168, 1, 59);
//#define MQTT_BROKER  "mqtt.mybroker.net"
const int MQTT_PORT = 1883;
const char MQTT_TOPIC_MAIN[] = "mynetwork";  // Main MQTT topic
```

Once the above steps done, you can flash your esp-output board by pressing the "Upload" button from the Arduino IDE.

## MQTT

MQTT is the main mechanism to drive the board relays. This board also provides support for HTTP commands but bear in mind that the provided sketch needs a MQTT broker to work.

Each esp-output board is uniquely identified with its MAC. You have to open a serial terminal (38400 bps 8N1) against the board the first time it is started in order to get the MAC:

```
Working to connect.......
panStamp 6-relay WiFi board 1AFE34D30AAD
Connected to Panstamp
IP address: 192.168.1.68
Attempting MQTT connection...connected
```

In the above example 1AFE34D30AAD is the board's MAC. In order to subscribe to the statuses sent by the board we can run the following mosquitto command:

```
mosquitto_sub -h localhost -p 1883 -v -t mynetwork/1AFE34D30AAD/#
```

The above command subscribes only to the packets transmitted by our board. In case that we want to see anything happening in "mynetwork" then we need to run:

```
mosquitto_sub -h localhost -p 1883 -v -t mynetwork/#
```

esp-output sends 1-minute periodic heartbeats like this one:

```
mynetwork/1AFE34D30AAD/system RUNNING
```

And also a "CONNECTED" message only at startup:

```
mynetwork/1AFE34D30AAD/system CONNECTED
```

In roder to control the board we need to publish through the "control" subtopic as follows:

```
mosquitto_pub -h localhost -p 1883 -t mynetwork/1AFE34D30AAD/control/5 -m 1
```

Where "localhost" means that we are running the MQTT broker on the same computer where we are running mosquitto_sub and mosquitto_pub. 1883 is the default MQTT port and the "mynetwork/1AFE34D30AAD/control/5" topic means that we want to drive relay number 5. Relays are numbered from 0 to 5. Finally "1" as the message payload will turn the relay on whilst "0" will turn it off.

In response to the above MQTT control command, the board will reply with the relay being turned on and also withthe following status message:

```
mynetwork/1AFE34D30AAD/network/5 1
```

Remember that "mynetwork/1AFE34D30AAD/network/5", the topic, is being displayed because we have enabled the "-v" option in the mosquitto_sub command.

## HTTP

We have seen how easy is controlling the esp-output board via MQTT. We are now about to explain how to do the same via HTTP. Outputs and states are transmitted to the board by means of simple HTTP GET commands:

```
http://192.168.1.68/control?output=5&state=1
```

Where 192.168.1.68 is the IP address of the esp-control board in this case. Since the IP address is got by DHCP from the router, we need to get this address from a serial terminal in the same way as we got the MAC in the previous section.

## Technical support

[panStamp forum](http://www.panstamp.org/forum/forumdisplay.php?fid=26)

