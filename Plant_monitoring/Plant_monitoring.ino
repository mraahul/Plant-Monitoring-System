#include <dht.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <Servo.h>

#define DHT_PIN 6 // Analog Pin sensor is connected to DHT22

const int rs = 12, en =   11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myservo;
dht DHT;
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

unsigned long previousMillis = 0;
const long interval = 3000;
int pos = 0;    // variable to store the servo position
int servoState = 0; 

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {

  Serial.begin(115200);
  delay(500);//Delay to let system boot
 
  myservo.attach(10);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  lcd.begin(16, 2);     lcd.print("Plant Monitoring");
  lcd.setCursor(0, 1);  lcd.print("System");

  Serial.println("DHT22 Humidity & temperature Sensor\n\n");
  delay(2000);
  
  myservo.write(0);
  lcd.clear();
}//end "setup()"

void loop() {
  int reading = digitalRead(A0);
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH && servoState == 0) {
        myservo.write(180); servoState = 1;
      } else if (buttonState == HIGH && servoState == 1) {
        myservo.write(0); servoState = 0;
      }
    }
  }
  lastButtonState = reading;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    Serial.println("Reading data & RTC...");

    DHT.read22(DHT_PIN);

    lcd.setCursor(0, 0);
    lcd.print("Temp: "); lcd.print(DHT.temperature); lcd.print((char)223); lcd.print("C ");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: "); lcd.print(DHT.humidity); lcd.print("% ");

    readTime();
  }
}// end loop()

void readTime(void) {

  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.println();
  //    delay(3000);

}
