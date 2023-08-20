

//khai bao de ket noi esp8266
#include <SoftwareSerial.h>
const byte RX = 8;
const byte TX = 9;
SoftwareSerial mySerial = SoftwareSerial(RX,TX);
bool control = 0;

String inputString = "";      
bool stringComplete = false;


int trig1 = A0;
int echo1 = A1;
int trig2 = A2;
int echo2 = A3;
int trig3 = A4;
int echo3 = A5;
//int trig4 = 12;
//int echo4 = 10;

int in1 = 4;
int in2 = 5;
int in3 = 6; ///chân động cơ
int in4 = 7;
int ENA = 3;
int ENB = 11;
int LeftSpeed = 100;
int RightSpeed = 100;
long leftDistance = 0,middleDistance = 0,rightDistance = 0;
long pingTime,distance;
float speedSound = 0.0343;

int DIS = 10;
int DIS2 = 35;

long leftMeasurement()
{
  digitalWrite(trig1,LOW);
  delayMicroseconds(2);
  digitalWrite(trig1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1,LOW);
  pingTime = pulseIn(echo1,HIGH);
  distance = (pingTime/2)*speedSound;
  return(distance); 
}

long middleMeasurement()
{
  digitalWrite(trig2,LOW);
  delayMicroseconds(2);
  digitalWrite(trig2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2,LOW);
  pingTime = pulseIn(echo2,HIGH);
  distance = (pingTime/2)*speedSound;
  return(distance); 
}

long rightMeasurement()
{
  digitalWrite(trig3,LOW);
  delayMicroseconds(2);
  digitalWrite(trig3,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3,LOW);
  pingTime = pulseIn(echo3,HIGH);
  distance = (pingTime/2)*speedSound;
  return(distance); 
}

void setup()
{
  Serial.begin(9600);

  mySerial.begin(9600); //cong giao tiep esp8266

  pinMode(trig1,OUTPUT);
  pinMode(trig2,OUTPUT);
  pinMode(trig3,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(echo2,INPUT);
  pinMode(echo3,INPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  moveStop();
}

void loop()
{ 
  leftDistance = leftMeasurement();
  delay(10);
  middleDistance = middleMeasurement();
  delay(10);
  rightDistance = rightMeasurement();
  delay(10);
 
  Read_Uart();
  xuli1();
}

void Read_Uart()
{
  while (mySerial.available()) 
  {
    
    char inChar = (char)mySerial.read();
    
    inputString += inChar;
  
    if (inChar == '\n') 
    {
      stringComplete = true;
    }
    if (stringComplete) 
    {
      Serial.print("Data nhận: ");
      Serial.println(inputString);

      //xử lí dữ liệu
      if(inputString.indexOf("ON")>=0) control = 1;
      else if(inputString.indexOf("OFF")>=0) control = 0;

      inputString = "";
      stringComplete = false;
    }
  }
}
void xuli1()
{
  if(control == 0)
  {
    moveStop();
  }
  else if(control == 1)
  {
    xuli2();
  }
}
void xuli2()
{
  if(middleDistance<DIS2)
      {   
          moveStop();
          delay(2000);

          if(Serial.available())
          {
            String dulieu;
            dulieu=Serial.readStringUntil('\r');
            //doc bien bao giao thong
            if(dulieu=="Turn Right")
              { 
                turnRight();
                delay(600);
              }
            else if(dulieu=="Turn Left")
              {
                turnLeft();
                delay(600);
              }
          }

          else 
          {
            //  re phai
            if(leftDistance < DIS && rightDistance > DIS)
            {
              
              turnRight();
              delay(400);  // quay 90  
            }

            //re trai
            else if(leftDistance > DIS && rightDistance < DIS)
            {
              
              turnLeft();
              delay(400);  //quay 90
            }
            else if(leftDistance > DIS && rightDistance > DIS)
            {
              if(leftDistance < rightDistance)
                {
                  turnRight();
                  delay(400); 
                }
              else if(leftDistance > rightDistance)
                {
                  turnLeft();
                  delay(400);  
                }
            }
          
            else if(leftDistance < DIS && rightDistance < DIS)
            {
              moveStop();
              delay(1000);
              
                moveBackward();
                delay(500);
                if(leftDistance < rightDistance)
                  {
                    turnRight();
                    delay(800); 
                  }
                else if(leftDistance > rightDistance)
                  {
                    turnLeft();
                    delay(800);  //turn 180 degrees
                  }
              
              
            
            }
          }
      }
    else if(middleDistance>DIS2)
      {
        if(Serial.available())
          {
            String dulieu;
            dulieu=Serial.readStringUntil('\r');
            //doc bien bao giao thong
            if(dulieu=="Turn Right")
              { 
                turnRight();
                delay(400);
              }
            else if(dulieu=="Turn Left")
              {
                turnLeft();
                delay(400);
              }
          }
        else if(leftDistance < DIS && rightDistance > DIS)
            {
              
              turnRight();
              delay(25);  //  turn lightly right    
            }

            //re trai
            else if(leftDistance > DIS && rightDistance < DIS)
            {
              
              turnLeft();
              delay(25);  //turn lightly left  
            }
          
            else if(leftDistance < DIS && rightDistance < DIS)
            {
              moveStop();
              delay(500);
                if(leftDistance < rightDistance)
                {
                  turnRight();
                  delay(50); 
                }
                else if(leftDistance > rightDistance)
                {
                  turnLeft();
                  delay(50);  //turn 180 degrees
                }
            }
            else
            {
              moveForward();
            }
      }
}


void moveBackward()
{
  Serial.println("Move Backward");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
}
void moveForward()
{
  Serial.println("Move Forward");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
}

void turnRight()
{
  Serial.println("Turn Right");
  analogWrite(ENA,240);
  analogWrite(ENB,240);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft()
{
  Serial.println("Turn Left");
  analogWrite(ENA,240);
  analogWrite(ENB,240);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
}

void moveStop()
{
  Serial.println("Move Stop");
  analogWrite(ENA,LOW);
  analogWrite(ENB,LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
}
