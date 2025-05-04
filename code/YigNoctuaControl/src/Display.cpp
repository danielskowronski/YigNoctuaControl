#include "Display.h"

Display::Display()
{
}
bool Display::begin(uint8_t address, uint8_t width, uint8_t height)
{
  display = Adafruit_SSD1306(width, height, &Wire, -1);
  if (!display.begin(SSD1306_SWITCHCAPVCC, address))
  {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  return true;
}
char *Display::formatTemperature(float temperature, bool decimal, const char *prefixInput)
{
  static char buffer[16];
  char prefix[9];
  strncpy(prefix, prefixInput, 8);
  prefix[8] = '\0';

  if (decimal)
  {
    if (isnan(temperature) || temperature == -127)
    {
      sprintf(buffer, "%s---.-`C", prefix);
    }
    else
    {
      sprintf(buffer, "%s%3d.%1d`C", prefix, int(temperature), (int(temperature * 10) % 10));
    }
  }
  else
  {
    if (isnan(temperature) || temperature == -127)
    {
      sprintf(buffer, "%s---`C", prefix);
    }
    else
    {
      sprintf(buffer, "%s%3d`C", prefix, int(temperature));
    }
  }
  return buffer;
}
char *Display::formatDutyCycle(int duty)
{
  static char buffer[16];
  sprintf(buffer, "%3d%%", duty);
  return buffer;
}
char *Display::formatTacho(int rpm)
{
  static char buffer[16];
  if (rpm > 9999)
  {
    sprintf(buffer, "---- RPM");
  }
  else
  {
    sprintf(buffer, "%4d RPM", rpm);
  }
  return buffer;
}
void Display::update(FanControlChannelStatus fcs, AmbientStatus aS)
{
  char buffer[256];
  display.clearDisplay();

  display.setTextSize(3);
  display.setCursor(0, 0);
  display.write(formatTemperature(fcs.temperatureMax));

  display.setTextSize(3);
  display.setCursor(0, 30);
  display.write(formatDutyCycle(fcs.dutyCycle));

  display.setTextSize(1);
  display.setCursor(0, 56);
  display.write(formatTemperature(fcs.temperatureFirst, false));
  display.setTextSize(1);
  display.setCursor(36, 56);
  display.write(formatTemperature(fcs.temperatureSecond, false));
  display.setTextSize(1);
  display.setCursor(80, 56);
  display.write(formatTacho(fcs.fanSpeed));

  display.display();
}
void Display::update(FanControlChannelStatus fcs1, FanControlChannelStatus fcs2, AmbientStatus aS)
{
  char buffer[256];
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.write(formatTemperature(fcs1.temperatureMax, false));

  display.setTextSize(2);
  display.setCursor(80, 0);
  display.write(formatDutyCycle(fcs1.dutyCycle));

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.write(formatTemperature(fcs1.temperatureFirst, false));
  display.setTextSize(1);
  display.setCursor(36, 20);
  display.write(formatTemperature(fcs1.temperatureSecond, false));
  display.setTextSize(1);
  display.setCursor(80, 20);
  display.write(formatTacho(fcs1.fanSpeed));

  display.drawLine(0, 32, 128, 32, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 36);
  display.write(formatTemperature(fcs2.temperatureMax, false));

  display.setTextSize(2);
  display.setCursor(80, 36);
  display.write(formatDutyCycle(fcs2.dutyCycle));

  display.setTextSize(1);
  display.setCursor(0, 56);
  display.write(formatTemperature(fcs2.temperatureFirst, false));
  display.setTextSize(1);
  display.setCursor(36, 56);
  display.write(formatTemperature(fcs2.temperatureSecond, false));
  display.setTextSize(1);
  display.setCursor(80, 56);
  display.write(formatTacho(fcs2.fanSpeed));

  display.display();
}