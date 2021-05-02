#define DHTPin    13
#define  DS18S20_Pin  15
#define SensorPin         32     //pH meter Analog output to ADC1 CH4
#define Offset            0.00   //deviation compensate
#define samplingInterval  20
#define printInterval     1000
#define ArrayLenth        40     //times of collection

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <SSD1306.h>

OneWire oneWire(DS18S20_Pin);
DallasTemperature sensors(&oneWire);

DHT dht(DHTPin,DHT22);
float t,h,temperature,humidity,tempds;
char data;
int progress;
unsigned long int timer;

WiFiManager wifiManager;
SSD1306  display(0x3c,21,22); //(ADDRESS,SDA,SCL)

int pHArray[ArrayLenth],pHArrayIndex=0; 
static float pHValue,voltage;
static unsigned long samplingTime,printTime; 

void setup()
{
  Serial.begin(115200);
  sensors.begin();
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
//===========Temp DS18B20==================
  sensors.requestTemperatures();
  tempds=sensors.getTempCByIndex(0);
  
  if(millis()-timer>=1000)
  {
    timer=millis();
    Serial.println("Temp DS18= "+String(tempds)+" degC");
  }
  delay(1);
  
//===========Temp & humidity==================
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

 //===============PHValue======================
  if(millis()-samplingTime>samplingInterval)
  {
    pHArray[pHArrayIndex++]=analogRead(SensorPin);
    if(pHArrayIndex==ArrayLenth) pHArrayIndex=0;
    voltage=avergearray(pHArray,ArrayLenth)*3.3/4095;
    pHValue=3.5*voltage+Offset;
    samplingTime=millis();
  }
  
  if(millis()-printTime>printInterval)
  {
    Serial.print("Voltage: ");
    Serial.print(voltage,2);
    Serial.print("V\tpH: ");
    Serial.println(pHValue,2);
    printTime=millis();
  }  
  
  //===============OLED Display======================
  delay(1);
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0,"Projek IoT-Hasrulnizam");
  display.drawString(0,10,"Temp = "+String(temperature)+" degC");
  display.drawString(0,20,"Humi = "+String(humidity)+" %");
  display.drawString(0,30,"Temp DS18= "+String(tempds)+" degC");
  display.drawString(0,40,"Voltage= "+String(voltage)+"V");
  display.drawString(0,50,"PH Value= "+String(pHValue));
  display.display();
  delay(5000);
  
  timer=millis();
  while(millis()-timer<=5000)
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

//calculate average all sample except min and max value.
//So total sample is deduct 2
double avergearray(int* arr,int number)  
{
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0)
  {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5)
  {   //less than 5, calculated directly statistics
    for(i=0;i<number;i++)
    {
      amount+=arr[i];
    }
    avg=amount/number;
    return avg;
  }
  else
  {
    if(arr[0]<arr[1])
    {
      min=arr[0];
      max=arr[1];
    }
    else
    {
      min=arr[1];
      max=arr[0];
    }
    for(i=2;i<number;i++)
    {
      if(arr[i]<min)
      {
        amount+=min;    //arr<min
        min=arr[i];
      }
      else if(arr[i]>max)
      {
        amount+=max;    //arr>max
        max=arr[i];
      }
      else
      {
        amount+=arr[i]; //min<=arr<=max
      }
    }
    avg=(double)amount/(number-2);
  }
  return avg;
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
