/*
 * Created by Lam Hong Duc
 * Date 17/11/2020
 */

int trig1 = A0;
int echo1 = A1;
int trig2 = A2;
int echo2 = A3;
int trig3 = A4;
int echo3 = A5;
int trig4 = 10;
int echo4 = 9;

int in1 = 4;
int in2 = 5;
int in3 = 6;
int in4 = 7;
int ENA = 3;
int ENB = 11;
int LeftSpeed = 210;
int RightSpeed = 80;
long leftDistance = 0,middleDistance = 0,rightDistance = 0,afterDistance = 0;
long pingTime,distance;
float speedSound = 0.0343;
int DIS = 25;
int 

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

long afterMeasurement()
{
  digitalWrite(trig4,LOW);
  delayMicroseconds(2);
  digitalWrite(trig4,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig4,LOW);
  pingTime = pulseIn(echo4,HIGH);
  distance = int(pingTime/2/29.412);
  return(distance); 
}

void setup()
{
  Serial.begin(115200);
  pinMode(trig1,OUTPUT);
  pinMode(trig2,OUTPUT);
  pinMode(trig3,OUTPUT);
  pinMode(trig4,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(echo2,INPUT);
  pinMode(echo3,INPUT);
  pinMode(echo4,INPUT);
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
  afterDistance = afterMeasurement();
  delay(10);
  Serial.print("leftDistance = ");
  Serial.print(leftDistance);
  Serial.print("cm /");
  Serial.print("middleDistance = ");
  Serial.print(middleDistance);
  Serial.print("cm /");
  Serial.print("rightDistance = ");
  Serial.print(rightDistance);
  Serial.println("cm");
  Serial.print("afterDistance = ");
  Serial.print(afterDistance);
  Serial.println("cm");

  if(Serial.available()>0)
    {
      String dulieu;
      dulieu=Serial.readStringUntil('\r');
      //doc bien bao giao thong
      if(dulieu=="Turn Right")
        { 
          moveStop();
          delay(300);
          turnRight();
          delay(400);
        }
      else if(dulieu=="Turn Left")
        {
          moveStop();
          delay(300);
          turnLeft();
          delay(400);  //  turnLeft 90 degree
        }
    }
    
  

  else 
    {
      //  re phai
      if(leftDistance < DIS && middleDistance > DIS && rightDistance > DIS)
      {
        moveStop();
        delay(300);
        turnRight();
        delay(25);  //  turn lightly right    
      }
      
      else if(leftDistance < DIS && middleDistance < DIS && rightDistance > DIS)
      { 
        moveStop();
        delay(300);
        turnRight();
        delay(400);  //  turnRight 90 degree
      }

      //re trai
      else if(leftDistance > DIS && middleDistance > DIS && rightDistance < DIS)
      {
        moveStop();
        delay(300);
        turnLeft();
        delay(25);  //turn lightly left  
      }
      else if(leftDistance > DIS && middleDistance < DIS && rightDistance < DIS)
      {
        moveStop();
        delay(300);
        turnLeft();
        delay(400);  //  turnLeft 90 degree
      }
      

      else if(leftDistance > DIS && middleDistance < DIS && rightDistance > DIS)
      {
        if(leftDistance > rightDistance)
        {
          moveStop();
          delay(300);
          turnLeft();
          delay(400);  //  turn 90 degree left
        }
        if(leftDistance < rightDistance)
        {
          moveStop();
          delay(300);
          turnRight();
          delay(400);  //  turn 90 degree right
        }
      }

      else if(leftDistance < DIS && middleDistance > DIS && rightDistance < DIS)
      {
        if(leftDistance > rightDistance)
        {
          moveStop();
          delay(300);
          turnLeft();
          delay(50);  //  shift left
        }
        if(leftDistance < rightDistance)
        {
          moveStop();
          delay(300);
          turnRight();
          delay(50);  //  shift right
        }
      }

      //  Obstacle on all 3 side
      else if(leftDistance < DIS && middleDistance < DIS && rightDistance < DIS)
      {
        moveStop();
        delay(1000);
        if(afterDistance < DIS)
          {
            turnLeft();
            delay(400);  //  turn 90 degree left
          }
        else
          {
            moveBackward();
            delay(1000);
            turnLeft();
            delay(800);  //turn 180 degrees
          }
        
      }
      else
      {
        moveForward();
      }
    }


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

void turnRight()
{
  Serial.println("Turn Right");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft()
{
  Serial.println("Turn Left");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
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
