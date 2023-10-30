#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BOTtoken "your bot Token"
#define CHAT_ID "Your chat ID"
#define BLYNK_TEMPLATE_ID "Your Blynk Template ID"
#define BLYNK_TEMPLATE_NAME "Your Blynk template Namw"
#define BLYNK_AUTH_TOKEN "Your Blynk Auth token "
#define BLYNK_PRINT Serial

SoftwareSerial swSerial(D3, D4);
const char* ssid = "Your SSID";// Enter your WIFI SSID
const char* pass = "Your Password"; // Enter your WIFI Password

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup()
{
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");      
  client.setTrustAnchors(&cert);
  swSerial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println("");
  Serial.println("WiFi connected");

  bot.sendMessage(CHAT_ID, "Wifi Connected!", "");
  bot.sendMessage(CHAT_ID, "System has Started!!", "");

}

  
 void loop() 
{
  if (swSerial.available())
   {
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
  if(ph<4) //minimum ph value for sending message
  {
    bot.sendMessage(CHAT_ID, "Low Ph Value detected", "");
  }
  if(ph<8) //maximum ph value for sending message
  {
    bot.sendMessage(CHAT_ID, "High Ph Value detected", "");
  }

  
  if(tds>300) //maximum tds value for sending message
  {
    bot.sendMessage(CHAT_ID, "High TDS Value detected!!Change water", "");
  }
} 
