//================================
#include <SoftwareSerial.h>
const byte RX = D6;
const byte TX = D5;
SoftwareSerial mySerial = SoftwareSerial(RX,TX);

String inputString = "";
bool stringComplete = false;

String inputString1 = "";
bool stringComplete1 = false;

long last=0;
//============================================



#define ENA   5          // Điều khiển tốc độ động cơ bên trái     GPIO5(D1)
#define ENB   15         // Điều khiển tốc độ động cơ bên phải    GPIO13(D8)
#define IN1  4          // L298N in1 Động cơ trái quay             GPIO4(D2)
#define IN2  0          // L298N in2 Động cơ trái quay ngược lại   GPIO0(D3)
#define IN3  2           // L298N in3 Động cơ phải quay            GPIO2(D4)
#define IN4  13          // L298N in4 Động cơ phải quay ngược lại GPIO15(D7)
#include <ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

/* WIFI settings */
const char* ssid = "TPT";
const char* password = "thinh999";

/* data received from application */
String  data =""; 
int tocdoxe = 800;         // 400 - 1023.
/* define L298N or L293D motor control pins */

void setup()
{
  /* initialize motor control pins as output */
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  mySerial.begin(9600);
  while(!mySerial);
  connectWiFi();

  server.begin();
}

void loop()
{
    
    /* If the server available, run the "checkClient" function ===============================================================*/
    client = server.available();
    if (!client) return; 
    data = checkClient ();
  

    if (data == "ON") send_ON();
    
    else if (data == "OFF") send_OFF();
   
    else if (data == "tien") moveForward();
   
    else if (data == "lui") moveBackward();
   
    else if (data == "trai") turnLeft();
   
    else if (data == "phai") turnRight();

    else if(data=="dung") moveStop();
    //============================================================================================================================
} 
void send_ON()
{
  String data1 = "ON";
  mySerial.println(data1);
  Serial.println(data1);  
}
void send_OFF()
{
  String data2 = "OFF";
  mySerial.println(data2);
  Serial.println(data2);
}
/*
void sendUno()
{
  while(Serial.available())
  {
    char inChar1 = (char)Serial.read();
    inputString1 += inChar1;

    if(inChar1 == '\n')
    {
      stringComplete1 = true;
    }
    if (stringComplete1) 
    {
      Serial.print("Send UNO: ");
      Serial.println(inputString1);

      mySerial.print(inputString1);
      mySerial.flush();
    
      inputString1 = "";
      stringComplete1 = false;
    }
  }
}
*/
/*
void sendUno()
{
  while(Serial.available())
  {
    char inChar1 = (char)Serial.read();
    inputString1 += inChar1;

    if(inChar1 == '\n')
    {
      stringComplete1 = true;
    }
    if (stringComplete1) 
    {
      Serial.print("Send UNO: ");
      Serial.println(inputString1);

      mySerial.print(inputString1);
      mySerial.flush();
    
      inputString1 = "";
      stringComplete1 = false;
    }
  }
}
*/
void moveBackward()
{
  Serial.println("Move Backward");
  analogWrite(ENA,tocdoxe);
  analogWrite(ENB,tocdoxe);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
}
void moveForward()
{
  Serial.println("Move Forward");
  analogWrite(ENA,tocdoxe);
  analogWrite(ENB,tocdoxe);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
}

void turnRight()
{
  Serial.println("Turn Right");
  analogWrite(ENA,tocdoxe);
  analogWrite(ENB,tocdoxe);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft()
{
  Serial.println("Turn Left");
  analogWrite(ENA,tocdoxe);
  analogWrite(ENB,tocdoxe);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
}

void moveStop()
{
  Serial.println("Move Stop");
  analogWrite(ENA,LOW);
  analogWrite(ENB,LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW); 
}
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  request.remove(0, 5);
  Serial.println(request);
  request.remove(request.length()-9,9);
  Serial.println(request);
  return request;
}
void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}
