#include <math.h>


#define LED 3
#define LDR A5


int read;
float lux;


float calc_lux_ldr(int read){

  float R_LDR;
  float volt;
  float R = 10000;
  float lux;

  volt = read/1024.0 * 5;
  //Serial.print("Volt:");
  //Serial.println(volt);
  R_LDR = ((5-volt)*R)/volt ;
  //Serial.print("R_LDR:");
  //Serial.println(R_LDR);
  lux = (pow(10,( ( log10(R_LDR) - 4.1 ) / (-0.7332337683) ))) / 0.093;
  return lux;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  analogWrite(LED, 100);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  read = analogRead(LDR);
  //Serial.print("Read val:");
  //Serial.println(read);
  Serial.print("Lux:");
  lux = calc_lux_ldr(read);
  Serial.println(lux);

  delay(2000);

}
