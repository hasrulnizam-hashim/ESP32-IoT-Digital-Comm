#define ID      7
#define TX      16
#define RX      17
#define Lora    Serial1

int counter;
String message, temp;
unsigned long int timer;

void setup()
{
  Serial.begin(115200);
  Lora.begin(9600,SERIAL_8N1,RX,TX);    //8 bit, non parity, 1 stop bit
}

void loop()
{ 
  if(Serial.available()){
    while(Serial.available()) message+=(temp)Serial.read();
  }
  
  if(Lora.available())
  {
    message="";
    while(Lora.available()) message+=(char)Lora.read();
    Serial.println(message);
    if(message.indexOf("id"+String(ID))>-1)
    {
      counter++;
      Serial.println(message+":"+String(counter));
      Lora.print("id"+String(ID)+":"+String(counter)+temp);
    }
  }
  delay(1);
}
