#include "AmbientThermometer.h"

AmbientThermometer::AmbientThermometer()
{
  initialized = false;
}
bool AmbientThermometer::begin()
{
  if (initialized)
  {
    return true;
  }
  if (!aht.begin())
  {
    Serial.println("Couldn't find sensor!");
    return false;
  }
  initialized = true;
  return true;
}
AmbientStatus AmbientThermometer::getStatus()
{
  AmbientStatus status;
  if (!initialized)
  {
    status.temperature = NAN;
    status.humidity = NAN;
    return status;
  }
  sensors_event_t humidity, temperature;
  aht.getEvent(&humidity, &temperature);
  status.temperature = temperature.temperature;
  status.humidity = humidity.relative_humidity;
  return status;
}