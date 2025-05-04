#pragma once
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>
#include "FanControlChannel.h"
#include "AmbientThermometer.h"

class Display
{
public:
  Display();
  bool begin(uint8_t address = 0x3C, uint8_t width = 128, uint8_t height = 64);
  void update(FanControlChannelStatus fcs, AmbientStatus aS);
  void update(FanControlChannelStatus fcs1, FanControlChannelStatus fcs2, AmbientStatus aS);

private:
  char *formatTemperature(float temperature, bool decimal = true, const char *prefixInput = "");
  char *formatDutyCycle(int duty);
  char *formatTacho(int rpm);
  Adafruit_SSD1306 display;
};
