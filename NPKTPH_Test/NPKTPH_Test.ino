// Vincent Fix 2023/3/1

#include <SoftwareSerial.h>



#define RS485_power 7
#define MYSerialRX 8
#define MYSerialTX 9




SoftwareSerial MySerial(MYSerialRX , MYSerialTX);

// unsigned char test_command[8] = {0X01, 0X03, 0X00, 0X00, 0X00, 0X07, 0X04, 0X08};
// New Sensor 2023/3/1
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

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(4800);

  MySerial.begin(4800);






  pinMode(RS485_power, OUTPUT);
  
  delay(50);

  digitalWrite(RS485_power, HIGH);
  delay(1000);

  Serial.println(" Test Begin!");



  MySerial.write(test_command, 8);

  delay(1000);

  MySerial.write(test_command, 8);

  delay(1000);
}

void loop()
{
  Serial.println("Send loop");
  //********************GET Temperature, Moisture and PH*******************
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

  Serial.print("Answer Length:");
  Serial.println(i);

  for (int j = 0; j < 19; j++)
  {
    Serial.print((int)test_response[j]);
    Serial.print("  ");
  }
  Serial.print("\n");

  moisture = CaculateValue((int)test_response[3], (int)test_response[4]);
  moisture_value = moisture * 0.1;
  tem = CaculateValue((int)test_response[5], (int)test_response[6]);
  tem_value = tem * 0.1;
  ph = CaculateValue((int)test_response[9], (int)test_response[10]);
  ph_value = ph * 0.1;

  Serial.print("moisture:");
  Serial.println(moisture);
  Serial.print("moisture_value:");
  Serial.println(moisture_value);
  Serial.print("tem_value:");
  Serial.println(tem_value);
  Serial.print("ph_value:");
  Serial.println(ph_value);


  delay(1000);

  //****************GET Nitrogen, Phosphorus and Potassium

  N_value = CaculateValue((int)test_response[11], (int)test_response[12]);
  P_value = CaculateValue((int)test_response[13], (int)test_response[14]);
  K_value = CaculateValue((int)test_response[15], (int)test_response[16]);

  Serial.print("N= ");
  Serial.print(N_value);
  Serial.println(" mg/kg");
  Serial.print("P= ");
  Serial.print(P_value);
  Serial.println(" mg/kg");
  Serial.print("K= ");
  Serial.print(K_value);
  Serial.println(" mg/kg");


  delay(1000);
}



int CaculateValue(int x, int y)
{
  int t = 0;
  t = x * 256;
  t = t + y;
  return t;
}

