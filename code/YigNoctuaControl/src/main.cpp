#include <Arduino.h>
#include "FanControlChannel.h"
#include "Console.h"
#include "Display.h"

FanControlChannel fcc1;
FanControlChannelStatus fcs1;
FanControlChannel fcc2;
FanControlChannelStatus fcs2;
AmbientThermometer aT;
AmbientStatus aS;
Display oled;

void setup()
{
  Serial.begin(115200);
  Serial.println("YigNoctuaControl");
  Wire.begin(21, 22);

  fcc1.beginFan(14, 13);
  fcc1.beginThermometers(15, 4);
  fcc1.setPreferences(25, 40, 10, 100);

  fcc2.beginFan(27, 26);
  fcc2.beginThermometers(16, 17);
  fcc2.setPreferences(25, 40, 10, 100);

  aT = AmbientThermometer();
  aT.begin();

  oled = Display();
  oled.begin();
}

void loop()
{
  fcs1 = fcc1.process();
  fcs2 = fcc2.process();
  aS = aT.getStatus();
  Serial.println(getStatusJson(fcs1, fcs2, aS));
  // led.update(fcs1, fcs2, aS);
  oled.update(fcs1, aS);

  delay(500);
}
