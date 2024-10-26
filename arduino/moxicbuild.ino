#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


byte volume = 22; //กำหนดระดับความดัง 0 - 30
byte prev_volume = -1;

SoftwareSerial mySoftwareSerial(3, 2); // TX, RX
DFRobotDFPlayerMini myDFPlayer;

bool isPause = false;
bool initi = true;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(volume);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  
}

void loop()
{
  int busyState = digitalRead(7);
  int sw_prev = digitalRead(8);
  int sw_toggle = digitalRead(9);
  int sw_next = digitalRead(10);
  int potValue = analogRead(A0);
  byte new_volume = potValue/35;
  
  if (initi) {
    Serial.println(F("Play Young Dumb"));
    myDFPlayer.play(1);
    initi = false;
  }
  if (sw_toggle == 0) {
    Serial.println(F("Toggle"));
    if (isPause) {
      myDFPlayer.start();
      isPause = !isPause;
    }
    else {
      myDFPlayer.pause();
      isPause = !isPause;
    }
    delay(500);
  }

  if (sw_next == 0) {
    Serial.println(F("Next"));
    myDFPlayer.next();
    delay(500);
  }

  if (sw_prev == 0) {
    Serial.println(F("Previous"));
    myDFPlayer.previous();
    delay(500);
  }

  Serial.print(F("Volume: "));
  Serial.println(new_volume);
  delay(100);

  if(new_volume != prev_volume){
    prev_volume = new_volume;
    myDFPlayer.volume(new_volume);
    delay(100);
  }
}