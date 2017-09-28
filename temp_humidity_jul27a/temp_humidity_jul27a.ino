#include "DHT.h"
#include <LiquidCrystal.h>
#define DHTPIN 8   
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
;LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
Serial.begin(9600);
lcd.begin(16, 2);
dht.begin();
}
void loop() {
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(t) || isnan(h)) {
  Serial.println("Failed to read from DHT");
  }
else {
  lcd.setCursor(0,0);
  lcd.print("Temp=");
  lcd.print(t);
  lcd.print(" *C");
  lcd.setCursor(0,1);
  lcd.print("Humidity=");
  lcd.print(h);
  lcd.print("% ");
  delay(500);
 }
}
