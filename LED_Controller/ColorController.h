#ifndef COLOR_CONTROLLER_H
#define COLOR_CONTROLLER_H

#include <EEPROM.h>
#include <Timer.h>
#include "LEDStrip.h"

class ColorController {
  public:
    ColorController(LEDStrip* strip, int EEPromAdress);
    void update(bool forceUpdate = false);

    void up();
    void down();

    void changeColorTo(uint32_t newColor);
    void startColorFading();
    void startRainbow();

  private:
    void fadeColor();
    void changeColor();
    void doTheRainbow();

    uint32_t getWheelColor(uint8_t wheelPosition);

    void saveSettings();
    void loadSettings();
    void EEPROMWritelong(uint8_t address, long value);
    long EEPROMReadlong(uint8_t address);

    Timer updateTimer;
    LEDStrip* ledStrip;
    uint8_t brightness;
    uint8_t startAdress;

    uint8_t mode;
    uint8_t speed;
    int effectCounter;

    uint32_t previousColor;
    uint32_t nextColor;
};

#endif
