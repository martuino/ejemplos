///
/// @mainpage	CC3200_NTP_RTC
///
/// @details	RTC and NTP Date and Time Library for MSP432, CC3200 and TM4C
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
/// @date		18/07/2015 16:22
/// @version	101
///
/// @copyright	(c) Rei Vilo, 2015
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
///


///
/// @file		CC3200_NTP_RTC.ino
/// @brief		Example for NTP with CC3200 and MSP432
///
/// @details Based on UDP NTP Client, part of Energia 16 distribution
/// * Created 4 Sep 2010 by Michael Margolis
/// * Modified 9 Apr 2012 by Tom Igoe
/// * Modified 1 July 2014 by Noah Luskey
/// * Updated July 19, 2015 by Rei Vilo
///
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
/// @date		18/07/2015 16:22
/// @version	101
///
/// @copyright	(c) Rei Vilo, 2015
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
/// @n
///


#include "Energia.h"

#include <WiFi.h>

// The time library provides all the tools.
// No need for reinventing them!
//#include "time.h"
#include "DateTimeLibrary.h"
#include "Credentials.h"            // for SSID and password

// Define variables and constants
DateTime myRTC;
time_t myEpochNTP, myEpochRTC;
tm myTimeNTP, myTimeRTC;
uint32_t counter = 0;

// First time update of RTC
bool flagRTC = true;

// Prototypes
void printWifiStatus();
uint32_t sendNTPpacket(IPAddress& address);

// Add setup code
// Add loop code
void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    delay(100);
    Serial.println();
    
    myRTC.begin();
    myRTC.setTimeZone(tz_CEST);
    
    // Connect to Wifi network:
    Serial.print("Connecting to network ");
    Serial.print(ssid);
    
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println(" done");
    
    Serial.print("Waiting for IP address...");
    
    while (WiFi.localIP() == INADDR_NONE)
    {
        Serial.print(".");
        delay(300);
    }
    
    IPAddress myIP = WiFi.localIP();
    Serial.print(myIP);
    Serial.println(" done");
/*
    Serial.print("\nConnection to server ");
    Serial.print(timeServer);
    Serial.print("...");
    myUDP.begin(localPort);
    Serial.println(" done");
*/
 }

// Add loop code
void loop()
{
    bool flagNTP = getTimeNTP(myEpochNTP);
    
    Serial.print("NTP ? ");
    Serial.println(flagNTP, DEC);
    
    if (flagNTP)
    {
        // Set time to RTC, only once
        if (flagRTC)
        {
            myRTC.setTime(myEpochNTP);
            flagRTC = false;
            Serial.println("*** CC3200 RTC updated!");
        }
        
        myEpochRTC = myRTC.getTime();
        
        // Check difference
        Serial.print("NTP - RTC = \t");
        /*
         Serial.print(myEpochNTP, DEC);
         Serial.print("\t - \t");
         Serial.print(myEpochRTC, DEC);
         Serial.print("\t = \t");
         */
        Serial.println(myEpochNTP - myEpochRTC, DEC);
        
        convertEpoch2Structure(myEpochNTP, myTimeNTP);
        convertEpoch2Structure(myEpochRTC, myTimeRTC);
        
        // Print date and time nicely
        Serial.print("NTP = \t");
        Serial.print(stringDateTime(myTimeNTP));
        
        Serial.print("\rRTC = \t");
        Serial.print(stringDateTime(myEpochRTC));
        Serial.print("\r");
        
        Serial.println(formatStringDateTime("CEST time is %I:%M %p.", myRTC.getLocalTime()));
    }
    
    for (int8_t i=10; i>0; i--)
    {
        Serial.print(".");
        Serial.print(i-1, DEC);
        Serial.print("\r");
        delay(1000);
    }
    Serial.println("  ");
}

void printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    
    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
