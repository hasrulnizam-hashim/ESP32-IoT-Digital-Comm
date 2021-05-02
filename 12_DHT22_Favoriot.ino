#include <Favoriot.h>
#include <DHT.h>

#define DHTPin    19 
DHT dht(DHTPin,DHT22);

float t,h,temperature,humidity;
unsigned long int timer;

String ssid     = "ijamSiti2G@unifi";           // replace with your WiFi SSID
String password = "0123012469";       // replace with your WiFi password
String apikey   = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6Ikhhc3J1bG5pemFtIiwicmVhZF93cml0ZSI6dHJ1ZSwiaWF0IjoxNTcxOTcxNTE5fQ.QidLy4-RVDq-zirjI1HLRERpfjh-JFEPZUwYBDgD5PE";  // replace with your Favoriot Device Access Token

FavoriotHTTP favoriot;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  favoriot.begin(ssid, password, apikey);
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

  favoriot.deviceId("TempHumid@Hasrulnizam");
  favoriot.dataStream("suhu", String(temperature));
  favoriot.dataStream("kelembapan", String(humidity));
  favoriot.dataStreamEnd();
  
  delay(10000); // update data interval to Favoriot Data Stream using delay function

}
