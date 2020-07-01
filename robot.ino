#include <NewPing.h>
#include <Servo.h>
#include <String.h>
#include <SoftwareSerial.h>

//Pinii motor1 
#define mpin00 5
#define mpin01 6

//Pinii motor2
#define mpin10 3
#define mpin11 11

//Sensor pins
#define trig_pin 13 //analog input1
#define echo_pin 12 //analog input2 

#define maximum_distance 100
boolean goesForward = false;
int distance = 50;

String inputStringTimp = "";
String inputStringDirectie="";
boolean stringComplete = false;
boolean readSpace=false;
//pini BT
const byte rxPin =10;
const byte txPin =9;

SoftwareSerial mySerial(rxPin, txPin);
int timp=0;


NewPing sonar(trig_pin,echo_pin,maximum_distance); //sensor function
Servo srv;

void setup() {

  //Pentru ocolire obstacole
  
    pinMode(mpin00,OUTPUT);
    pinMode(mpin01,OUTPUT);
    pinMode(mpin10,OUTPUT);
    pinMode(mpin11,OUTPUT);

    //pin LED
    //pinMode(13,OUTPUT);

    srv.attach(8);

    srv.write(90);
    delay(2000);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);  

    //Pentru citire date pe bluetooth
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(txPin, OUTPUT);
    pinMode(rxPin, INPUT);


    //Pentru cuvant
    inputStringTimp.reserve(200);
    inputStringDirectie.reserve(200);
 
    
}

void loop()
{
    serialEvent1();
  if(stringComplete==true)
  {
    
    timp=atoi(inputStringTimp.c_str());
    //timp=inputStringTimp.toInt();
    //Serial.write(timp);
    Serial.write(inputStringDirectie.c_str());
    //inputStringDirectie.trim();
    //Serial.write(inputStringTimp.c_str());
    
   // delay(100);
    if(inputStringDirectie.compareTo("r")){
        moveStop();
         delay(300);
        turnRight();
        delay(300);
        moveStop();
         delay(300);
      }

      if(inputStringDirectie.compareTo("l")){
        moveStop();
        delay(300);
        turnLeft();
        delay(300);
        moveStop();
         delay(300);
      }

      if(inputStringDirectie.compareTo("b")){
        moveStop();
        delay(300);
        turnRight();
         moveStop();
        delay(300);
        turnRight();
         moveStop();
        delay(300);
       
      }
     timp=10;
    while (timp > 0)
    {
      //pentru ocolire obstacole
      int distanceRight = 0;
      int distanceLeft = 0;
      delay(50);
       
      if (distance <= 30)
      {
        moveStop();
        delay(300);
        moveBackward();
        delay(400);
        moveStop();
        delay(300);
        distanceRight = lookRight();
        delay(300);
        distanceLeft = lookLeft();
        delay(300);
    
        if (distance >= distanceLeft)
        {
          turnRight();
          moveStop();
        }
        else 
        {
          turnLeft();
          moveStop();
        }
      }
      else 
      {
        moveForward();
        timp--;
      }
      distance = readPing();
    
    }

    if(timp==0){
      stringComplete=true;
      inputStringTimp="";
      inputStringDirectie="";
      
    
    }
  }
}

int lookRight()
{
  srv.write(10);
  delay(500);
  int distance = readPing();
  delay(100);
  srv.write(90);
  return distance;
}

int lookLeft()
{
  srv.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  srv.write(90);
  return distance;
}

int readPing()
{
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0)
  {
    cm = 200;
  }
  return cm;
}

void moveStop()
{
  digitalWrite(mpin00,LOW);
  digitalWrite(mpin01,LOW);
  digitalWrite(mpin10,LOW);
  digitalWrite(mpin11,LOW);
}

void moveForward()
{
  if (!goesForward)
  {
    goesForward = true;

    digitalWrite(mpin00,HIGH);
    digitalWrite(mpin10,HIGH);

    digitalWrite(mpin01,LOW);
    digitalWrite(mpin11,LOW);
  }
}

void moveBackward()
{
  goesForward = false;

  digitalWrite(mpin01,HIGH);
  digitalWrite(mpin11,HIGH);

  digitalWrite(mpin00,LOW);
  digitalWrite(mpin10,LOW);
}

void turnRight()
{
  digitalWrite(mpin00,HIGH);
  digitalWrite(mpin11,HIGH);

  digitalWrite(mpin01,LOW);
  digitalWrite(mpin10,LOW);

  delay(500);

  digitalWrite(mpin00,HIGH);
  digitalWrite(mpin10,HIGH);

  digitalWrite(mpin01,LOW);
  digitalWrite(mpin11,LOW);
}

void turnLeft()
{
  digitalWrite(mpin01,HIGH);
  digitalWrite(mpin10,HIGH);

  digitalWrite(mpin00,LOW);
  digitalWrite(mpin11,LOW);

  delay(500);

  digitalWrite(mpin00,HIGH);
  digitalWrite(mpin10,HIGH);

  digitalWrite(mpin01,LOW);
  digitalWrite(mpin11,LOW);
}

void serialEvent1(){
    while(mySerial.available()){
      
      char inChar=(char)mySerial.read();
      if(inChar!='\n'){
        if(inChar==' ') //am citit timpul
          readSpace=true;
        else{
            if(readSpace==false)
            { //citim timpul
                inputStringTimp+=inChar;
            }else
                if(readSpace==true){
                    inputStringDirectie+=inChar; 
                }
        }
        
      }   
      if(inChar=='\n'){
            stringComplete=true;   
            readSpace=false;
      }
    }
}

   
