#include <SoftwareSerial.h>



#define RS485_power 4
#define MYSerialRX 5
#define MYSerialTX 6



SoftwareSerial MySerial(MYSerialRX , MYSerialTX);


const byte ec[] = {0x01,0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
unsigned char values[80] = {0};
double ec_value;


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  MySerial.begin(9600);


  pinMode(RS485_power, OUTPUT);
  
  delay(50);

  digitalWrite(RS485_power, LOW);
  delay(1000);
}

void loop()
{

  //********************GET EC value *******************
  ec_value = calculate_ec();

  Serial.print("EC= ");
  Serial.print(ec_value);
  Serial.println(" mS/cm");
 
  delay(1000);
}



int CaculateValue(int x, int y)
{
  int t = 0;
  t = x * 256;
  t = t + y;
  return t;
}



double calculate_ec(){
  double result;
  
  // enable writing to mx485
  digitalWrite(RS485_power,HIGH);
  delay(10);
  
  if( MySerial.write(ec,sizeof(ec)) == 8 ){
    // enable reading from mx485
    digitalWrite(RS485_power,LOW);
    
    for(byte i=0; i<7 ;i++){
      values[i] = MySerial.read();
      Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  result = (double) CaculateValue((int) values[3],(int) values[4]) / (double) 1000;
  return result;
}
