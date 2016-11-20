
int ARDUINO_NUMBER=1;
int lum = 10;
int duty = 255;
bool occupancy = 1;
int external_illu = 20;
int illu = 30;
int x;

void setup_serial(){
  Serial.begin(115200);           // start serial for output
  String challenge;
  while(1){

        if(Serial.available() > 0){
          challenge = Serial.readString();
        }

        if (challenge == "Who")
        {
            Serial.print("I am arduino ");
            Serial.println(ARDUINO_NUMBER);
            break;
        }
        delay(100);
  }

}

void setup() {
    setup_serial();
}


void check_serial(){
    String request;
    request = Serial.readString();

    if(request.charAt(0)=='g'){

      switch (request.charAt(2)) {
      case 'l': // response with current lummens at this desk
        Serial.print("l ");
        Serial.println(lum);
        break;
      case 'd': // get duty cycle
        Serial.print("d ");
        Serial.println(duty);
        break;
      case 'o': // get occupancy
        Serial.print("o ");
        Serial.println(occupancy);
        break;
      case 'L': // get current iluminance lower bound at desk
        Serial.print("o ");
        Serial.println(illu);
        break;
      case 'O': // get current external illuminance at desk
        Serial.print("O ");
        Serial.println(external_illu);
        break;
      case 'r': // get current external illuminance at desk
        Serial.print("r ");
        Serial.println(external_illu);
        break;
      default:
        break;
      }
    }


  return ;
}

void loop() {

  if(Serial.available()>0){
    check_serial();
  }

}
