#include <SoftwareSerial.h>
#include <TinyGPS.h>
float flat,flon; // create variable for latitude and longitude object
SoftwareSerial gpsSerial(3,4);//tx,rx
TinyGPS gps; // create gps object

float waitingtime = 0;

void setup(){
Serial.begin(9600); // connect serial
Serial.println("The GPS Received Signal:");
gpsSerial.begin(9600); // connect gps sensor
delay(8000);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}



void loop(){
  
smartdelay(1000);

gps.f_get_position(&flat, &flon);
String latitude = String(flat,6);
String longitude = String(flon,6);
String stime = String(waitingtime,6);
//Serial.println("first read ["+stime+"] = "+latitude+";"+longitude);
Serial.println(latitude+" , "+longitude);
delay(1000); 
waitingtime = waitingtime + 10;

while(gpsSerial.available()){ // check for gps data
  if(gps.encode(gpsSerial.read()))// encode gps data
    {
    gps.f_get_position(&flat,&flon); // get latitude and longitude
    latitude = String(flat,6);
    longitude = String(flon,6);
    Serial.println("While read = "+latitude+";"+longitude);
    delay(1000);
    }
  }
  
}
