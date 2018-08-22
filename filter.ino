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
const int sensorPin1 = A0;
const int sensorPin2 = A1;

int sense1 = 0;
int sense2 = 0;

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);         // initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.backlight(); // finish with backlight on  
  lcd.setCursor(1,0); //Start at character 4 on line 0
  lcd.print("Urinstand: ");
  //pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(led, OUTPUT);
  pinMode(waterSensor, OUTPUT);
  pinMode(dosagePump1, OUTPUT);
  pinMode(dosagePump2, OUTPUT);
  pinMode(dosagePump3, OUTPUT);
  pinMode(dosagePump4, OUTPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);

  // Relays standardmässig ausschalten (HIGH = aus)
  digitalWrite(dosagePump1,HIGH);
  digitalWrite(dosagePump2,HIGH);
  digitalWrite(dosagePump3,HIGH);
  digitalWrite(dosagePump4,HIGH);   
  
  //t.oscillate(dosagePump, 100000, LOW);
 // t.every(30000, t.pulse(dosagePump2, 5000, HIGH)); 
}
void loop() {

  /*
  updatePump();
  t.update();
}
boolean senseWater1() {
  //Read Pin
  digitalWrite(waterSensor,HIGH);
  Serial.println(analogRead(sensorPin1));
  if (analogRead(sensorPin1) < 100) {
    sense1+=1;
  }else{
    sense1-=1;
  }
  //Check Borders
  sense1 = min(sense1 , 100);
  sense1 = max(sense1 , 0);
  //Return Boolean
  if (sense1 >= 100) {
    return true;
  } else {
    return false;
  }
    digitalWrite(waterSensor,LOW);
}

boolean senseWater2() {
  //Read Pin
  digitalWrite(waterSensor,HIGH);
  if (analogRead(sensorPin2) < 100) {
    sense2+=1;
  }else{
    sense2-=1;
  }
  //Check Borders
  sense2 = min(sense2 , 10000);
  sense2 = max(sense2 , 0);
  //Return Boolean
  if (sense2 >= 10000) {
    return true;
  } else {
    return false;
  }
    digitalWrite(waterSensor,LOW);
}

void updatePump() {
  if (senseWater1() == true) {
    digitalWrite(dosagePump1,LOW);
  } else { 
    digitalWrite(dosagePump1,HIGH);
  }
  
  */
  
}


/*
void buzzer() {
  tone(buzzer, 101); // Send 1KHz sound signal...
  delay(100);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(100);        // ...for 1sec
}
*/
