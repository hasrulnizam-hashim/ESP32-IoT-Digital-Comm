#define  DS18S20_Pin  15

#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(DS18S20_Pin);
DallasTemperature sensors(&oneWire);

float temperature;
unsigned long int timer;

void setup()
{
  Serial.begin(115200);
  sensors.begin();
}

void loop()
{
  sensors.requestTemperatures();
  temperature=sensors.getTempCByIndex(0);
  
  if(millis()-timer>=1000)
  {
    timer=millis();
    Serial.println("Temperature="+String(temperature)+"degC");
  }
  delay(1);
}
