#include "Console.h"

String getStatusJson(FanControlChannelStatus stateLeft, FanControlChannelStatus stateRight, AmbientStatus ambient)
{
  JsonDocument doc;
  String output;

  doc["left"]["enabled"] = stateLeft.enabled;
  doc["left"]["temp"]["ch1"] = stateLeft.temperatureFirst;
  doc["left"]["temp"]["ch2"] = stateLeft.temperatureSecond;
  doc["left"]["temp"]["max"] = stateLeft.temperatureMax;
  doc["left"]["fan"]["duty"] = stateLeft.dutyCycle;
  doc["left"]["fan"]["speed"] = stateLeft.fanSpeed;

  doc["right"]["enabled"] = stateRight.enabled;
  doc["right"]["temp"]["ch1"] = stateRight.temperatureFirst;
  doc["right"]["temp"]["ch2"] = stateRight.temperatureSecond;
  doc["right"]["temp"]["max"] = stateRight.temperatureMax;
  doc["right"]["fan"]["duty"] = stateRight.dutyCycle;
  doc["right"]["fan"]["speed"] = stateRight.fanSpeed;

  doc["ambient"]["temp"] = ambient.temperature;
  doc["ambient"]["humidity"] = ambient.humidity;

  serializeJson(doc, output);
  return output;
}