#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "FanControlChannel.h"
#include "AmbientThermometer.h"

String getStatusJson(FanControlChannelStatus stateLeft, FanControlChannelStatus stateRight, AmbientStatus ambient);

// TODO: implement switching display modes
// TODO: implement storing preferences in flash
