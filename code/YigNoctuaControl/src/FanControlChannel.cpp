#include "FanControlChannel.h"

FanControlChannel::FanControlChannel()
{
  oneWire1 = nullptr;
  dt1 = nullptr;
  oneWire2 = nullptr;
  dt2 = nullptr;
  fan = nullptr;
  this->setPreferences();
}
void FanControlChannel::beginFan(uint8_t sensorPin, uint8_t pwmPin, uint16_t sensorThreshold)
{
  if (fanStarted)
  {
    return;
  }
  fan = new FanController(sensorPin, sensorThreshold, pwmPin);
  fan->begin();
  fan->setDutyCycle(100);
  fanStarted = true;
}
void FanControlChannel::beginThermometers(uint8_t firstThermometerPin, uint8_t secondThermometerPin)
{
  pinMode(firstThermometerPin, INPUT_PULLUP);
  oneWire1 = new OneWire(firstThermometerPin);
  dt1 = new DallasTemperature(oneWire1);
  pinMode(secondThermometerPin, INPUT_PULLUP);
  oneWire2 = new OneWire(secondThermometerPin);
  dt2 = new DallasTemperature(oneWire2);
  dt1->begin();
  dt2->begin();
  thermometersStarted = true;
}
float FanControlChannel::getTemperature(uint8_t id)
{
  assert(id == 1 || id == 2);
  if (!thermometersStarted)
  {
    return -127;
  }
  if (id == 1)
  {
    dt1->requestTemperatures();
    return dt1->getTempCByIndex(0);
  }
  else if (id == 2)
  {
    dt2->requestTemperatures();
    return dt2->getTempCByIndex(0);
  }
  return -127;
}
float FanControlChannel::getTemperatureMax()
{
  float t1 = getTemperature(1);
  float t2 = getTemperature(2);
  return max(t1, t2);
}
FanControlChannelStatus FanControlChannel::process(bool noAction)
{
  FanControlChannelStatus status;
  if (!fanStarted || !thermometersStarted)
  {
    return status;
  }
  status.enabled = true;
  status.temperatureFirst = getTemperature(1);
  status.temperatureSecond = getTemperature(2);
  status.temperatureMax = max(status.temperatureFirst, status.temperatureSecond);

  if (!noAction)
  {
    float tNow = status.temperatureMax; // TODO: make it more sophisticated, including ambient temp

    if (tNow >= tMax)
    {
      fan->setDutyCycle(dMax);
    }
    else if (tNow <= tMin)
    {
      fan->setDutyCycle(dMin);
    }
    else
    {
      fan->setDutyCycle(dMin + (dMax - dMin) * ((tNow - tMin) / (tMax - tMin)));
    }
  }

  status.dutyCycle = fan->getDutyCycle();
  status.fanSpeed = fan->getSpeed();

  return status;
}
void FanControlChannel::setPreferences(int8_t temperatureMin, int8_t temperatureMax, uint8_t dutyMin, uint8_t dutyMax)
{
  assert(temperatureMin < temperatureMax);
  assert(dutyMin < dutyMax);
  assert(dutyMin >= 0 && dutyMax <= 100);
  assert(temperatureMin >= -10 && temperatureMax <= 125); // DS18B20 range

  this->tMin = temperatureMin;
  this->tMax = temperatureMax;
  this->dMin = dutyMin;
  this->dMax = dutyMax;
}
