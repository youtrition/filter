#include <Event.h>
#include <Timer.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

Timer t;

const int led = 12;
const int waterSensor = 3;
const int dosagePump1 = 4;
const int dosagePump2 = 5;
const int dosagePump3 = 6;
const int dosagePump4 = 7;
const int maxSensorPin = A0;
const int minSensorPin = A1;

int senseMax = 0;
int sense2 = 0;

void setup(){
  Serial.begin(9600); //Initialize Serial for debugging

  //Initialize LCD Display
  lcd.begin(16,2);         // initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.backlight(); // finish with backlight on  
  lcd.setCursor(1,0); //Start at character 4 on line 0
  lcd.print("Urinstand: ");

  //Set pinModes
  pinMode(led, OUTPUT);
  pinMode(waterSensor, OUTPUT);
  pinMode(dosagePump1, OUTPUT);
  pinMode(dosagePump2, OUTPUT);
  pinMode(dosagePump3, OUTPUT);
  pinMode(dosagePump4, OUTPUT);
  pinMode(maxSensorPin, INPUT);
  pinMode(minSensorPin, INPUT);

  // Relays standardmässig ausschalten (HIGH = aus)
  digitalWrite(dosagePump1,HIGH);
  digitalWrite(dosagePump2,HIGH);
  digitalWrite(dosagePump3,HIGH);
  digitalWrite(dosagePump4,HIGH);   

  //Set Timers
  //t.oscillate(dosagePump, 100000, LOW);
  // t.every(30000, t.pulse(dosagePump2, 5000, HIGH)); 
}

void loop() {
  t.update(); //Update Timer
}

//Sense if maximal level is reached
boolean senseMaxLevel() {
  //Read Pin
  digitalWrite(waterSensor,HIGH); //Turn voltage on
  if (analogRead(maxSensorPin) < 100) {
    senseMax+=1;
  }else{
    senseMax-=1;
  }
  //Check Borders
  senseMax = min(senseMax , 100);
  senseMax = max(senseMax , 0);
  
  digitalWrite(waterSensor,LOW); //Turn voltage off
  //Return Boolean
  if (senseMax >= 100) {
    return true;
  } else {
    return false;
  }
}

//Sense if minimal level is reached
boolean senseMinLevel() {
  //Read Pin
  digitalWrite(waterSensor,HIGH); //Turn voltage on
  if (analogRead(minSensorPin) < 100) {
    sense2+=1;
  }else{
    sense2-=1;
  }
  //Check Borders
  sense2 = min(sense2 , 10000);
  sense2 = max(sense2 , 0);
  
  digitalWrite(waterSensor,LOW); //Turn voltage off
  //Return Boolean
  if (sense2 >= 10000) {
    return true;
  } else {
    return false;
  }
    digitalWrite(waterSensor,LOW);
}
