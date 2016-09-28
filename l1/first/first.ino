
static int led = 3;
static int ldr = A5;

int val;
int ledval;


void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(ldr,INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(ldr);
  Serial.println(val);
  ledval = map(val,600,950,255,0);

  analogWrite(led, ledval);
  delay(100);

}
