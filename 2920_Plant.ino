// Author: My-Hanh Nguyen 
// Arduino Mega 2560

#include <LiquidCrystal.h>
#include <DS3231.h>
#include <IRTemp.h>
#include <SPI.h>
#include <SD.h>

// --- SD Card --- //
// *** MOSI - pin 51
// *** MISO - pin 50
// *** SCK  - pin 52
// *** CS   - pin 53 
File myFile;

// --- Moisture Sensor --- //
int moisturePin = A0;

// --- Water Pump Motor --- //
int waterPump = 3;

// --- LCD --- //
LiquidCrystal lcd(8,9,4,5,6,7);

// --- Ultrasonic Sensor --- //
int echoPin = 22;
int trigPin = 23;

// --- IR Temperature Sensor --- //
static const byte dataPin = 24;
static const byte clockPin = 25;
static const byte acquirePin = 26;
static const TempUnit SCALE = FAHRENHEIT;
IRTemp irTemp(acquirePin, clockPin, dataPin);

// --- Speaker --- //
int speakerPin = 27;

// --- Button --- //
int buttonPin = 28;

// --- LED --- //
int redLED = 29;
int blueLED = 30;
int greenLED = 31;
int yellowLED = 32;

// --- RTC --- //
DS3231  rtc(SDA, SCL);
Time  t;

// --- Variable Declaration --- //
int waterLevel = 0;
int moistureLevel = 0;
int temp = 0;

//---------------------------------------------------------------------------------//

void setup() {
  //Serial.begin(9600);
  lcd.begin(20,4);
  rtc.begin();

  pinMode(buttonPin, INPUT);
  pinMode(moisturePin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(waterPump, OUTPUT);

  digitalWrite(trigPin, LOW);
  digitalWrite(waterPump, LOW);

  lcd.setCursor(0,1);
  lcd.print("......Starting......");
  delay(3000);
  lcd.clear();

  t = rtc.getTime();
  //  rtc.setTime(1, 23, 0);      // Uncomment on first upload
  //  rtc.setDate(29, 3, 2018);   // Then comment out on second upload
} // setup

//---------------------------------------------------------------------------------//

void loop() {
  
  moistureLevel = getMoistureLevel();
  waterLevel = getWaterLevel();
  temp = getTemperature();

  updateLCD();

  // Write to SD card every hour
  if(t.min == 0) {                                   
    writeSDCard();
  } // if
  
  // Water tank < 25% => Alert user
  if(waterLevel < 25) {
    playSound();
    digitalWrite(redLED, HIGH);
    delay(300);
    digitalWrite(redLED, LOW);
  } // if

  if(moistureLevel > 70) {                                // Wet
    digitalWrite(blueLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
  } // if
  else if(moistureLevel <= 70 && moistureLevel > 30) {    // Normal
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
  } // else if
  else if(moistureLevel <= 30 && moistureLevel > 10){     // Dry
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH); 
    pumpWater();  
  } // else if
  else {                                                  // Not Inserted
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);     
  } //else
  
  // Turns on motor when button is pressed
  if(digitalRead(buttonPin) == 1) {
    digitalWrite(waterPump, HIGH);
  } // if
  else {
    digitalWrite(waterPump, LOW);
  } // else

} // loop

//---------------------------------------------------------------------------------//

void pumpWater() {
  digitalWrite(waterPump, HIGH);
  delay(3000);
  digitalWrite(waterPump, LOW);
} // pumpWater

//---------------------------------------------------------------------------------//

void writeSDCard() {
  
  //Serial.print("Initializing SD card...");
  
  if (!SD.begin(53)) {
    //Serial.print("Initialization failed!");
    return;
  } // if
  else {
    myFile = SD.open("BlockChainData.txt", FILE_WRITE);
    if(myFile) {
      //Serial.println("Writing to BlockChainData.txt...");
    
      myFile.print("Date: ");
      myFile.print(t.mon);
      myFile.print("/");
      myFile.print(t.date);
      myFile.print("/");
      myFile.println(t.year%2000);

      myFile.print("Time: ");
      myFile.print(t.hour);
      myFile.print(":");
      myFile.println(t.min);
      
      myFile.print("Moisture Level: ");
      myFile.print(moistureLevel);
      myFile.println(" %");
      
      myFile.print("Water Level: ");
      myFile.print(waterLevel);
      myFile.println(" %");

      myFile.print("Temperature: ");
      myFile.print(temp);
      myFile.println(SCALE == FAHRENHEIT ? " F" : " C");
      
      myFile.println("---");
      myFile.close();
      //Serial.println("done.");
    } // if
    else {
      //Serial.println("error opening BlockChainData.txt");
    } // else
  } // else
} // writeSDCard

//---------------------------------------------------------------------------------//

void updateLCD() {

  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(t.mon);
  lcd.print("/");
  lcd.print(t.date);
  lcd.print("/");
  lcd.print(t.year%2000);
  lcd.print("       ");
  lcd.print(t.hour);
  lcd.print(":");
  lcd.print(t.min);
  
  lcd.setCursor(0,1);
  lcd.print("Moisture Lvl:  ");
  lcd.print(moistureLevel);
  lcd.print(" %");
  
  lcd.setCursor(0,2);
  lcd.print("Water Lvl:     ");
  lcd.print(waterLevel);
  lcd.print(" %");
  
  lcd.setCursor(0,3);
  lcd.print("Temperature:   ");
  lcd.print(temp);
  lcd.print(SCALE == FAHRENHEIT ? " F" : " C");

} // updateLCD

//---------------------------------------------------------------------------------//

int getWaterLevel() {
  
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distanceCM = duration*0.034/2;
  int waterLevel = 100*(16 - distanceCM)/(16 - 3);
  return waterLevel; 
  
} // getWaterLevel

//---------------------------------------------------------------------------------//

int getMoistureLevel() {
  
  int moistureRaw = analogRead(moisturePin);
  moistureRaw = constrain(moistureRaw, 435, 988);
  int moistureLevel = map(moistureRaw, 435, 988, 100, 0);
  return moistureLevel;
  
} // getMoistureLevel

//---------------------------------------------------------------------------------//

float getTemperature() { 
  //float temp = irTemp.getAmbientTemperature(SCALE);
  float temp = irTemp.getIRTemperature(SCALE);
  return temp; 
} // getTemperature

//---------------------------------------------------------------------------------//

void playSound() {
  tone(speakerPin, 523, 1000/4);
} // playSound

//---------------------------------------------------------------------------------//








