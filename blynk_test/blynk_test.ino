#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "Your Blynk template ID"
#define BLYNK_TEMPLATE_NAME "Your Blynk template namw"
#define BLYNK_AUTH_TOKEN "Your Blynk auth token"
#define BLYNK_PRINT Serial

SoftwareSerial swSerial(D3, D4);

char ssid[] = "Your SSID";
char pass[] = "Your Password";

void setup()
{
  Serial.begin(115200);
  swSerial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  if (swSerial.available()) {
    String receivedData = swSerial.readStringUntil('\n');
    receivedData.trim();
    parseData(receivedData);
  }
}

void parseData(String receivedData) {
  int commaIndex1 = receivedData.indexOf(',');
  int commaIndex2 = receivedData.indexOf(',', commaIndex1 + 1);
  
  String float1Str = receivedData.substring(0, commaIndex1);
  String float2Str = receivedData.substring(commaIndex1 + 1, commaIndex2);
  String intStr = receivedData.substring(commaIndex2 + 1);
  
  float ph = float1Str.toFloat();
  float tds = float2Str.toFloat();
  int temp = intStr.toInt();

  Serial.print("Received data:\n");
  Serial.print("PH: ");
  Serial.println(ph);
  Serial.print("TDS: ");
  Serial.println(tds);
  Serial.print("temperature: ");
  Serial.println(temp);
  Serial.println();
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, ph);
  Blynk.virtualWrite(V2, tds);
  Blynk.run();
}

