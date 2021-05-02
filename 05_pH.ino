#define SensorPin         32     //pH meter Analog output to ADC1 CH4
#define Offset            2.50   //deviation compensate
#define samplingInterval  20
#define printInterval     1000
#define ArrayLenth        40     //times of collection

int pHArray[ArrayLenth],pHArrayIndex=0; 
static float pHValue,voltage;
static unsigned long samplingTime,printTime; 
  
void setup(void)
{ 
  Serial.begin(115200);  
}

void loop(void)
{
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
