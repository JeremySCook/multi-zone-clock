#include "RTClib.h"
#include <Stepper.h>

RTC_DS3231 rtc;

int skip = 0;
const int stepsPerRevolution = 32; //for the motor itself, total steps w/ reduction ~2038

//Stepper clockStepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper clockStepper(stepsPerRevolution, 11, 9, 10, 8);

void setup () {
pinMode(4, INPUT_PULLUP);
pinMode(5, OUTPUT);
digitalWrite(5, LOW); // keep as low permanently to act as ground

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  clockStepper.setSpeed(100);
}

void loop() {
    DateTime now = rtc.now();
    int buttonForwardValue = digitalRead(4);
    //Serial.print(now.minute(), DEC); Serial.print("  "); Serial.print(now.second()); Serial.print("  "); Serial.println(buttonForwardValue);
    //delay(10);
    
    if (buttonForwardValue == LOW){
      //Serial.println("advance stepper 34 steps");
      clockStepper.step(-34); //advance ~1:00     
    }
    if (now.second() == 0){ 
      skip += 1;
      if (skip != 28){
      //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
      clockStepper.step(-34); //advance ~1:00
      delay(2000);
      }
      else{
      skip = 0;
      //Serial.print("1 step skipped to keep correct time"); Serial.print(" skip "); Serial.println(skip);
      clockStepper.step(-33); //advance ~1:00 minus 1 step to keep clock in time      
      delay(2000);
      }
    }
    else{
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW); 
      }
}
