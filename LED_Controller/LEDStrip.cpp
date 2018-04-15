#include "LEDStrip.h"

LEDStrip::LEDStrip(int pin):
strip(ELEMENT_LENGTH * 4, pin, NEO_GRB + NEO_KHZ800)
{  
stripLength = ELEMENT_LENGTH * 4;
}

void LEDStrip::begin(void){
  strip.begin();
  strip.clear();
  strip.show();
}

uint32_t LEDStrip::Color(uint8_t r, uint8_t g, uint8_t b){
  return strip.Color(r,g,b);
}

void LEDStrip::setMaximumBrightness(uint8_t newMaximumBrightness){
  strip.setBrightness(newMaximumBrightness);
}

void LEDStrip::setBrightness(uint8_t brightness){
    for (int i = 0; i < stripLength; i++) {
    globalBrightnessBuffer[i] = brightness;
  }
}

void LEDStrip::setElementBrightness(uint8_t element, uint8_t brightness) {
  uint8_t brightnessBuffer [ELEMENT_LENGTH];
  for ( int i = 0; i < ELEMENT_LENGTH; i++) {
    brightnessBuffer[i] = brightness;
  }
  setElementBrightnessBuffer(element, brightnessBuffer);
}

void LEDStrip::setElementBrightnessBuffer(uint8_t element, uint8_t* brightnessBuffer, bool outward) {
  int startPosition = ELEMENT_LENGTH * element;
  int endPosition = ELEMENT_LENGTH * (element + 1);

  if (outward) {
    int j = 0;
    for (int i = startPosition; i < endPosition; i++) {
      globalBrightnessBuffer[i] = brightnessBuffer[j];
      j++;
    }
  }
  else if (!outward) {
    int j = 0;
    for (int i = endPosition; i > startPosition; i--) {
      globalBrightnessBuffer[i] = brightnessBuffer[j];
      j++;
    }
  }
}

void LEDStrip::setEveryElementsBrightnessBuffer(uint8_t newBrightnessBuffer[ELEMENT_LENGTH]){
  for(int i = 0; i < 4; i++){
    setElementBrightnessBuffer(i, newBrightnessBuffer, true);
  }
}



void LEDStrip::setColor(uint32_t color){
    for (int i = 0; i < stripLength; i++) {
    globalColorBuffer[i] = color;
  }
}

void LEDStrip::setElementColor(uint8_t element, uint32_t color){
  uint32_t colorBuffer[ELEMENT_LENGTH];
  for (int i = 0; i < ELEMENT_LENGTH; i++) {
    colorBuffer[i] = color;
  }
  setElementColorBuffer(element, colorBuffer);
}

void LEDStrip::setElementColorBuffer(uint8_t element, uint32_t * colorBuffer, bool outward) {
  int startPosition = ELEMENT_LENGTH * element;
  int endPosition = ELEMENT_LENGTH * (element + 1);

  if (outward) {
    int j = 0;
    for (int i = startPosition; i < endPosition; i++) {
      globalColorBuffer[i] = colorBuffer[j];
      j++;
    }
  }
  else if (!outward) {
    int j = 0;
    for (int i = endPosition; i > startPosition; i--) {
      globalColorBuffer[i] = colorBuffer[j];
      j++;
    }
  }
}

void LEDStrip::setEveryElementsColorBuffer(uint32_t colorBuffer[ELEMENT_LENGTH]){
  for(int i = 0; i < 4; i++){
 setElementColorBuffer(i, colorBuffer, true);
  }
}

void LEDStrip::setStripsColorBuffer(uint32_t colorBuffer[ELEMENT_LENGTH*4])
{
  for(int i = 0; i < ELEMENT_LENGTH*4; i++){
  globalColorBuffer[i] = colorBuffer[i];
  }
}

uint32_t LEDStrip::dimmColor(uint32_t color, uint8_t brightness) {

  if (brightness == 0)
  {
    return 0;
  }
  else if (brightness == 255)
  {
    return color;
  }
  else {

#ifdef DEBUG3
    Serial.println("Dimming Color Value");
#endif

    uint8_t r, g, b;

    r = (uint8_t)(color >> 16),
    g = (uint8_t)(color >>  8),
    b = (uint8_t)color;

#ifdef DEBUG3
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
#endif

    r = ((r * brightness) >> 8) + 1;
    g = ((g * brightness) >> 8) + 1;
    b = ((b * brightness) >> 8) + 1;

#ifdef DEBUG3
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
#endif

    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b ;
  }
}

void LEDStrip::pushToStrip(void) {
  for ( int i = 0; i < stripLength; i++) {
    uint8_t brightness = globalBrightnessBuffer[i];
    uint32_t color = globalColorBuffer[i];
    strip.setPixelColor(i, dimmColor(color, brightness));
  }
  strip.show();
}

void LEDStrip::off(void){
  strip.clear();
  strip.show();
}


