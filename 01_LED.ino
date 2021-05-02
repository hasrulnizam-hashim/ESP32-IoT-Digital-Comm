#define LED       14

unsigned long int timer;

void setup()
{
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
}

void loop()
{
  if(millis()-timer>=1000)
  {
    timer=millis();
    digitalWrite(LED,!digitalRead(LED));
    if(digitalRead(LED)==LOW) Serial.println("LED OFF");
    else Serial.println("LED ON");
  }
  delay(1);
}
