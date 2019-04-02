int buzzerpin = 30;
int buttonpin = 40;
int buttonState = 0;


//Serial used to connect to Processing
//Serial1 used to connect to Serial Monitor
//Processing will turn on LED
//Serial Monitor will record when a button is pressed

void setup() {
  // put your setup code here, to run once:
pinMode(buzzerpin,OUTPUT);
pinMode(buttonpin,INPUT);
Serial1.begin(9600);


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

