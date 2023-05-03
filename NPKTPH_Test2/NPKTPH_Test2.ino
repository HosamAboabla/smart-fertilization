#include <SoftwareSerial.h>

 
#define RS485_power 7
#define MYSerialRX 8
#define MYSerialTX 9
 

const byte nitro_inquiry[]   = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos_inquiry[]    = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota_inquiry[]    = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
const byte ph_inquiry[]      = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};


const byte tmp_most_inquiry[]      = {0x01, 0x03, 0x00, 0x12, 0x00, 0x02, 0x65, 0xcb};

byte values[11];
SoftwareSerial MySerial(MYSerialRX , MYSerialTX);
 
void setup() {
  Serial.begin(9600);
  MySerial.begin(4800); // if not working try 9600
  pinMode(RS485_power, OUTPUT);



  delay(3000);
}
 
void loop() {
  int val1,val2,val3 , tmp_val , most_val , ph_val;
  val1 = get_nitrogen();
  delay(250);
  val2 = get_phosphorous();
  delay(250);
  val3 = get_potassium();
  delay(250);

  ph_val = get_ph();
  delay(250);


  most_val = get_most();
  delay(250);

  tmp_val = get_tmp();
  delay(250);
  
  
  Serial.print("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");
  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");



  Serial.print("PH: ");
  Serial.println(ph_val);

  Serial.print("most_val: ");
  Serial.println(most_val);

  Serial.print("tmp_val: ");
  Serial.println(tmp_val);
  delay(2000);
 
  
}
 


int CaculateValue(int x, int y)
{
  int t = 0;
  t = x * 256;
  t = t + y;
  return t;
}


int get_nitrogen(){
  digitalWrite(RS485_power,HIGH);
  delay(10);
  if(MySerial.write(nitro_inquiry,sizeof(nitro_inquiry))==8){
    digitalWrite(RS485_power,LOW);
    for(int i=0;i<7;i++){
    values[i] = MySerial.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
int get_phosphorous(){
  digitalWrite(RS485_power,HIGH);
  delay(10);
  if(MySerial.write(phos_inquiry,sizeof(phos_inquiry))==8){
    digitalWrite(RS485_power,LOW);
    for(int i=0;i<7;i++){
    values[i] = MySerial.read();
    //Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[4];
}
 
int get_potassium(){
  digitalWrite(RS485_power,HIGH);
  delay(10);
  if(MySerial.write(pota_inquiry,sizeof(pota_inquiry))==8){
    digitalWrite(RS485_power,LOW);
    for(int i=0;i<7;i++){
    values[i] = MySerial.read();
    //Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}


int get_ph(){
  digitalWrite(RS485_power,HIGH);
  delay(10);
  if(MySerial.write(ph_inquiry,sizeof(ph_inquiry))==8){
    digitalWrite(RS485_power,LOW);
    for(byte i=0;i<7;i++){
    values[i] = MySerial.read();
    //Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return CaculateValue(values[3] , values[4]);
}

int get_most(){
  digitalWrite(RS485_power,HIGH);
  delay(10);
  if(MySerial.write(tmp_most_inquiry,sizeof(tmp_most_inquiry))==8){
    digitalWrite(RS485_power,LOW);
    for(byte i=0;i<9;i++){
    values[i] = MySerial.read();
    //Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return CaculateValue(values[3] , values[4]);
}




int get_tmp(){
  digitalWrite(RS485_power,HIGH);
  delay(10);
  if(MySerial.write(tmp_most_inquiry,sizeof(tmp_most_inquiry))==8){
    digitalWrite(RS485_power,LOW);
    for(byte i=0;i<9;i++){
    values[i] = MySerial.read();
    //Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return CaculateValue(values[5] , values[6]);
}