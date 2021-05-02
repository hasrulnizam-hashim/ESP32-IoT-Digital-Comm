#define DHTPin    13 
#include <DHT.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <SSD1306.h>

DHT dht(DHTPin,DHT22);
float t,h,temperature,humidity;
unsigned long int timer1;
char data;
int progress;
unsigned long int timer2;

WiFiManager wifiManager;
SSD1306  display(0x3c,21,22); //(ADDRESS,SDA,SCL)

void setup()
{
  Serial.begin(115200);
  dht.begin();
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0,"START");
  display.display();

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
  
  if(millis()-timer1>=1000)
  {
    timer1=millis();
    Serial.println("Temperature="+String(temperature)+"degC Humidity="+String(humidity)+"%");
  }
  
  //===============OLED Display======================
  delay(1);
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0,"Projek IoT");
  display.drawString(64,10,"Temp="+String(temperature)+"degC");
  display.drawString(64,20,"Humi="+String(humidity)+"%");
  display.drawString(64,30,"Pondok_IoT");
  display.drawString(64,40,"Pondok_4H");
  display.drawString(64,50,"hasrulnizam.com");
  display.display();
  delay(1000);
  
  timer2=millis();
  while(millis()-timer2<=5000)
  {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0,"IP Address");
    display.drawString(64,15, WiFi.localIP().toString());
    display.drawString(64,30,"millis()");
    display.drawString(64,45, String(millis()));
    display.display(); 
    delay(10);   
  }
  
  display.clear();
  drawRectDemo();
  delay(1000);
  
  display.clear();
  drawCircleDemo();
  delay(1000);

  for(progress=0;progress<=100;progress++)
  {
    display.clear();
    drawProgressBarDemo();
    delay(20);
  }
  delay(1000);
}

void configModeCallback (WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0,"Entered config mode");
  display.drawString(64,30,myWiFiManager->getConfigPortalSSID());
  display.drawString(64,45,WiFi.softAPIP().toString());
  display.display(); 
}

void drawRectDemo()
{
      // Draw a pixel at given position
    for (int i = 0; i < 10; i++) {
      display.setPixel(i, i);
      display.setPixel(10 - i, i);
    }
    display.drawRect(12, 12, 20, 20);

    // Fill the rectangle
    display.fillRect(14, 14, 17, 17);

    // Draw a line horizontally
    display.drawHorizontalLine(0, 40, 20);

    // Draw a line horizontally
    display.drawVerticalLine(40, 0, 20);
    display.display(); 
}

void drawCircleDemo()
{
  for (int i=1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i*3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i* 3);
  }
  display.display(); 
}

void drawProgressBarDemo()
{
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
  display.display(); 
}
