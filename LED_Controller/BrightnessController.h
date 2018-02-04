#ifndef BRIGHTNESS_CONTROLLER_H
#define BRIGHTNESS_CONTROLLER_H

#include <EEPROM.h>
#include "LEDStrip.h"
#include <Timer.h>

class BrightnessController{
public: 
BrightnessController(LEDStrip* strip, uint8_t EEPromAdress);
void update();

void up();
void down();

void startAcelerationMode();
void turnOnRunning();
void turnOff();
void turnOn();

private:
Timer updateTimer;
LEDStrip* ledStrip;
uint8_t startAdress;

uint8_t maximumBrightness;
uint8_t mode;
int effectCounter;

void turnOnAnimation();
void turnOffAnimation();

void loadSettings();
void saveSettings();
};

#endif

