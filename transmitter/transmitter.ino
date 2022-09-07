#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
const char text[10];
const int sensor=A0; // Assigning analog pin A5 to variable 'sensor'
float tempc; //variable to store temperature in degree Celsius
float tempf; //variable to store temperature in Fahreinheit
float vout; //temporary variable to hold sensor reading
int fan=2;
int lamp=3;
void setup() {
 pinMode(sensor,INPUT); 
 pinMode(fan,OUTPUT);
 pinMode(lamp,OUTPUT);
 Serial.begin(9600);
 radio.begin();
 radio.openWritingPipe(address);
 radio.setPALevel(RF24_PA_MIN);
 radio.stopListening();
}
void loop() {
 vout=analogRead(sensor); //Reading the value from sensor
 vout=(vout*500)/1023;
 tempc=vout; // Storing value in Degree Celsius
 tempf=(vout*1.8)+32; // Converting to Fahrenheit
 Serial.print("In Degree C = ");
 Serial.print(tempc);
 Serial.print(" ");
 Serial.print(" In Fahrenheit = ");
 Serial.print(tempf);
 Serial.println();
 dtostrf(tempc,2,2,text);
 radio.write(&text, sizeof(text));
if(tempc<36.0){
 digitalWrite(lamp,HIGH);
 digitalWrite(fan,LOW);
 Serial.println("condition1");
 }
 if(tempc>=36.0){
 digitalWrite(lamp,LOW);
 digitalWrite(fan,HIGH);
 Serial.println("condition2");
 } 
 delay(1000);
}
