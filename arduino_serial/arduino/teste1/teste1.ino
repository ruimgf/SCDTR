
int ARDUINO_NUMBER=1;
int lum = 10;
int duty = 255;
bool occupancy = 1;
int external_illu = 20;
int illu = 30;
int x;

void setup() {
    Serial.begin(115200);
}

void loop(){
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
}
