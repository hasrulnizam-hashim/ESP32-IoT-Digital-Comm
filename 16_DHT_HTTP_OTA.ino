#define DHTPin    19 
#include <DHT.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define LED       2
#define INTERVAL  500

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
  Serial.println("Booting");
  pinMode(LED,OUTPUT);
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

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("esp32@ijam");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
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
