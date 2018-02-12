#include "BrightnessController.h"

enum effects{NO_EFFECT, TURNING_OFF,TURNING_ON, ACELERATION_MODE};

BrightnessController::BrightnessController(LEDStrip* strip, uint8_t EEPromAdress):
updateTimer(20, true)
{
    startAdress = EEPromAdress;
    updateTimer.start();
 ledStrip = strip;
  loadSettings();
ledStrip->setMaximumBrightness(maximumBrightness);
  
}

void BrightnessController::update(){
  if(updateTimer.isFinished() && mode != NO_EFFECT){
 switch(mode){
  case TURNING_OFF: turnOffAnimation();
  break;
  case TURNING_ON: turnOnAnimation(); 
  break; 
  default: break;
}

ledStrip->pushToStrip(); 
}
}

void BrightnessController::up(){
  if(maximumBrightness < 255){
  maximumBrightness++;
  ledStrip->setMaximumBrightness(maximumBrightness); 
  }
  saveSettings();
}

void BrightnessController::down(){
  if(maximumBrightness > 0){
  maximumBrightness--;
  ledStrip->setMaximumBrightness(maximumBrightness); 
  }
  saveSettings();
}

void BrightnessController::startAcelerationMode(){
  mode = ACELERATION_MODE;
}

void BrightnessController::turnOnRunning(){
  effectCounter = 0;
  mode = TURNING_ON;
}

void BrightnessController::turnOff(){
  mode = TURNING_OFF;
  effectCounter = (255 + (ELEMENT_LENGTH - 1) * BRIGHTNESS_DIFFERENCE);
}

void BrightnessController::turnOn(){
  ledStrip->setBrightness(255);
  ledStrip->pushToStrip();
}

void BrightnessController::turnOnAnimation(){
effectCounter += 5; 

uint8_t brightnessBuffer[8];

 for(int i = 0; i < ELEMENT_LENGTH; i++){
  int brightness = effectCounter - i*BRIGHTNESS_DIFFERENCE; 
  if(brightness < 0){
    brightnessBuffer[i] = 0;
  }
  else if(brightness > 255){
    brightnessBuffer[i] = 255;
  }
  else{
    brightnessBuffer[i] = brightness;
  }
  
  ledStrip->setEveryElementsBrightnessBuffer(brightnessBuffer);
 }
 if(effectCounter >= (255 + (ELEMENT_LENGTH - 1) * BRIGHTNESS_DIFFERENCE)){
  mode = NO_EFFECT;
 }
}

void BrightnessController::turnOffAnimation(){
effectCounter -= 5;  
uint8_t brightnessBuffer[8];
 for(int i = 0; i < ELEMENT_LENGTH; i++){
  int brightness = effectCounter - i*BRIGHTNESS_DIFFERENCE; 
  if(brightness < 0){
    brightnessBuffer[i] = 0;
  }
  else if(brightness > 255){
    brightnessBuffer[i] = 255;
  }
  else{
    brightnessBuffer[i] = brightness;
  }
  
  ledStrip->setEveryElementsBrightnessBuffer(brightnessBuffer);
 }

 if(effectCounter <= 0){
  mode = NO_EFFECT;
 }
}

void BrightnessController::saveSettings(){
  EEPROM.update(startAdress, 31);
  EEPROM.update(startAdress + 1, maximumBrightness);
}

void BrightnessController::loadSettings(){
  if(EEPROM.read(startAdress) == 31){
    maximumBrightness = EEPROM.read(startAdress + 1);
  }
  else{
    maximumBrightness = 255;
  }
}

