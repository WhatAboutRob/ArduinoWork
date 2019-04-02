int buzzerpin = 30;
int buttonpin = 40;
int buttonState = 0;




void setup() {
  // put your setup code here, to run once:
pinMode(buzzerpin,OUTPUT);
pinMode(buttonpin,INPUT);

}

void loop() {

buttonState = digitalRead(buttonpin);

if (buttonState) 
  BuzzerTest();
  
 }

void BuzzerTest() {
    digitalWrite(buzzerpin,LOW);
    delay(100);
    digitalWrite(buzzerpin,HIGH);
    delay(100);
    digitalWrite(buzzerpin,LOW);  
}

