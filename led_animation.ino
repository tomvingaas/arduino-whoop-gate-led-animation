#include <FastLED.h>
#include <EEPROM.h>

//WS2812 LED Strip
//arduino

#define NUMBER_OF_LEDS 45
#define DATA_PIN 10
#define BUTTON_1 8
#define HEARTBEAT_DELAY 5
#define DEBOUNCE 100

/* EEPROM Structure
 *  0   animation
 *  1   animation
 *  2+  empty
 */

 int animationChoice;
 int stateButton;
 long debounce;
 int brightness;

 void setup() { 
  //Setup uController
  brightness = 84;
  stateButton=HIGH;
  debounce=0;

  Serial.begin(57600);
  pinMode(BUTTON_1, INPUT_PULLUP);
  Serial.print("Neustart");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUMBER_OF_LEDS);
  LEDS.setBrightness(brightness);
  animationChoice=readIntFromEEPROM(EEPROMADDR_ANIMATION);
  checkAnimations();
}

void loop() { 
  checkButton();//Button Checken
  switch(animationChoice){
    case 0: staticColor(0); break;
    case 1: staticColor(64); break;
    case 2: staticColor(96); break;     
  }
}

void checkAnimations(){
 if(animationChoice>20||animationChoice<0){
    animationChoice=0;    
  }
  else if(animationChoice>13&&animationChoice<20){
    animationChoice=20;    
  }
  else if(animationChoice>2&&animationChoice<10){
    animationChoice=10;
  }
}

bool checkButton(){
    if((DEBOUNCE+debounce+DEBOUNCE)<millis()){
    if(digitalRead(BUTTON_1)==LOW){
      if(stateButton=HIGH){
        animationChoice++;
        checkAnimations();
        writeIntIntoEEPROM(EEPROMADDR_ANIMATION, animationChoice);
        Serial.print("BUTTON_1 - Animation: ");
        Serial.print(animationChoice, DEC);
        Serial.print("\n");
        debounce=millis();        
        stateButton=digitalRead(BUTTON_1);
        return true;
      }
    }
  }
  stateButton=digitalRead(BUTTON_1);
  return false;
}

void staticColor(int hue){
  static int saturation=255;
  static int direction=0;

  for(int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i] = CHSV(hue, 255, saturation);  
  }
  FastLED.show();
  if(direction==0){
    saturation--;
  }
  else{
    saturation++;
  }
  if(saturation>=255){
    direction = 0;
  }
  else if(saturation <=75){
    direction =1;
  }
  delay(HEARTBEAT_DELAY);
}
