//
// DateTimeLibrary.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode+
// http://embedXcode.weebly.com
//
// Project 		MSP432_RTC
//
// Created by 	Rei Vilo, 19/07/2015 16:31
// 				http://embeddedcomputing.weebly.com
//
// Copyright 	(c) Rei Vilo, 2015
// Licence		CC = BY NC SA
//
// See 			DateTimeLibrary.h and ReadMe.txt for references
//


// Library header
#include "DateTimeLibrary.h"

// Utilities
void convertEpoch2Structure(time_t timeEpoch, tm &timeStructure)
{
    // gmtime_r for re-entrance
    gmtime_r(&timeEpoch, &timeStructure);
}

void convertStructure2Epoch(tm timeStructure, time_t &timeEpoch)
{
    timeEpoch = mktime(&timeStructure);
}

String stringDateTime(tm timeStructure)
{
    return (String)asctime(&timeStructure);
}

String stringDateTime(time_t timeEpoch)
{
    return (String)ctime(&timeEpoch);
}

String formatStringDateTime(const char * format, tm timeStructure)
{
    char buffer[128];
    strftime(buffer, 128, format, &timeStructure);
    return (String)buffer;
}

String formatStringDateTime(const char * format, time_t timeEpoch)
{
    tm timeStructure;
    convertEpoch2Structure(timeEpoch, timeStructure);
    return formatStringDateTime(format, timeStructure);
}

// Class
DateTime::DateTime()
{
    _timeZoneDifference = 0;
}

void DateTime::setTimeZone(int32_t timeZone)
{
    _timeZoneDifference = timeZone;
}

uint32_t DateTime::getLocalTime()
{
    return getTime() + _timeZoneDifference;
}

void DateTime::setLocalTime(uint32_t epoch)
{
    setTime(epoch + _timeZoneDifference);
}

// MSP432 RTC
#if defined(__MSP432P401R__)

void DateTime::begin()
{
    RTC_C_startClock();
}

void DateTime::setTime(uint32_t epoch)
{
    //  MSP432 specific structure   Standard C structure
    //  struct _RTC_C_Calendar      struct tm
    //  {                           {
    //      uint_fast8_t seconds;       int tm_sec;  // seconds after the minute [0-60]
    //      uint_fast8_t minutes;       int tm_min;  // minutes after the hour [0-59]
    //      uint_fast8_t hours;         int tm_hour;  // hours since midnight [0-23]
    //      uint_fast8_t dayOfWeek;     (*)
    //      uint_fast8_t dayOfmonth;    int tm_mday;  // day of the month [1-31]
    //      uint_fast8_t month;         int tm_mon;  // months since January [0-11]
    //      uint_fast16_t year;         int tm_year;  // years since 1900
    //      (*)                         int tm_wday;  // days since Sunday [0-6]
    //                                  int tm_yday;  // days since January 1 [0-365]
    //                                  int tm_isdst;  // Daylight Savings Time flag
    //                                  long tm_gmtoff;  // offset from CUT in seconds
    //                                  char *tm_zone;  // timezone abbreviation
    //  };                          };
    
    // gmtime requires int32_t instead of uint32_t with MSP432
    //    int32_t _i32 = epoch;
    // Convert epoch into standard C structure
    //    _structureRTC = *gmtime(&_i32);
    convertEpoch2Structure(epoch, _structureRTC);
    
    // Convert standard C structure into MSP432 specific structure
    _calendarMSP432.seconds    = _structureRTC.tm_sec;
    _calendarMSP432.minutes    = _structureRTC.tm_min;
    _calendarMSP432.hours      = _structureRTC.tm_hour;
    _calendarMSP432.dayOfWeek  = _structureRTC.tm_wday;
    _calendarMSP432.dayOfmonth = _structureRTC.tm_mday;
    _calendarMSP432.month      = _structureRTC.tm_mon + 1; // tm_mon is 0..11
    _calendarMSP432.year       = _structureRTC.tm_year + 1900;
    
    RTC_C_holdClock();
    // RTC_FORMAT_BINARY not available, so I guess RTC_FORMAT_BINARY = 0
    RTC_C_initCalendar(&_calendarMSP432, 0);
    RTC_C_startClock();
}

uint32_t DateTime::getTime()
{
    _calendarMSP432 = RTC_C_getCalendarTime();
    
    /*
     Serial.print("year = ");
     Serial.println(_calendarMSP432.year, DEC);
     Serial.print("month = ");
     Serial.println(_calendarMSP432.month, DEC);
     Serial.print("dayOfmonth = ");
     Serial.println(_calendarMSP432.dayOfmonth, DEC);
     Serial.print("hours = ");
     Serial.println(_calendarMSP432.hours, DEC);
     Serial.print("minutes = ");
     Serial.println(_calendarMSP432.minutes, DEC);
     Serial.print("second = ");
     Serial.println(_calendarMSP432.seconds, DEC);
     */
    
    // Convert specific structure into standard C structure MSP432
    _structureRTC.tm_sec  = _calendarMSP432.seconds;
    _structureRTC.tm_min  = _calendarMSP432.minutes;
    _structureRTC.tm_hour = _calendarMSP432.hours;
    _structureRTC.tm_wday = _calendarMSP432.dayOfWeek;
    _structureRTC.tm_mday = _calendarMSP432.dayOfmonth;
    _structureRTC.tm_mon  = _calendarMSP432.month - 1; // tm_mon is 0..11
    _structureRTC.tm_year = _calendarMSP432.year - 1900;
    
    // Convert standard C structure into epoch
    //    return mktime(&_structureRTC);
    convertStructure2Epoch(_structureRTC, _epochRTC);
    return _epochRTC;
}

#elif defined(__CC3200R1M1RGC__)

void DateTime::begin()
{
    PRCMRTCInUseSet();
    //    PRCMRTCSet(0, 0);
}

void DateTime::setTime(uint32_t epoch)
{
    PRCMRTCSet(epoch, 0);
}

uint32_t DateTime::getTime()
{
    uint32_t _seconds = 0;
    uint16_t _millis = 0;
    PRCMRTCGet(&_seconds, &_millis);
    return _seconds;
}

#elif defined(__LM4F120H5QR__) || defined(__TM4C123GH6PM__) || defined(__TM4C129XNCZAD__)

void DateTime::begin()
{
    ROM_HibernateEnableExpClk(F_CPU);
    ROM_HibernateRTCEnable();
}

void DateTime::setTime(uint32_t epoch)
{
    ROM_HibernateRTCSet(epoch);
}

uint32_t DateTime::getTime()
{
    return ROM_HibernateRTCGet();
}

#else
#	error Platform not supported.
#endif

#if (INCLUDE_NTP == 1)

bool getTimeNTP(time_t &epochNTP, IPAddress serverNTP)
{
    bool result = false;
    
    // Check WiFi connection
    if (WiFi.localIP() == INADDR_NONE)
    {
        if (Serial) Serial.println("ERROR No WiFi");
        return false;
    }
    
    const uint16_t localPort = 2390;      // local port to listen for UDP packets
    const uint8_t NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
    uint8_t bufferNTP[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
    
    // A UDP instance to let us send and receive packets over UDP
    WiFiUDP myUDPforNTP;
    
    myUDPforNTP.begin(2390);
    
    // Send NTP packet to a time server
    memset(bufferNTP, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // see https://www.meinbergglobal.com/english/info/ntp-packet.htm
    bufferNTP[0] = 0b11100011;   // LI, Version, Mode
    bufferNTP[1] = 0;     // Stratum, or type of clock
    bufferNTP[2] = 6;     // Polling Interval
    bufferNTP[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    bufferNTP[12]  = 49;
    bufferNTP[13]  = 0x4e;
    bufferNTP[14]  = 49;
    bufferNTP[15]  = 52;
    
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    myUDPforNTP.beginPacket(serverNTP, 123); // NTP requests are to port 123
    myUDPforNTP.write(bufferNTP, NTP_PACKET_SIZE);
    myUDPforNTP.endPacket();
    
    delay(1000);
    
    if (myUDPforNTP.parsePacket())
    {
        // Packet received, to be read
        myUDPforNTP.read(bufferNTP, NTP_PACKET_SIZE);
        
        // Timestamp starts at byte 40 of the received packet and is 4 bytes long.
        epochNTP = bufferNTP[40];
        epochNTP <<= 8;
        epochNTP |= bufferNTP[41];
        epochNTP <<= 8;
        epochNTP |= bufferNTP[42];
        epochNTP <<= 8;
        epochNTP |= bufferNTP[43];
        
        // Rebase to 00:00 Jan 1, 1970
        // epoch = number of seconds since 00:00, Jan 1st, 1970 UTC = POSIX time.
        // see http://www.epochconverter.com
        epochNTP -= (uint32_t)2208988800;
        result = true;
    }
    
    myUDPforNTP.stop();
    
    // Failure
    return result;
}

#endif // NTP
