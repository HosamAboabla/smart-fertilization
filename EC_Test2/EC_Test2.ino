double EC_val;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(A0);
  float voltage = analogValue * (5.0 / 1023.0);
  EC_val = voltage * 4000;


  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("V");

  Serial.print("EC: ");
  Serial.print(EC_val);
  Serial.println("mS/cm");


  delay(2000);
}
