#include <SoftwareSerial.h>
#include <String.h>

#define RS485_power 7
#define MYSerialRX 8
#define MYSerialTX 9

#define VAVLE1       13



SoftwareSerial gprsSerial(2,3); //SIM900 Tx & Rx is connected to Arduino #7 & #8
SoftwareSerial MySerial(MYSerialRX , MYSerialTX);

unsigned char test_command[8] =
    {0X01, 0X04, 0X00, 0X00,
     0X00, 0X07, 0XB1, 0XC8}; 
unsigned char test_response[80] = {0};

int moisture;
int tem;
int ph;
float moisture_value;
float tem_value;
float ph_value;
int P_value;
int N_value;
int K_value;
float P_float_value;
float N_float_value;
float K_float_value;
double EC_val;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  gprsSerial.begin(9600);               // the GPRS baud rate   
  MySerial.begin(4800);


  //####################### Valves
  pinMode(VAVLE1 , OUTPUT);



  pinMode(RS485_power, OUTPUT);
  
  delay(50);

  digitalWrite(RS485_power, HIGH);
  delay(1000);

}

void loop()
{
  // enable writing to mx485
  digitalWrite(RS485_power,HIGH);
  MySerial.write(test_command, 8);
  // enable reading from mx485
  digitalWrite(RS485_power,LOW);
 
  int i = 0;
  while (MySerial.available() > 0 && i < 80)
  {
    test_response[i] = MySerial.read();
    i++;
    yield();
  }

  moisture = CaculateValue((int)test_response[3], (int)test_response[4]);
  moisture_value = moisture * 0.1;
  tem = CaculateValue((int)test_response[5], (int)test_response[6]);
  tem_value = tem * 0.1;
  ph = CaculateValue((int)test_response[9], (int)test_response[10]);
  ph_value = ph * 0.1;


  Serial.print("moisture_value:"); Serial.println(moisture_value);
  Serial.print("tem_value:"); Serial.println(tem_value);
  Serial.print("ph_value:"); Serial.println(ph_value);


  //****************GET Nitrogen, Phosphorus and Potassium

  N_value = CaculateValue((int)test_response[11], (int)test_response[12]);
  P_value = CaculateValue((int)test_response[13], (int)test_response[14]);
  K_value = CaculateValue((int)test_response[15], (int)test_response[16]);

  Serial.print("N= "); Serial.print(N_value); Serial.println(" mg/kg");
  Serial.print("P= "); Serial.print(P_value); Serial.println(" mg/kg");
  Serial.print("K= "); Serial.print(K_value); Serial.println(" mg/kg");



  //**************** GET EC Value
  int analogValue = analogRead(A0);
  float voltage = analogValue * (5.0 / 1023.0);
  EC_val = voltage * 4000;

  Serial.print("EC: "); Serial.print(EC_val); Serial.println("mS/cm");


  delay(2000);

  //####################### Valves

  digitalWrite(VAVLE1, !digitalRead(VAVLE1));





  //######################################################### GSM

         

   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"jawwalnet.com\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=LGKD6S5PFHHQOF2R&field1=" + String(11);

  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
}



int CaculateValue(int x, int y)
{
  int t = 0;
  t = x * 256;
  t = t + y;
  return t;
}


void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
}
