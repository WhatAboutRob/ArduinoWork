int buttonPin = 40;
int buttonState = 11;
bool runonce = true;



void setup(){
Serial.begin(9600);
Serial.println("CLEARDATA");
Serial.println("LABEL,Acolumn,Bcolumn,...");
Serial.println("RESETTIMER");
}


void loop(){
buttonState = digitalRead(buttonPin);

  if (buttonState){
    if (runonce){
    Serial.print("DATA,TIME,TIMER,");
    Serial.println(buttonState);
    delay(10);
    runonce=false;
    }
  }
   if (!buttonState){
    runonce = true; 
   }
  }



