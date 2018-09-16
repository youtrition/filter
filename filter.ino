#include <Event.h>
#include <Timer.h>
#include <LiquidCrystal_I2C.h>

#define displayLength 20.0

//Create LCD Display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//Create Timer
Timer t;

// StatusBar Characters
byte p1[8] = { 0x10,  0x10,  0x10,  0x10,  0x10,  0x10,  0x10,  0x10};
byte p2[8] = { 0x18,  0x18,  0x18,  0x18,  0x18,  0x18,  0x18,  0x18};
byte p3[8] = { 0x1C,  0x1C,  0x1C,  0x1C,  0x1C,  0x1C,  0x1C,  0x1C};
byte p4[8] = { 0x1E,  0x1E,  0x1E,  0x1E,  0x1E,  0x1E,  0x1E,  0x1E};
byte p5[8] = { 0x1F,  0x1F,  0x1F,  0x1F,  0x1F,  0x1F,  0x1F,  0x1F};

//Set Pins
const int led = 12;
const int waterSensor = 3;
const int dosagePump1 = 4;
const int dosagePump2 = 5;
const int dosagePump3 = 7;
const int dosagePump4 = 6;
const int UVSterilizer = 8;
const int urineLevelCylinder1 = 9;
const int urineLevelCylinder2 = 10;
const int urineLevelStorageUnfilteredMin = 11;
const int urineLevelStorageUnfilteredMax = 12;
const int urineLevelFlipFlop = 13;
const int sensorPin = A0;

//Declare Variables
int senseMax = 0;
int senseMin = 0;
int phase = 0;
double counter = 0;
double percent = 0.0;
unsigned char b;
unsigned int lines;
double test;

boolean Phase0 = false;
boolean Phase1 = false;
boolean Phase2 = false;
boolean Phase3 = false;

long phase0Timer = 1000; //Timer Phase 0: 2min 40seconds - 160000
long phase1Timer = 60000; //Timer Phase 1: 1,5 hours - 5400000
long phase2Timer = 60000; //Timer Phase 2: 2 minutes???? - 120000
long phase3Timer = 60000; //Timer Phase 3: same time?? 2 minutes?? - 120000

void setup(){
  Serial.begin(9600); //Initialize Serial for debugging

  //Initialize LCD Display
  lcd.begin(20,4);  // initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.backlight(); // finish with backlight on  
  lcd.clear();

  lcd.createChar(0, p1);
  lcd.createChar(1, p2);
  lcd.createChar(2, p3);
  lcd.createChar(3, p4);
  lcd.createChar(4, p5);
   
  //Set pinModes
  pinMode(led, OUTPUT);
  pinMode(waterSensor, OUTPUT);
  pinMode(dosagePump1, OUTPUT);
  pinMode(dosagePump2, OUTPUT);
  pinMode(dosagePump3, OUTPUT);
  pinMode(dosagePump4, OUTPUT);
  pinMode(UVSterilizer, OUTPUT);
  pinMode(urineLevelCylinder1, OUTPUT);
  pinMode(urineLevelCylinder2, OUTPUT);
  pinMode(urineLevelStorageUnfilteredMin, OUTPUT);
  pinMode(urineLevelStorageUnfilteredMax, OUTPUT); 
  pinMode(urineLevelFlipFlop, OUTPUT);
//  pinMode(maxSensorPin, INPUT);
//  pinMode(minSensorPin, INPUT);
  pinMode(sensorPin, INPUT);  

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


void setPolarity(boolean flip1,boolean flip2,boolean flip3,boolean flip4,boolean flip5) {
  if(flip1==false){digitalWrite(urineLevelFlipFlop, LOW);}else{digitalWrite(urineLevelFlipFlop, HIGH);}; 
  if(flip2==false){digitalWrite(urineLevelCylinder1, LOW);}else{digitalWrite(urineLevelCylinder1, HIGH);}; 
  if(flip3==false){digitalWrite(urineLevelCylinder2, LOW);}else{digitalWrite(urineLevelCylinder2, HIGH);}; 
  if(flip4==false){digitalWrite(urineLevelStorageUnfilteredMin, LOW);}else{digitalWrite(urineLevelStorageUnfilteredMin, HIGH);}; 
  if(flip5==false){digitalWrite(urineLevelStorageUnfilteredMax, LOW);}else{digitalWrite(urineLevelStorageUnfilteredMax, HIGH);}; 
}

boolean checkWater(int sensorNumber = 1) {
  int val =0;
  switch(sensorNumber) {
    case 1:
      setPolarity(0,1,0,0,0);
      val = analogRead(sensorPin);
      setPolarity(1,0,1,1,1);
      val += 600 - analogRead(sensorPin);
      setPolarity(1,1,1,1,1);
      /*
      Serial.println(val);
      lcd.setCursor(1,1);
      lcd.print(val);
      */
      if(val>=5) return true;
      return false;
      break;
    case 2:
      setPolarity(0,0,1,0,0);
      val = analogRead(sensorPin);
      setPolarity(1,1,0,1,1);
      val += 600 - analogRead(sensorPin);
      setPolarity(1,1,1,1,1);
      if(val>=5) return true;
      return false;
      break;
    case 3:
      setPolarity(0,0,0,1,0);
      val = analogRead(sensorPin);
      setPolarity(1,0,0,0,1);
      val += 600 - analogRead(sensorPin);
      if(val>=10) return true;
      return false;
      break;
    case 4:
      setPolarity(0,0,0,0,1);
      val = analogRead(sensorPin);
      setPolarity(1,1,1,1,0);
      val += 600 - analogRead(sensorPin);
      if(val>=10) return true;
      return false;
      break;
 
  }
}

void statusBar(double percent, int lineNr = 2) {
    lcd.setCursor(0,lineNr);
    double a=displayLength/100*percent; //1% = 1 Character auf Display
    if (a>=1) {
      for (int i=1;i<a;i++) {
        lcd.write(4); //Byte direkt auf Zeile schreiben (Char 4)
        b = i; //b = Zeilennummer
    }
    a = a-b;  // Immer wieder von 0 auf 1 raufzählen durch a-b
  }
  lines = a*5; // 0,2 x 5 = case 1; 0,4 x 5 = case 2; etc.
  switch (lines) {
    case 0:
      break;
    case 1:
      lcd.print((char)0);
      break;
    case 2:
      lcd.print((char)1);
      break;
    case 3:
      lcd.print((char)2);
      break;
    case 4:
      lcd.print((char)3);
      break;    
    case 5: 
      lcd.print((char)4);
      break;
  }
}


String intToString(long timer) {
   return String(" Noch ") + (((timer / 1000 - int(counter))/60)%60) +  String(" min ") + ((timer / 1000 - int(counter))%60) + String(" sek");
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
  boolean one;
  boolean two;
  if(checkWater(1)==true) {
    digitalWrite(dosagePump1,HIGH);
    one=true;
  }else {
    digitalWrite(dosagePump1,LOW);
    one=false;
  }
  if(checkWater(2)==true) {
    digitalWrite(dosagePump3,HIGH);
    two=true;
  } else {
    digitalWrite(dosagePump3,LOW);
    two=false;
  }
  resetLCD();
  counter = 0; //counter reset
  if(one==true&&two==true) {
    phase = 1;
    Phase0 = false;
  } else {
    t.after(phase0Timer,phase0Off);
  }
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

/*
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
*/
void loop() {
  t.update(); //Update Timer

  switch(phase) {
    case 0: //Fill up charcoal filters
    lcd.setCursor(0,3);
    lcd.print(intToString(phase0Timer));
    //test = (counter/phase0Timer*100);
    lcd.setCursor(0,2);
    statusBar(counter/phase0Timer*100000);
    if (Phase0 == false) { phase0(); Phase0 = true; }
    break;
    case 1: //Filtration for 1.5 hours
    lcd.setCursor(0,3);    
    lcd.print(intToString(phase1Timer));
    statusBar(counter/phase1Timer*100000);
    if (Phase1 == false) { phase1(); Phase1 = true; }
    break;
    case 2: //Empty Filters
    lcd.setCursor(0,3);    
    lcd.print(intToString(phase2Timer));
    statusBar(counter/phase2Timer*100000);   
    if (Phase2 == false) { phase2(); Phase2 = true; }
    break;
    case 3: //Empty Sterilizer
    lcd.setCursor(0,3);    
    lcd.print(intToString(phase3Timer));
    statusBar(counter/phase3Timer*100000);    
    if (Phase3 == false) { phase3(); Phase3 = true; }
    break; 
  }
}

void resetLCD() {
  lcd.begin(20,4);  // initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.clear();
}
