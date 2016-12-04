
int ARDUINO_NUMBER=1;
float lux = 10;
float duty = 255;
bool occupancy = 1;
int external_illu = 20;
int illu = 30;
int x;

void setup() {
    Serial.begin(115200);
    /*setup serial*/
    String challenge;
    while(1){

          if(Serial.available() > 0){
            challenge = Serial.readString();
          }

          if (challenge == "W")
          {
              Serial.println(ARDUINO_NUMBER);
              break;
          }
    }
    delay(1000);
}

void loop(){
  Serial.println(duty);
  Serial.readStringUntil('\n');
  Serial.println(lux);
  Serial.readStringUnti('\n');
  Serial.println(micros());
  delay(10);
}
