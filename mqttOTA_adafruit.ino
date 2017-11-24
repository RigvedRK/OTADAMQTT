/*Libraries*/

#include <Arduino.h>
#include <Credentials.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "abcd"
#define WLAN_PASS       "abcd"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                                                                                                                 // use 8883 for SSL
#define AIO_USERNAME    "username"                                                                                                             //Adafruit Username
#define AIO_KEY         "key"                                                                                   //Adafruit Key 

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/***************************For OTA*************************************/

ESP8266WiFiMulti WiFiMulti;

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.                                                                                            Feed used to send data
//Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

void MQTT_connect();

void setup() 
{
 
  WiFi.begin(WLAN_SSID, WLAN_PASS);                                                                                                           //Connecting to Wifi
    while (WiFi.status() != WL_CONNECTED) 
      {
         delay(500);
      }
     
    WiFiMulti.addAP(WLAN_SSID, WLAN_PASS);

  mqtt.subscribe(&onoffbutton);
}

void loop() 
{
  
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    if (subscription == &onoffbutton)
    {
      if (strcmp((char *)onoffbutton.lastread, "ON") == 0)
      {
       if((WiFiMulti.run() == WL_CONNECTED)) 
       {
                     t_httpUpdate_return ret = ESPhttpUpdate.update("server/path/file.bin");
       } 
      }
    }
  }

                                                                                                                                                  //Publish Data
                                                                                                                                                
                                                                                                                                                //  if (! photocell.publish(x++)) {
                                                                                                                                                //    Serial.println(F("Failed"));
                                                                                                                                                //  } else {
                                                                                                                                                //    Serial.println(F("OK!"));
                                                                                                                                                //  }
  /*
  if(! mqtt.ping()) {                                                                                                                           // to be used if a bigger loop needs to be executed
    mqtt.disconnect();
  }
  */
}

void MQTT_connect() 
{
  int8_t ret;

  if (mqtt.connected())                                                                                                                         //Check if already connected  Returns 0 for connection
    {
      return;
    }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)                                                                                                           //Retries to connect
    { 
         
         mqtt.disconnect();
         delay(5000);  // wait 5 seconds
         retries--;
         if (retries == 0) 
         {
             while (1);
         }
    }
}
