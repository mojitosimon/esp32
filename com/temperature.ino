#include "OneWire.h"
#include "DallasTemperature.h"
 
OneWire oneWire(A1);
DallasTemperature ds(&oneWire);

void setupTemp(){
    ds.begin();          // sonde activée
}

void loopTemp(){
  ds.requestTemperatures();
  int t = ds.getTempCByIndex(0);
 
  Serial.print(t);
  Serial.println( "C");
 // screenPrint(t+"C");
}