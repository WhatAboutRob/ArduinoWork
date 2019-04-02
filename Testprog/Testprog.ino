int buzzerpin = 30;
int buttonpin = 40;
int button2pin = 38;
int ledpin = 10;
bool buttonState = false;
bool button2State = false;
int i = 0;
bool runonce = false;

void setup() {
  pinMode(buzzerpin,OUTPUT);
  pinMode(buttonpin,INPUT);
  pinMode(button2pin, INPUT);
  pinMode(ledpin, OUTPUT);
  Serial.begin(9600);    //start serial communication @9600 bps

  digitalWrite(ledpin, LOW);
  delay(100);
  digitalWrite(ledpin, HIGH);
  delay (200);
  digitalWrite(ledpin, LOW);
  
  
  }

void BuzzerTest() {
    digitalWrite(buzzerpin,LOW);
    delay(100);
    digitalWrite(buzzerpin,HIGH);
    delay(100);
    digitalWrite(buzzerpin,LOW);  
    
}


void loop(){

buttonState = digitalRead(buttonpin);
button2State = digitalRead(button2pin);



if (buttonState){
  if (runonce){
    BuzzerTest();
    runonce = false;
  }

  if (!buttonState){
    runonce = true;
  }
}

if (button2State){
  Serial.begin(9600);
  digitalWrite(ledpin, LOW);
  delay(100);
  digitalWrite(ledpin, HIGH);
  delay (200);
  digitalWrite(ledpin, LOW);  
}



  
  if(Serial.available()){ //id data is available to read

    char val = Serial.read();

    if(val == '1'){       //if r received
      BuzzerTest();
      }

    if (val =='2'){
      Serial.begin(9600);
    }

    if(val == '3'){       //if f received
      Serial.end();
      }      
    }

  }
    

