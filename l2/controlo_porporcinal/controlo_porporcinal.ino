#include <math.h>


#define LED 3
#define LDR A5


int read;
float lux;
int new_lux;
int pwm_value;


float calc_lux_ldr(int read){

  float R_LDR;
  float volt;
  float R = 10000;
  float lux;

  volt = read/1024.0 * 5;

  R_LDR = ((5-volt)*R)/volt ;

  lux = (pow(10,( ( log10(R_LDR) - 4.1 ) / (-0.7332337683) ))) / 0.093;

  return lux;
}

int value_to_write(float desired_lux){

  float m = 0.6947;
  float b = -1.0189;


  return (int)((desired_lux-b)/m);


}
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  analogWrite(LED, value_to_write(50));
  Serial.begin(9600);
  pwm_value = value_to_write(50);
  new_lux = 50;

}
int i = 0;
void loop() {
  // put your main code here, to run repeatedly:

  read = analogRead(LDR);
  if(Serial.available() > 0){

    new_lux = Serial.parseInt();
    pwm_value = value_to_write(new_lux);
    analogWrite(LED, pwm_value);

  }else{
    if(abs(lux - new_lux) > 2 ){
      if(lux < new_lux){
        pwm_value++;
        analogWrite(LED, pwm_value);
      }
      if(lux > new_lux){
        pwm_value--;
        analogWrite(LED, pwm_value);
      }

    }

  }

  lux = calc_lux_ldr(read);



  if(i >= 2000/50 ){
    Serial.print("Lux:");
    Serial.println(lux);
    i=0;
  }
  i++;

  delay(50);

}
