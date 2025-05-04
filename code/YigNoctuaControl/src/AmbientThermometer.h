#pragma once
#include <Arduino.h>
#include <Adafruit_AHTX0.h>

struct AmbientStatus
{
  float temperature;
  float humidity;
};
class AmbientThermometer
{
public:
  AmbientThermometer();
  bool begin();
  AmbientStatus getStatus();

private:
  Adafruit_AHTX0 aht;
  bool initialized = false;
};
