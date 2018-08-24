#include <Event.h>
#include <Timer.h>
#include <LiquidCrystal_I2C.h>

//Create LCD Display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//Create Timer
Timer t;

//Set Pins
const int led = 12;
const int waterSensor = 3;
const int dosagePump1 = 4;
const int dosagePump2 = 5;
const int dosagePump3 = 6;
const int dosagePump4 = 7;
const int UVSterilizer = 8;
const int maxSensorPin = A0;
const int minSensorPin = A1;

//Declare Variables
int senseMax = 0;
int senseMin = 0;
int phase = 0;

boolean phase0 = false;
boolean phase1 = false;
boolean phase2 = false;
boolean phase3 = false;

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
}

void loop() {
  t.update(); //Update Timer
  Serial.println(phase)
  switch(phase) {
    case 0: //Fill up charcoal filters
    if phase0 == false { phase0(); phase0 = true; }
    break;
    case 1: //Filtration for 1.5 hours
    if phase1 == false { phase1(); phase1 = true; }
    break;
    case 2: //Empty Filters
    if phase2 == false { phase2(); phase0 = true; }
    break;
    case 3: //Empty Sterilizer
    if phase3 == false { phase3(); phase3 = true; }
    break; 
  }
}

void phase0() {
  digitalWrite(dosagePump1,LOW};
  digitalWrite(dosagePump3,LOW};
  t.after(5000, phase0Off()); 
}

void phase0Off() {
  digitalWrite(dosagePump1,HIGH};
  digitalWrite(dosagePump3,HIGH};
  phase = 1;
  phase0 = false;
}

void phase1() {
  t.after(5000, phase1Off())
}

void phase1Off() {
  phase = 2;
  phase1 = false;
}

void phase2() {
  digitalWrite(dosagePump4,LOW};
  digitalWrite(UVSterilizer,LOW);
  t.after(5000, phase1Off());
}

void phase2Off() {
  digitalWrite(dosagePump4,HIGH};
  phase = 3;
  phase2 = false;
}

void phase3() {
  digitalWrite(dosagePump2,LOW};
  t.after(5000, phase3Off());
}

void phase3Off() {
  digitalWrite(dosagePump2,HIGH};
  digitalWrite(dosagePump4,HIGH};
  digitalWrite(UVSterilizer,HIGH);
  phase = 0;
  phase3 = false;
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
    senseMin+=1;
  }else{
    senseMin-=1;
  }
  //Check Borders
  senseMin = min(senseMin , 10000);
  senseMin = max(senseMin , 0);
  
  digitalWrite(waterSensor,LOW); //Turn voltage off
  //Return Boolean
  if (senseMin >= 10000) {
    return true;
  } else {
    return false;
  }
    digitalWrite(waterSensor,LOW);
}
