#define DHTPin    19 
#include <DHT.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

HTTPClient http;
DHT dht(DHTPin,DHT22);
float t,h,temperature,humidity;
unsigned long int timer;
char data;

WiFiManager wifiManager;
String url;

void setup()
{
  Serial.begin(115200);
  dht.begin();

  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setTimeout(180);
  if(!wifiManager.autoConnect("ESP32-Ijam"))
  {
    Serial.println("failed to connect and hit timeout");
  }
  else
  {
    Serial.println("Successfully connected");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop()
{
  if(Serial.available())
  {
    data=Serial.read();
    if(data=='1')
    {
      WiFi.disconnect(false,true);
      Serial.println("Reset wifi setting and restart");
      wifiManager.resetSettings();
      ESP.restart();     
    }
  }
  
//===========Temp & humidity==================
  t=dht.readTemperature();
  h=dht.readHumidity();
  if(!isnan(t) && !isnan(h))
  {
    temperature = t;
    humidity = h;
  }
  
//  if(millis()-timer>=1000)
//  {
//    timer=millis();
//    Serial.println("Temperature="+String(temperature)+"degC Humidity="+String(humidity)+"%");
//  }

  if(millis()-timer>=500)
  {
    timer=millis();
    Serial.println("Temperature="+String(temperature)+"degC Humidity="+String(humidity)+"%");
    url ="http://hasrulnizam.com/iot/humidity-temperature-data.php?id=test&data";
//    url="http://endpoint.projekiot.com/send?token=08109c26-cd01-4e1f-a3f4-dd6a356c64ea";
    url+="&humidity="+String(humidity);
    url+="&temperature="+String(temperature);
    url+="&interval=10";
    Serial.println(url);
    http.begin(url);
    if(http.GET()==HTTP_CODE_OK) Serial.println(http.getString());
  }
  //delay(1);
}

void configModeCallback (WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
