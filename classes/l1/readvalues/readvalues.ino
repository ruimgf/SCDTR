static int LED = 3;
static int sensor = A5;

int val;
int ledval = 0;
int i;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(ldr,INPUT);
  Serial.begin(9600);
  analogWrite(LED, ledval);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i=1;i <= 255*4 ;i++){

    val = analogRead(ldr);
    Serial.println(val);
    delay(20);
    if(i%4 = 0){// 4 mesures for 1 val on led
      ledval++;
      analogWrite(LED, ledval);
      delay(100);
    }
  }
  
}
