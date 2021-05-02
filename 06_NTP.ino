#include <WiFiManager.h> 
#include <NTPClient.h>

WiFiManager wifiManager;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"sg.pool.ntp.org",28800,60000);
//other timeserver at https://www.ntppool.org

int ntc[7];
String message;
unsigned long int timestamp;

void setup()
{
  Serial.begin(115200);

  wifiManager.setDebugOutput(false);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setTimeout(60);
  if(!wifiManager.autoConnect("TTGO"))
  {
    Serial.println("Wifi failed to connect and hit timeout");
  }
  else
  {
    Serial.println("Wifi successfully connected.");
  }
  delay(1000);
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); 
  timeClient.begin(); 
}

void loop()
{
  timeClient.update();
  timestamp=timeClient.getEpochTime();
  getDateTime(timestamp);

  switch(ntc[3])
  {
  case 0:{Serial.print("SUN");break;} 
  case 1:{Serial.print("MON");break;} 
  case 2:{Serial.print("TUE");break;} 
  case 3:{Serial.print("WED");break;} 
  case 4:{Serial.print("THU");break;} 
  case 5:{Serial.print("FRI");break;} 
  case 6:{Serial.print("SAT");break;}  
  }
  Serial.print(" ");
 
  ntc[4]<10?Serial.print("0"+String(ntc[4])):Serial.print(ntc[4]);
  Serial.print("/");
  ntc[5]<10?Serial.print("0"+String(ntc[5])):Serial.print(ntc[5]);
  Serial.print("/");
  ntc[6]<10?Serial.print("0"+String(ntc[6])):Serial.print(ntc[6]);
  Serial.print(" ");
 
  ntc[2]<10?Serial.print("0"+String(ntc[2])):Serial.print(ntc[2]);
  Serial.print(":");
  ntc[1]<10?Serial.print("0"+String(ntc[1])):Serial.print(ntc[1]);
  Serial.print(":");
  ntc[0]<10?Serial.print("0"+String(ntc[0])):Serial.print(ntc[0]);
  Serial.println();

  delay(1000);
}

void configModeCallback (WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.println(WiFi.softAPIP());
}

void getDateTime(unsigned long int timestamp)
{
  int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  ntc[0]=timestamp%60;
  ntc[1]=(timestamp%3600)/60;
  ntc[2]=(timestamp%86400)/3600;
  ntc[3]=((timestamp/86400)+4)%7;
  ntc[6]=timestamp/86400/365; //years since 1970
  ntc[4]=timestamp/86400-365*ntc[6]-(ntc[6]+1)/4;  //leap year start at 1972
  if(ntc[4]<0)
  {
    ntc[6]--;
    ntc[4]=timestamp/86400-365*ntc[6]-(ntc[6]+1)/4;  //days for current year
  }
  ntc[6]=(1970+ntc[6])%100;

  ntc[5]=0;
  while(ntc[4]>=days[ntc[5]])
  {
    if(((ntc[6]%4)==0)&(ntc[5]==1)) ntc[4]--; //if leap year and February
    ntc[4]-=days[ntc[5]];
    ntc[5]++;
  }
  ntc[4]++;  //1st day of Month is 0 days
  ntc[5]++;  //get value 1-12  
  if((ntc[4]==0)&(ntc[5]==3))
  {
    ntc[4]=29;
    ntc[5]=2;
  }
}
