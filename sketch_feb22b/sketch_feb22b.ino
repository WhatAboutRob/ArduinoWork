int buzzerPin = 30;
int counter;
bool StartTest;



void setup() {

  pinMode(buzzerPin, OUTPUT);   //set pin as output
  Serial.begin(9600);    //start serial communication @9600 bps
  counter = 0;
  Serial.println("Test is beginning");
  StartTest = false;
  
  }


void PrintTestLog(){
  Serial.println(counter);
}

void BuzzerTest(){
  digitalWrite(buzzerPin, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  delay(1000);
}



void loop(){
  
  if(Serial.available()){  //id data is available to read

    char val = Serial.read();

    if(val == '1'){      
      BuzzerTest();
      Serial.println("Start");
      }

    if(val == 'B'){      
      StartTest = true;
      }      

     if(val == 'E'){      
      StartTest = false;
      }   

    if (StartTest){
      PrintTestLog();
    }
      
    }
  }
