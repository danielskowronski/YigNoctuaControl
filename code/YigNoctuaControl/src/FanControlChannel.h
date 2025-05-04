#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FanController.h>

struct FanControlChannelStatus
{
  bool enabled;
  float temperatureMax;
  float temperatureFirst;
  float temperatureSecond;
  uint8_t dutyCycle;
  uint16_t fanSpeed;
};
class FanControlChannel
{
public:
  FanControlChannel();
  void beginFan(uint8_t sensorPin, uint8_t pwmPin, uint16_t sensorThreshold = 1000);
  void beginThermometers(uint8_t firstThermometerPin, uint8_t secondThermometerPin);
  void setPreferences(int8_t temperatureMin = 25, int8_t temperatureMax = 50, uint8_t dutyMin = 10, uint8_t dutyMax = 100);
  FanControlChannelStatus process(bool noAction = false);

private:
  bool fanStarted = false;
  FanController *fan;

  bool thermometersStarted = false;
  OneWire *oneWire1;
  DallasTemperature *dt1;
  OneWire *oneWire2;
  DallasTemperature *dt2;
  float getTemperature(uint8_t id);
  float getTemperatureMax();

  int8_t tMin = 25;
  int8_t tMax = 50;
  uint8_t dMin = 10;
  uint8_t dMax = 100;
};