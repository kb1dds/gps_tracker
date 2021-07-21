// Save GPS track data to SD card
// 
// Assumptions:
// 9600 baud serial GPS connected as follows:
//  RX   :  D3
//  TX   :  D4
//  
// SD card attached to SPI bus as follows:
//  MOSI :  D11
//  MISO :  D12
//  CLK  :  D13
//  CS   :  D10
//
// Copyright (c) 2021 Michael Robinson

#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define SD_CS_PIN 10 // Chip select for SD card

#define GPS_RX_PIN 3
#define GPS_TX_PIN 4
#define GPS_BAUD 9600

// SD card details for track data
File fp;

// GPS setup
TinyGPS gps;
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);

static void smartdelay(unsigned long ms);

// Delays are good for polling GPS data
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


void setup()
{
  char filename[50];
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;

  Serial.begin(9600);
  ss.begin(9600);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("NO SD.");
  }

  // Wait for GPS fix...
  Serial.println("Waiting for GPS fix.");
  while(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES)
  {
    smartdelay(1000);  // Note: this is polling the serial line...
  }

  Serial.println("GPS fix obtained.");

  // Unpack date-time for track filename
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    sprintf(filename,"track.csv");
  else
  {
    //sprintf(filename, "%04d-%02d-%02dT%02d_%02d_%02d.csv",year,month, day, hour, minute, second);  // Apparently SD library only supports 8.3 filenames.  Hmph.
    sprintf(filename, "%02d:%02d.csv",hour, minute);
  }

  Serial.print("Opening track file: ");
  Serial.println(filename);

  fp=SD.open(filename,FILE_WRITE);
  if( !fp ){
    Serial.println("Error opening track file");
  }

  fp.print("year");
  fp.print(",");
  fp.print("month");
  fp.print(",");
  fp.print("day");
  fp.print(",");
  fp.print("hour");
  fp.print(",");
  fp.print("minute");
  fp.print(",");
  fp.print("second");
  fp.print(",");
  fp.print("hundredths");
  fp.print(",");
  fp.print("latitude");
  fp.print(",");
  fp.print("longitude");
  fp.print(",");
  fp.print("altitude");
  fp.print(",");
  fp.print("course");
  fp.print(",");
  fp.println("speed");
  fp.flush();
}

void loop()
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  float flat, flon;
  char posstr[15];
  
  smartdelay(700);

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  gps.f_get_position(&flat, &flon, &age);
  fp.print(year);
  fp.print(",");
  fp.print(month);
  fp.print(",");
  fp.print(day);
  fp.print(",");
  fp.print(hour);
  fp.print(",");
  fp.print(minute);
  fp.print(",");
  fp.print(second);
  fp.print(",");
  fp.print(hundredths);
  fp.print(",");
  dtostrf(flat,3,7,posstr);
  fp.print(posstr);
  fp.print(",");
  dtostrf(flon,3,7,posstr);
  fp.print(posstr);
  fp.print(",");
  fp.print(gps.f_altitude());
  fp.print(",");
  fp.print(gps.f_course());
  fp.print(",");
  fp.println(gps.f_speed_kmph());
  fp.flush();
  
  Serial.print(year);
  Serial.print(",");
  Serial.print(month);
  Serial.print(",");
  Serial.print(day);
  Serial.print(",");
  Serial.print(hour);
  Serial.print(",");
  Serial.print(minute);
  Serial.print(",");
  Serial.print(second);
  Serial.print(",");
  Serial.print(hundredths);
  Serial.print(",");
  dtostrf(flat,3,7,posstr);
  Serial.print(posstr);
  Serial.print(",");
  dtostrf(flon,3,7,posstr);
  Serial.print(posstr);
  Serial.print(",");
  Serial.print(gps.f_altitude());
  Serial.print(",");
  Serial.print(gps.f_course());
  Serial.print(",");
  Serial.println(gps.f_speed_kmph());
}
