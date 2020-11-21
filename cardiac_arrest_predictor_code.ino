#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(6,7);
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

int HBSensor = 4;
int HBCount = 0;
int HBCheck = 0;
int TimeinSec = 0;
int HBperMin = 0;
int HBStart = 2;
int HBStartCheck = 0;
int tempPin = 1;

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);  
  lcd.begin(20, 4);
  pinMode(HBSensor, INPUT);
  pinMode(HBStart, INPUT_PULLUP);
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("everything is good!!");
  lcd.setCursor(0,1);
  lcd.print("Temperature : ");
  lcd.setCursor(0,2);
  lcd.print("HB per Min  : ");
}

void loop() {
int val = analogRead(tempPin);
val=(val/1024.0)*500;
lcd.setCursor(14,1);
lcd.print(val);
lcd.print("*C");
  if(digitalRead(HBStart) == LOW){lcd.setCursor(0,3);lcd.print("HB Counting ..");HBStartCheck = 1;}
  if(HBStartCheck == 1)
  {
      if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
      {
        HBCount = HBCount + 1;
        HBCheck = 1;
      }
      if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
      {
        HBCheck = 0;   
      }
      if(TimeinSec == 10)
      {
          HBperMin = HBCount * 6;
          HBStartCheck = 0;
          lcd.setCursor(14,2);
          lcd.print(HBperMin);
          lcd.print(" ");
          HBCount = 0;
          TimeinSec = 0;      
      }
      if(val>40 && HBperMin>100)
      {
        lcd.setCursor(0,0);
        lcd.print("ALERT! visit doctor!");
        SendMessage();
      }
  }
}

void timerIsr()
{
  if(HBStartCheck == 1)
  {
      TimeinSec = TimeinSec + 1;
  }
}

 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917397434200\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("RUSH TO HOSPITAL!");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
