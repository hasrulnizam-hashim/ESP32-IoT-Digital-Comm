#define DHTPin    19 

#include <DHT.h>

DHT dht(DHTPin,DHT22);

float t,h,temperature,humidity;
unsigned long int timer;

void setup()
{
  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  t=dht.readTemperature();
  h=dht.readHumidity();
  if(!isnan(t) && !isnan(h))
  {
    temperature = t;
    humidity = h;
  }
  
  if(millis()-timer>=1000)
  {
    timer=millis();
    Serial.println("Temperature="+String(temperature)+"degC Humidity="+String(humidity)+"%");
  }
  delay(1);
}
