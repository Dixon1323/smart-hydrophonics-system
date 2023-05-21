#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BOTtoken "6110455886:AAGWHbrnggxJ7RYG5MYI232aNr8WjkFzdrc"
#define CHAT_ID "687558002"
#define BLYNK_TEMPLATE_ID "TMPL3Tm7K0WgV"
#define BLYNK_TEMPLATE_NAME "hydrophonics"
#define BLYNK_AUTH_TOKEN "aHjqZTiIaxA_Y0iY6ZhJmMMat7qH2l9d"
#define BLYNK_PRINT Serial

SoftwareSerial swSerial(D3, D4);
const char* ssid = "Chathamkottu";// Enter your WIFI SSID
const char* pass = "12345678"; // Enter your WIFI Password

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
  if(ph<6.5)
  {
    bot.sendMessage(CHAT_ID, "Low Ph Value detected", "");
  }
  
  if(tds>300)
  {
    bot.sendMessage(CHAT_ID, "High TDS Value detected!!Change", "");
  }
} 
