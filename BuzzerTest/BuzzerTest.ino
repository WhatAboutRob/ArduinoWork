int buzzerpin = 30;
int buttonpin = 40;
int buttonState = 0;




void setup() {
  // put your setup code here, to run once:
pinMode(buzzerpin,OUTPUT);
pinMode(buttonpin,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonpin);
 while (buttonState){ 
  digitalWrite(buzzerpin,LOW);
  delay(100);
  digitalWrite(buzzerpin,HIGH);
  delay(100);
  buttonState = digitalRead(buttonpin);
  digitalWrite(buzzerpin,LOW);
  
 }

}
