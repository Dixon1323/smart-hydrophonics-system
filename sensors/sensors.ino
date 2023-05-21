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

float calibration_value = 22.44;
int phval = 0,a; 
unsigned long int avgval; 
int buffer_arr[10],temp;


void setup()
{
    Serial.begin(9600);
    pinMode(4,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    digitalWrite(4,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    sensors.begin();
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
    gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
    gravityTds.begin();  //initialization
   // myDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    myDisplay.begin(SH1106_SWITCHCAPVCC, 0x3C);
    myDisplay.clearDisplay();
    myDisplay.setTextSize(2);
    myDisplay.setCursor(0,0);
    myDisplay.setTextColor(WHITE);
    myDisplay.println("Hydrophonics System");
    myDisplay.display();
    //delay(2000);
}
 
void loop()
{
     
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
 float ph_act = -5.70 * volt + calibration_value;
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
    if(ph_act<5)
    {
      digitalwrite(5,LOW);
      delay(300);
      digitalWrite(5,HIGH)
    }

    if(ph_act>8)
    {
      digitalwrite(5,LOW);
      delay(300);
      digitalWrite(5,HIGH)
    }



//delay(500);
}

void sendCombinedData(float value1, float value2, int value3)
 {
  String combinedData = String(value1) + "," + String(value2) + "," + String(value3);
  Serial.println(combinedData);
}
