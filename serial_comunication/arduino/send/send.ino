int ARDUINO_NUMBER=1;

void setup() {
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
int x;
void loop() {
  Serial.print("Hello World ");
  Serial.println(x);
  x++;
  delay(1000);

}
