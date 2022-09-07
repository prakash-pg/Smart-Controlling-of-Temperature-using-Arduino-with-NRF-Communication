#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>
const int RS = D0, EN = D1, d4 = D8, d5 = D3, d6 = 3, d7 = 1; 
LiquidCrystal lcd(RS, EN, d4, d5, d6, d7);
int msg[1];
RF24 radio(2, 4); 
const byte address[6] = "00001";
const char* ssid = "******"; //ssid of your wifi
const char* password = "******"; //password of your wifi
const char* host = "******"; //replace it with your webhost url
void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
 radio.begin();
 radio.openReadingPipe(0, address);
 radio.setPALevel(RF24_PA_MIN);
 radio.startListening();
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 lcd.clear();
 lcd.print("WiFi connecting");
 }
 lcd.clear();
 lcd.print("WiFi connected");
 delay(2000);
}
void loop() {
 lcd.clear();
 if (radio.available()) {
 char text[10] = "";
 radio.read(&text, sizeof(text));
 Serial.print("Temperature = ");
 Serial.println(text);
 Serial.println(" C..");
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Temperature in C");
 lcd.setCursor(0, 1);
 lcd.print(text);
 Serial.print("Connecting to ");
 Serial.println(host);
 WiFiClient client;
 const int httpPort = 80;
 if(!client.connect(host, httpPort)){
 Serial.println("Connection Failed");
 return;
 }
 String url = "/insert.php?temperature_c=" + String(text);
 Serial.println("Requesting URL: ");
 Serial.println(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
 "Host: " + host + "\r\n" +
 "Connection: close\r\n\r\n");
 delay(500);
 while(client.available()){
 String line = client.readStringUntil('\r');
 Serial.print(line);
 }
 Serial.println();
 Serial.println("closing connection");
 delay(2000);
}
