int ledpin = 10;
int buttonpin = 8;
bool buttonState = false;


void setup() {
pinMode(ledpin,OUTPUT);
pinMode(buttonpin, INPUT);

digitalWrite(ledpin,HIGH);
delay(1000);
digitalWrite(ledpin,LOW);
delay(100);
Serial.begin(9600);
Serial3.begin(9600);
}

void loop() {


buttonState = digitalRead(buttonpin);

if (buttonState){
  Serial3.println("button pressed");
  digitalWrite(ledpin,HIGH);
  delay(100);
  digitalWrite(ledpin,LOW);
  delay(100);
}
/*
if (Serial.available()){
  char val = Serial.read();

  if(val == '1'){
    digitalWrite(ledpin, HIGH);
  }
  if(val == '2'){
    digitalWrite(ledpin, LOW);
  }
  */
 
}

