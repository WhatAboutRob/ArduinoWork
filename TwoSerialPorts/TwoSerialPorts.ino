int ledpin = 8;
int buttonpin = 30;
bool buttonState = false;


//Serial used to connect to Processing
//Serial1 used to connect to Serial Monitor
//Processing will turn on LED
//Serial Monitor will record when a button is pressed

void setup() {
  // put your setup code here, to run once:
pinMode(ledpin,OUTPUT);
pinMode(buttonpin,INPUT);

//Serial.begin(9600);
//Serial1.begin(9600);




}

void loop() {

digitalWrite(ledpin,HIGH);
delay(1000);
digitalWrite(ledpin,LOW);
delay(100);

//buttonState = digitalRead(buttonpin);

//if (buttonState) {
 //Serial.println("Button has been pressed");
// buttonState = false;
// }
/*
if (Serial.available()){
  char val = Serial.read();

  if(val == '1'){
    digitalWrite(ledpin, HIGH);
  }
  if(val == '2'){
    digitalWrite(ledpin, LOW);
  }
  
}
*/

}
