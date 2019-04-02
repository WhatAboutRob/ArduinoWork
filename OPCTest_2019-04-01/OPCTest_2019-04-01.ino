#include <Servo.h>

int triggerPin = 2;
int echoPin = 3;
int servoPin = 10;
Servo servo;

long distance;

void setup() {
  // Initialize pin modes
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  servo.attach(servoPin); // attach the servo to the pin 
  servo.write(120); // Set the servo to mid point 
  Serial.begin(9600); // Begin serial communications with a 9600 baud rate
}

void loop() {
  poll(); //update distance and print to the stream

  //Handle any incoming data
  if(Serial.available()){
    serialInterpreter();
  }
  
  delay(250); //delay next cycle
}

//wait for outgoing serial data to complete transmission and end the connection
void terminate() {
  Serial.flush();
  Serial.end();
}

/* set the servo to the specified position while keeping the value within 
 * the supported range*/
void setServoPos(int pos){
  if (pos < 0){
    pos = 0;
  } else if (pos > 180){
    pos = 180;
  }
  servo.write(pos);
}

//Returns the time it took to detect an echo off of on object in front of the sensor
long ping(int echoPin, int triggerPin)
{
  unsigned long ping;
  
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(echoPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(echoPin, LOW);
  
  ping = pulseIn(triggerPin, HIGH);
  return ping;
}

//update distance and print to the stream
void poll(){
  //Serial.flush();
  //distance=ping(echoPin, triggerPin);
  //Serial.println(distance);
  //Serial.println("22");


  long duration, distance;
  digitalWrite(triggerPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(triggerPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);





  
}

//function to encapsulate handling of incoming transmissions
void serialInterpreter(){
    String line = readLine();
    if (line.startsWith("function: ")) {
      line.replace("function: ", "");
      parseSerialFunction(line);
    }
}

//reads 200 bytes of incoming data up until the new line character is detected
String readLine(){
  char stream[200];
  String line;
  if(Serial.available()){
    Serial.readBytesUntil("\n", stream, 200);
  }
  line = (String)stream;
  line.trim();
  return line;
}

//interprets incoming data intended to call a function stored on the Arduino
void parseSerialFunction(String line){
  int paramStartIndex = line.indexOf('(');
  int paramEndIndex = line.indexOf(')');
  String function = line;
  function.remove(paramStartIndex);
  String paramString = line.substring(paramStartIndex+1,paramEndIndex);

  /* Supported functions logic */
  if(function == "setServoPos") {
    setServoPos(paramString.toInt());
  }

  if(function == "poll") {
    poll();
  }

  if(function == "terminate") {
    terminate();
  }
}
