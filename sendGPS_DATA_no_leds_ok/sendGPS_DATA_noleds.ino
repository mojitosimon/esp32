#include <SPI.h>
#include "nRF24L01.h"//RF com
#include "RF24.h"//RF com
#include "printf.h"
#include <FastLED.h>//LEDs https://github.com/FastLED/FastLED
#include <SoftwareSerial.h> //GPS
#include <TinyGPS.h> //GPS

//General stuff
int BaudPcConnection = 9600; //is used for serial with pc and GPS
int TIMEOUT = 2000; //ms
float waitingtime = 0;

//GPS stuff
float flat,flon; // create variable for latitude and longitude object
SoftwareSerial gpsSerial(3,4);//tx,rx
TinyGPS gps; // create gps object

// RF  Hardware configuration
RF24 radio(7, 8);  // Set up nRF24L01 radio on SPI bus plus pins 7 & 8
const uint64_t addresses[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

//leds stuff

//role stuff
const short role_pin = 5;  // use pin 5
typedef enum { role_ping_out = 1,
               role_pong_back } role_e;                                     // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back" };  // The debug-friendly names of those roles
role_e role;                                                                // The role of the current running sketch

/////////////FUNCTIONS/////////////
char receive_payload[max_payload_size + 1];  // +1 to allow room for a terminating NULL char

double distanceDeg(double lat1, double long1, double lat2, double long2){
  double lat = lat1 - lat2;
  double lng = long1 - long2;
  return sqrt(lat * lat + lng * lng);
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

String getGPSData(){
    //Serial.println("Get Gps Data");
    gps.f_get_position(&flat, &flon);
    String latitude = String(flat,6);
    String longitude = String(flon,6);
    Serial.println("GPS data: "+latitude+" , "+longitude);

//    while(gpsSerial.available()){ // check for gps data
//    if(gps.encode(gpsSerial.read()))// encode gps data
//        {
//        gps.f_get_position(&flat,&flon); // get latitude and longitude
//        latitude = String(flat,6);
//        longitude = String(flon,6);
//        Serial.println("While read = "+latitude+";"+longitude);
//        delay(1000);
//        }
//    }
//   // Serial.println("Get Gps Data END");
    return latitude+" , "+longitude;
}


///////////////////////////////////////SETUP///////////////////////////////////////////////////////////////////
void setup(void) {
  pinMode(role_pin, INPUT);  // set up the role pin
  digitalWrite(role_pin, HIGH);
  delay(20);  // Just to get a solid reading on the role pin
 
  // read the role_pin, establish our role
  if (digitalRead(role_pin)) {
    role = role_ping_out;
  } else {
    role = role_pong_back;
  }
 
  Serial.begin(BaudPcConnection);
  printf_begin();  // needed for printDetails()
 
  // Print preamble
  Serial.println(F("-------------------/"));
  Serial.print(F("ROLE: "));
  Serial.println(role_friendly_name[role]);
 
  // Setup and configure rf radio
  if (!radio.begin()) {
  Serial.println(F("radio hardware not responding!"));
  while (1) {} // hold program in infinite loop to prevent subsequent errors  
    }
  radio.enableDynamicPayloads();  // Enable dynamic payloads
  radio.setRetries(5, 15);        // delay between retries = 5 * 250 + 250 = 1500 microseconds, number of retries = 15
 
  // Open a writing and reading pipe on each radio, with opposite addresses
  if (role == role_ping_out) {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  } else {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  }
 
  radio.startListening();  // Start listening
  radio.printDetails();    // Dump the configuration of the rf unit for debugging

   Serial.println("GPS Init :");
   gpsSerial.begin(BaudPcConnection); // connect gps sensor
   delay(3000);


}

///////////////////////////////////////LOOP///////////////////////////////////////////////////////////////////
void loop() { 
  smartdelay(1000);
  String send_payload;
  send_payload = getGPSData();

  int str_len = send_payload.length() + 1; 

  // Prepare the character array (the buffer) 
  char char_array[str_len];

  // Copy it over 
  send_payload.toCharArray(char_array, str_len);
  bool timeout = false;
  uint8_t len = 0;
  /////////////////////////////////////////////////////////COMMUNICATION///////////////////////////////////
  if (role == role_ping_out) {//send first recieved later
    // Send the payload
    radio.stopListening();  // First, stop listening so we can talk.
    Serial.print(F("Now sending length "));
    Serial.println(send_payload_size);
    radio.write(char_array, send_payload_size);  // This will block until complete
    radio.startListening();  // Now, continue listening
    //end send

    unsigned long started_waiting_at = millis();  // Start a timer for measuring timout
    unsigned long timeping =  0;
    while (!radio.available() && !timeout)  // Wait until we get a response or timeout is reached
    {
      timeping = millis() - started_waiting_at;
      if (timeping > TIMEOUT)  // Only wait for 500 milliseconds
        timeout = true;
      }
      if (timeout) {
            Serial.println(F("Ping failed, response timed out."));
            return;
            } 
   
      len = radio.getDynamicPayloadSize();  // get payload's length
       if (!len)
        {
        Serial.println(F("Ping failed, empty response."));
        return;
        }
      radio.read(receive_payload, len);
 
      // Use payload as a C-string (for easy printing)
      receive_payload[len] = 0;  // put a NULL terminating zero at the end
 
     }//if role
     ///////////////////////////////////
     else if (role == role_pong_back) {
      unsigned long started_waiting_at = millis();  // Start a timer for measuring timout
      unsigned long timeping =  0;
      while (!radio.available() && !timeout)  // Wait until we get a response or timeout is reached
        {
          timeping = millis() - started_waiting_at;
          if (timeping > TIMEOUT)  // Only wait for 500 milliseconds
            timeout = true;
        }
      if (timeout) {
            Serial.println(F("Pong Back failed, response timed out."));
            return;
            } 
   
       len = radio.getDynamicPayloadSize();  // get payload's length
       if (!len)
        {
        Serial.println(F("Pong Back failed, empty response."));
        return;
        }
      radio.read(receive_payload, len);
 
      // Use payload as a C-string (for easy printing)
      receive_payload[len] = 0;  // put a NULL terminating zero at the end
 
      // Send the payload
      radio.stopListening();  // First, stop listening so we can talk.
      Serial.print(F("Now sending length "));
      Serial.println(send_payload_size);
      radio.write(char_array, send_payload_size);  // This will block until complete
      radio.startListening();  // Now, continue listening
      //end send

      }//else role

    /////////////////////////////////////COMPUTATION
    // Show response
    Serial.print("received_payload :");
    Serial.println(receive_payload);
    Serial.print("send_payload :");
    Serial.println(send_payload);
    
    long int lat1, lat2, long1, long2;
    long int lat3, lat4, long3, long4;
    sscanf(receive_payload, "%ld.%ld , %ld.%ld", &lat1, &lat2, &long1, &long2);
    float latitude1 = lat1 + ( lat2 / 1000000.0);
    float longitude1 = long1 + ( long2 / 1000000.0);

    sscanf(char_array, "%ld.%ld , %ld.%ld", &lat3, &lat4, &long3, &long4);
    float latitude2 = lat3 + ( lat4 / 1000000.0);
    float longitude2 = long3 + ( long4 / 1000000.0);
    if(latitude2 == 1000.00) {
        latitude2 = 48.696969;//dummy values for test
        longitude2 = 2.116969; 
      }
    Serial.println(F("Our poisiton :")); 
    Serial.print(latitude2,6);
    Serial.print(", ");
    Serial.println(longitude2,6);
    Serial.println("Their poisiton :"); 
    Serial.print(latitude1,6);
    Serial.print(", ");
    Serial.println(longitude1,6);
    //code de toute beaute
    float pi = 3.14159/180.0;
    float l1 = latitude1*pi;//48.691154*pi;
    float l2 = longitude1*pi;//2.120305*pi;
    float l3 = latitude2*pi;
    float l4 = longitude2*pi;
    float dlat = l1-l3;
    float dlon = l2-l4;
    //haversine method
    float a = sin(dlat/2) * sin(dlat/2) + cos(l1)*cos(l3)*sin(dlon/2)*sin(dlon/2);
    float c = 2* atan2(sqrt(a), sqrt(1-a));
    float d = 6378.137*c; 
    
    Serial.print("DISTANCE BETWEEN ARDUINOS IS ");//km
    Serial.println(d*1000.0,4);

    if(d>10 || d<0)//fake values
      {
      
      }


    Serial.print("\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n");
}  // loop
