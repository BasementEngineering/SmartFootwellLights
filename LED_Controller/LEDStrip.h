#ifndef LEDStrip_h
#define LEDStrip_h

#include <Adafruit_NeoPixel.h>
const int ELEMENT_LENGTH = 18;

class LEDStrip{

public:
LEDStrip(int pin);

void begin(void);
void off(void);

uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

void setMaximumBrightness(uint8_t newMaximumBrightness);

void setColor(uint32_t color);
void setElementColor(uint8_t element, uint32_t color);
void setElementColorBuffer(uint8_t element, uint32_t colorBuffer[ELEMENT_LENGTH], bool outward = true);
void setEveryElementsColorBuffer(uint32_t colorBuffer[ELEMENT_LENGTH]);

void setBrightness(uint8_t brightness);
void setElementBrightness(uint8_t element, uint8_t brightness);
void setElementBrightnessBuffer(uint8_t element, uint8_t brightnessBuffer[ELEMENT_LENGTH], bool outward = true);
void setEveryElementsBrightnessBuffer(uint8_t newBrightnessBuffer[ELEMENT_LENGTH]);

uint32_t dimmColor(uint32_t color, uint8_t brightness);

void pushToStrip(void);

private:
uint8_t stripLength;
Adafruit_NeoPixel strip;
uint8_t globalBrightnessBuffer[ELEMENT_LENGTH*4];
uint32_t globalColorBuffer[ELEMENT_LENGTH*4];

};
#endif

