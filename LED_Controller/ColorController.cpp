#include "ColorController.h"
#define DEBUG
enum effects{NO_EFFECT, FADING, RAINBOW, CHANGING_COLOR};

ColorController::ColorController(LEDStrip* strip, int EEPromAdress):
  updateTimer(20, true)
{
   updateTimer.start();
  startAdress = EEPromAdress;
  ledStrip = strip;
  loadSettings();

Serial.begin(9600);
Serial.println(mode);
Serial.println(nextColor);
ledStrip->setColor(nextColor);
}

void ColorController::update(bool forceUpdate) 
{
  if ((updateTimer.isFinished() && mode != NO_EFFECT) || forceUpdate) 
  {
    if(!forceUpdate)
    {
    effectCounter++;
    }
    switch (mode) {
      case NO_EFFECT: ledStrip->setColor(nextColor);
      break;
      case FADING: fadeColor();
        break;
      case RAINBOW: doTheRainbow();
        break;
      case CHANGING_COLOR: changeColor();
        break;
      default: break;
    }
    ledStrip->pushToStrip(); 
  }  
}

void ColorController::up() 
{
  if (mode == RAINBOW || mode == FADING) 
  {
    if (speed < 10) {
      speed++;
      updateTimer.setInterval(speed * 20);
    }
  }
}

void ColorController::down() {
  if (mode == RAINBOW || mode == FADING) {
    if (speed > 1) {
      speed--;
      updateTimer.setInterval(speed * 20);
    }
  }
}

void ColorController::changeColorTo(uint32_t newColor) {
  updateTimer.setInterval(100);
  previousColor = nextColor;
  nextColor = newColor;
  mode = CHANGING_COLOR;
  effectCounter = 0;
  
  #ifdef DEBUG
  Serial.print("Starting color change from: ");
   Serial.println(previousColor, HEX);
  Serial.print(" to ");
  Serial.println(nextColor, HEX);
  #endif
}

void ColorController::startColorFading() {
  updateTimer.setInterval(speed * 20);
  mode = FADING;
  effectCounter = 0;
}

void ColorController::startRainbow() {
  updateTimer.setInterval(speed * 20);
  mode = RAINBOW;
  effectCounter = 0;
}

void ColorController::fadeColor() {
  effectCounter %= 255;

  uint32_t newColor = getWheelColor(effectCounter);
  ledStrip->setColor(newColor);
}

void ColorController::changeColor() {
  if (effectCounter == ELEMENT_LENGTH) {
    mode = NO_EFFECT;
  }
  else {
    uint32_t colorBuffer[ELEMENT_LENGTH];
    for (int i = 0; i < ELEMENT_LENGTH; i++) {
      if (i < ELEMENT_LENGTH) {
        colorBuffer[effectCounter] = nextColor;
      }
      else {
        colorBuffer[effectCounter] = previousColor;
      }
    }
    ledStrip->setEveryElementsColorBuffer(colorBuffer);
  }
}

void ColorController::doTheRainbow() {
  effectCounter %= 255;

  uint32_t colorBuffer[ELEMENT_LENGTH * 4];

  for (int i = 0; i < ELEMENT_LENGTH * 4; i++) {
    uint8_t wheelPosition = i + effectCounter;
    colorBuffer[i] = getWheelColor(wheelPosition);
  }

}

uint32_t ColorController::getWheelColor(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return ledStrip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return ledStrip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ledStrip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void ColorController::saveSettings() {
  EEPROM.update(startAdress, 42);
  EEPROM.update(startAdress + 1, mode);
  EEPROM.update(startAdress + 2, speed);
  EEPROMWritelong(startAdress + 3, nextColor);
}

void ColorController::loadSettings() {
  if (EEPROM.read(startAdress) == 42) {
    mode = EEPROM.read(startAdress + 1);
    speed = EEPROM.read(startAdress + 2);
    nextColor = EEPROMReadlong(startAdress + 3);
  }
  else {
    mode = 0;
    speed = 5;
    nextColor = ledStrip->Color(200, 200, 255);
  }
}

void ColorController::EEPROMWritelong(uint8_t address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.update(address, four);
  EEPROM.update(address + 1, three);
  EEPROM.update(address + 2, two);
  EEPROM.update(address + 3, one);
}

long ColorController::EEPROMReadlong(uint8_t address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
