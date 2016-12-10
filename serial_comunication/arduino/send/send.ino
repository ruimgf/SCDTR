int ARDUINO_NUMBER=2;
float lum = 10;
int duty = 255;
bool occupancy = 1;
int external_illu = 20;
int illu = 30;
int x;
int check=0;

void setup_serial(){
  Serial.begin(115200);
  Serial.print("arduino");
  Serial.println(ARDUINO_NUMBER);
}

void setup() {
    setup_serial();
}


void check_serial(){
    String request;
    request = Serial.readString();
    if(request.charAt(0)=='c'){

      switch (request.charAt(1)) {
      case 'R':
          lum = 0 ; // chamar func de calibracao
      break;
      case 'o': // change oc
        if(request.charAt(2) =='N'){
           lum = 0;
        }else{
          lum = 20;
        }
        break;
      default:
        break;
      }
    }


  return ;
}

void write_values(){

  Serial.print("l");
  Serial.print(lum);
  Serial.print("|");
  Serial.println(duty);

}

void loop(){

  if(Serial.available()>0){
    check_serial();
  }

  write_values();
  duty=duty+1;
  delay(10);
}
