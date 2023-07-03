#include <EEPROM.h>
#include "GravityTDS.h"
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <Adafruit_SH1106.h>
#define ONE_WIRE_BUS 7
#define TdsSensorPin A1
#define OLED_RESET -1
Adafruit_SH1106 myDisplay(OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS); 
GravityTDS gravityTds;
DallasTemperature sensors(&oneWire);
//Adafruit_SSD1306 myDisplay(128, 64, &Wire);
float tdsValue = 0;
bool state=false;
float calibration_value = 22.44,ph_act;
int phval = 0,a,wt=0,count; 
unsigned long int avgval; 
int buffer_arr[10],temp,less,more;


void setup()
{
    Serial.begin(9600);
    pinMode(4,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(9,OUTPUT);
    digitalWrite(4,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    analogWrite(9,0);
    sensors.begin();
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);
    gravityTds.setAdcRange(1024);
    gravityTds.begin();
   // myDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    myDisplay.begin(SH1106_SWITCHCAPVCC, 0x3C);
    myDisplay.clearDisplay();
    myDisplay.setTextSize(2);
    myDisplay.setCursor(0,0);
    myDisplay.setTextColor(WHITE);
    myDisplay.println("   Hydro    phonics   System");
    myDisplay.display();
    //delay(2000);
}
 
void loop()
{
  count++;
//Serial.println(count); 
 // analogWrite(9,80);

 for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 ph_act =((-5.70 * volt + calibration_value));
 //ph_act =((-5.70 * volt + calibration_value)+11);
 //Serial.print("pH Val:");
 //Serial.println(ph_act);
 delay(500);
    digitalWrite(4,LOW);
    digitalWrite(3,LOW);
    sensors.requestTemperatures();
    gravityTds.setTemperature(sensors.getTempCByIndex(0));  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    a=sensors.getTempCByIndex(0);
    tdsValue = gravityTds.getTdsValue();  // then get the value
    //Serial.print(tdsValue,0);
    //Serial.println(less);
    //Serial.println("ppm");
    delay(200);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
   // Serial.print("Temperature is: "); 
    //Serial.println(a);
    myDisplay.clearDisplay();
    myDisplay.setTextSize(2);
    myDisplay.setCursor(0,0);
    myDisplay.setTextColor(WHITE);
    myDisplay.print("Ph :");
    myDisplay.println(ph_act);
    myDisplay.setCursor(0,25);
    myDisplay.print("TDS:");
    myDisplay.println(tdsValue);
    myDisplay.setCursor(0,46);
    myDisplay.print("Temp:");
    myDisplay.println(a);
    myDisplay.display();
    sendCombinedData(ph_act, tdsValue, a);
    wt++;
    if(ph_act<6)
    {
     less++;
    }

    if(ph_act>7)
    {
      more++;
    }

    if(less>=15&&wt>25)
    {
      digitalWrite(6,LOW);
      digitalWrite(5,HIGH);
      delay(500);
      digitalWrite(6,LOW);
      digitalWrite(5,LOW);
      less=0;
      wt=0;
    }

    if(more>=15&&wt>25)
    {
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      delay(500);
      digitalWrite(6,LOW);
      digitalWrite(5,LOW);
      more=0;
      wt=0;
    }

    if(ph_act>6&&ph_act<7)
    {
      less=0;
      more=0;
    }



//delay(500);
}

void sendCombinedData(float value1, float value2, int value3)
 {
 //String combinedData = String(value1) + "," + String(value2) + "," + String(value3);
 String combinedData = "," + String(value2) + "," + String(value3);
  Serial.print(value1);
  Serial.println(combinedData);
  
}
