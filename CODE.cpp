#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
SimpleTimer timer;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "34a7a951dfae45cbb7acfd40bc8ff175";
const char *ssid =  "moto";//WIFI Name
const char *password =  "12345678";//WIFI Passcode
WiFiClient client;
#define SELPIN D5 //Selection Pin
#define DATAOUT D6//MOSI
#define DATAIN  D7//MISO
#define SPICLOCK  D8//Clock
#define LED_BUILTIN D0
int readvalue1;
int readvalue2;
int readvalue3;
int readvalue4;
int readvalue5;
void repeatMe()
{
int y1,y3,y4,y5;
//TEMPERATURE 
lcd.setCursor(5,0);
readvalue1=read_a dc(1)*0.02;
int x1= (int)readvalue1;
y1=x1+25;
Blynk.virtualWrite(V0,y1);
lcd.print(y1);
if(y1<=34)
{

digitalWrite(LED_BUILTIN, LOW);
}
else
{
Blynk.notify("High Temperature");
Blynk.email("spavanraj97@gmail.com", "Temperature", "High Temperature");
digitalWrite(LED_BUILTIN, HIGH);
}
Serial.println("Temperature value is ");
Serial.println(y1);
delay(500);
//SMOKE
lcd.setCursor(14,0);
readvalue2 = read_adc(2)*0.02;
int x2= (int)readvalue2;
lcd.print(x2);
Blynk.virtualWrite(V1,x2);
if(x2>=50)
{
Blynk.notify("Smoke Detected");
Blynk.email("spavanraj97@gmail.com", "Smoke", "Smoke Detected");
digitalWrite(LED_BUILTIN, HIGH);
}
else
{
digitalWrite(LED_BUILTIN, LOW);
}
Serial.println("Smoke value is ");
Serial.println(x2);
delay(500);
//FLAME
lcd.setCursor(5,1);
readvalue3 = read_adc(3)*0.02;
int x3= (int)readvalue3;
y3=x3+18;
Blynk.virtualWrite(V2,y3);
lcd.print(y3);
if(y3<87 || y3<90)
{
Blynk.notify("Fire Detected");
Blynk.email("spavanraj97@gmail.com", "Fire", "Fire Detected");
digitalWrite(LED_BUILTIN, HIGH);
}
else
{
digitalWrite(LED_BUILTIN, LOW);
}
Serial.println("Fire value is ");
Serial.println(y3);
delay(500);
delay(500);
//LIGHT
lcd.setCursor(14,1);
readvalue4 = read_adc(4)*0.02;
int x4= (int)readvalue4;
y4=x4+20;
Blynk.virtualWrite(V3,y4);
lcd.print(y4);
if(y4<=22)
{
Blynk.notify("No Light");
Blynk.email("spavanraj97@gmail.com", "Light", "No Light");
digitalWrite(LED_BUILTIN, HIGH);
}
else
digitalWrite(LED_BUILTIN, LOW);
Serial.println("Light value is ");
Serial.println(y4);
delay(500);
//IR
readvalue5 = read_adc(5)*0.02;
int x5= (int)readvalue5;
y5=x5+18;
Blynk.virtualWrite(V4,y5);
lcd.print(y5);
if(y5<=90)
{
Blynk.notify("Motion Detected");
Blynk.email("spavanraj97@gmail.com", "IR", "Motion Detected");
digitalWrite(LED_BUILTIN, HIGH);
}
else
{
digitalWrite(LED_BUILTIN, LOW);
}
Serial.println("Motion value is ");
Serial.println(y5);
delay(500);
}
//Initialize all variable for ADC and LCD before starting the code
void setup()
{
//ADC
Serial.begin(9600);
Blynk.begin(auth, ssid, password);
WiFi.begin(ssid, password);
pinMode(SELPIN, OUTPUT);
pinMode(DATAOUT, OUTPUT);
pinMode(DATAIN, INPUT);
pinMode(SPICLOCK, OUTPUT);
digitalWrite(SELPIN,HIGH);
digitalWrite(DATAOUT,LOW);
digitalWrite(SPICLOCK,LOW);
Serial.begin(9600);
Serial.begin(9600);
timer.setInterval(1000, repeatMe);
pinMode(LED_BUILTIN, OUTPUT);
pinMode(D5, OUTPUT);
//LCD
lcd.init();
lcd.clear();
lcd.backlight();
lcd.setCursor(3, 0);
lcd.print("INDUSTRIAL");
lcd.setCursor(0, 1);
lcd.print("MONITERINGSYSTEM");
delay(3000);
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("TEMP:");
lcd.setCursor(8,0);
lcd.print("SMOKE:");
lcd.setCursor(0,1);
lcd.print("FIRE:");
lcd.setCursor(8,1);
lcd.print("LIGHT:");
lcd.setCursor(7,0);
lcd.print("|");
lcd.setCursor(7,1);
lcd.print("|");
}
//Loop to run continuous
void loop()
{
timer.run();
Blynk.run();
}
int read_adc(int channel)
{
int adcvalue = 0;
byte commandbits = B11000000;
commandbits|=((channel-1)<<3);
digitalWrite(SELPIN,LOW);
for (int i=7; i>=3; i--)
{
digitalWrite(DATAOUT,commandbits&1<<i);
digitalWrite(SPICLOCK,HIGH);
digitalWrite(SPICLOCK,LOW);
}
digitalWrite(SPICLOCK,HIGH);
digitalWrite(SPICLOCK,LOW);
digitalWrite(SPICLOCK,HIGH);
digitalWrite(SPICLOCK,LOW);
for (int i=11; i>=0; i--)
{
adcvalue+=digitalRead(DATAIN)<<i;
digitalWrite(SPICLOCK,HIGH);
digitalWrite(SPICLOCK,LOW);
}
digitalWrite(SELPIN, HIGH);
return adcvalue;
}
