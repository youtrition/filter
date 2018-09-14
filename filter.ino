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
int counter = 0;

boolean Phase0 = false;
boolean Phase1 = false;
boolean Phase2 = false;
boolean Phase3 = false;

long phase0Timer = 10000; //Timer Phase 0: 2min 40seconds - 160000
long phase1Timer = 10000; //Timer Phase 1: 1,5 hours - 5400000
long phase2Timer = 10000; //Timer Phase 2: 2 minutes???? - 120000
long phase3Timer = 10000; //Timer Phase 3: same time?? 2 minutes?? - 120000

void setup(){
  Serial.begin(9600); //Initialize Serial for debugging

  //Initialize LCD Display
  lcd.begin(20,4);  // initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.backlight(); // finish with backlight on  
  lcd.clear();
    
  //Set pinModes
  pinMode(led, OUTPUT);
  pinMode(waterSensor, OUTPUT);
  pinMode(dosagePump1, OUTPUT);
  pinMode(dosagePump2, OUTPUT);
  pinMode(dosagePump3, OUTPUT);
  pinMode(dosagePump4, OUTPUT);
  pinMode(UVSterilizer, OUTPUT);  
  pinMode(maxSensorPin, INPUT);
  pinMode(minSensorPin, INPUT);

  // Relays standardmässig ausschalten (HIGH = aus)
  digitalWrite(dosagePump1,HIGH);
  digitalWrite(dosagePump2,HIGH);
  digitalWrite(dosagePump3,HIGH);
  digitalWrite(dosagePump4,HIGH);  
  digitalWrite(UVSterilizer,HIGH);  

  t.every(1000, count);
}

void count() {
  counter +=1;
  lcd.setCursor(0,3);
  lcd.print("                    "); //refresh 3. row of display every second to show timer properly
}

/*
void statusBar(long timer) {
  String bars;
  for(int i=1; i<=int((counter / timer*100))%5;i++){
    bars += "-";
  }
  lcd.setCursor(0,2);
  lcd.print((counter /(timer/1000))*5);
  Serial.println((counter /(timer/1000))*5);
}
*/

String intToString(long timer) {
   return String(" Noch ") + (((timer / 1000 - counter)/60)%60) +  String(" min ") + ((timer / 1000 - counter)%60) + String(" sek");
}
  
void phase0() {
  digitalWrite(dosagePump1,LOW);
  digitalWrite(dosagePump3,LOW);
  resetLCD();
  lcd.setCursor(0,0);
  lcd.print("  Urin wird in AK-");
  lcd.setCursor(0,1);
  lcd.print("  Filter gepumpt.");
//  lcd.setCursor(0,2);
//  lcd.print(" ------------------ ");  
  t.after(phase0Timer, phase0Off); 
}

void phase0Off() {
  digitalWrite(dosagePump1,HIGH);
  digitalWrite(dosagePump3,HIGH);
  resetLCD();
  counter = 0; //counter reset
  phase = 1;
  Phase0 = false;
}

void phase1() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Urinaufbereitung im"); 
  lcd.setCursor(0,1);
  lcd.print("  Aktivkohlefilter");
//  lcd.setCursor(0,2);
//  lcd.print(" ------------------ ");     
  t.after(phase1Timer, phase1Off);
}

void phase1Off() {
  counter = 0;
  phase = 2;
  Phase1 = false;
}

void phase2() {
  digitalWrite(dosagePump4,LOW);
  digitalWrite(UVSterilizer,LOW);
  resetLCD();
  lcd.setCursor(0,0);
  lcd.print("  Urin wird in UV-");
  lcd.setCursor(0,1);
  lcd.print(" Sterilizer gepumpt");
//  lcd.setCursor(0,2);
//  lcd.print(" ------------------ ");     
  t.after(phase2Timer, phase2Off);
}

void phase2Off() {
  counter = 0;
  digitalWrite(dosagePump4,HIGH);
  resetLCD();
  phase = 3;
  Phase2 = false;
}

void phase3() {
  counter = 0; //counter reset
  digitalWrite(dosagePump2,LOW);
  resetLCD();
  lcd.setCursor(0,0);
  lcd.print("Fluessiges Gold wird");
  lcd.setCursor(0,1);
  lcd.print("     abgepumpt");
//  lcd.setCursor(0,2);
//  lcd.print(" ------------------ ");    
  t.after(phase3Timer, phase3Off);
}

void phase3Off() {
  digitalWrite(dosagePump2,HIGH);
  digitalWrite(dosagePump4,HIGH);
  digitalWrite(UVSterilizer,HIGH);
  resetLCD();
  counter = 0;
  phase = 0;
  Phase3 = false;
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

void loop() {
  t.update(); //Update Timer

  switch(phase) {
    case 0: //Fill up charcoal filters
    lcd.setCursor(0,3);
    lcd.print(intToString(phase0Timer));
//    statusBar(phase0Timer);
    if (Phase0 == false) { phase0(); Phase0 = true; }
    break;
    case 1: //Filtration for 1.5 hours
    lcd.setCursor(0,3);    
    lcd.print(intToString(phase1Timer));
//    statusBar(phase1Timer);
    if (Phase1 == false) { phase1(); Phase1 = true; }
    break;
    case 2: //Empty Filters
    lcd.setCursor(0,3);    
    lcd.print(intToString(phase2Timer));
//    statusBar(phase2Timer);    
    if (Phase2 == false) { phase2(); Phase2 = true; }
    break;
    case 3: //Empty Sterilizer
    lcd.setCursor(0,3);    
    lcd.print(intToString(phase3Timer));
//    statusBar(phase3Timer);    
    if (Phase3 == false) { phase3(); Phase3 = true; }
    break; 
  }
}
void resetLCD() {
  lcd.begin(20,4);  // initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.clear();
}
