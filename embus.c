//Source code EMBUS
#include "HX711.h"  
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#define DOUT  3
#define CLK  2
SoftwareSerial mySerial(9, 10); 
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
 
HX711 scale(DOUT, CLK);
 

void setup() {
  
  Serial.begin(9600); // Setting the baud rate of Serial Monitor (Arduino) 
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  scale.set_scale(-96650);  //Calibration Factor obtained from first sketch
  scale.tare();  //Reset the scale to 0 
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  delay(100); 
}
 

void loop() 
{
  int flag=0;
  Serial.print("Weight: ");
  Serial.print(scale.get_units(), 3);  //Up to 3 decimal points
  Serial.println(" kg"); //Change this to kg and re-adjust the calibration factor if you follow lbs
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  lcd.begin(16,2);
  lcd.setCursor(4,0);
  lcd.print("EMBUS");
  lcd.setCursor(0,1);
  lcd.print("Vehicle moving");
   if(scale.get_units()>2)
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("foot boarding");
    lcd.setCursor(0,1);
    lcd.print("detected");
    digitalWrite(4,HIGH);
    delay(1000);
    digitalWrite(4,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print("vehicle turns");
    lcd.setCursor(0,1);
    lcd.print("off in 10sec");
    delay(2000);
    lcd.clear();
    
    lcd.setCursor(0,0);
    int i;
    for(i=10;i>0;i--)
      {
        if(scale.get_units()<2)
        {
          digitalWrite(5,HIGH);
          digitalWrite(6,HIGH);
          break;
        }
        
     else
     {
      lcd.setCursor(6,0);
      lcd.print(i);
      delay(1000);
      lcd.clear();
     // if(i==5)
      //digitalWrite(4,LOW);
      }
     }
     if(i==0)
     {
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    lcd.setCursor(0,0);
    lcd.print("vehicle turned");
    lcd.setCursor(0,1);
    lcd.print("off");
    delay(1000);
    lcd.clear();
     SendMessage();
     lcd.setCursor(0,0);
    lcd.print("press button");
    lcd.setCursor(0,1);
    lcd.print("to restart");
    delay(1000000);
     }
   
  }
      
 
}
      void SendMessage()
      {
         mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
         delay(1000);  // Delay of 1000 milli seconds or 1 second
         mySerial.println("AT+CMGS=\"+918639086559\"\r"); // Replace x with mobile number
         delay(1000);
         mySerial.println("Foot boarding detected in  ROUTE-1. Action required!");// The SMS text you want to send
         delay(100);
         mySerial.println((char)26);// ASCII code of CTRL+Z
         delay(1000);
      }
  



