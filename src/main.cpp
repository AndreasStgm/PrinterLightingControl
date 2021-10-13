//----------Include----------

#include <Arduino.h>
#include <FastLED.h>

//----------Define----------

#define SW_MODE 18
#define SW_PWR 19

//----------Variable----------

bool rainbowMode = 1;
bool pulsateMode = 0;
bool whiteMode = 0;
bool offMode = 0;

//----------ISR----------

void SwitchModeISR()
{
}
void SwitchPowerISR()
{
}

//----------Function Defenitions----------

void EffectHandler();
void Rainbow(uint8_t rainbowSpeed, uint8_t rainbowSaturation, uint8_t rainbowBrightness);
void Pulsate(uint8_t pulsateSpeed, uint8_t pulsateColor, uint8_t pulsateSaturation);
void StaticWhite(uint8_t staticHue, uint8_t saticSaturation, uint8_t staticBrightness);
void TurnOff();

//----------Setup----------

void setup()
{
  delay(2000); //2 second power on delay for safety/sanity

  pinMode(SW_MODE, INPUT_PULLUP);
  pinMode(SW_PWR, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SW_MODE), SwitchModeISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(SW_PWR), SwitchPowerISR, FALLING);
}

void loop()
{
  EffectHandler();
}

//----------Functions----------

void EffectHandler()
{
  if (rainbowMode == 1 && pulsateMode == 0 && whiteMode == 0 && offMode == 0)
    Rainbow(25, 255, 255);
  else if (rainbowMode == 0 && pulsateMode == 1 && whiteMode == 0 && offMode == 0)
    Pulsate(10, HUE_PINK, 255);
  else if (rainbowMode == 0 && pulsateMode == 0 && whiteMode == 1 && offMode == 0)
    StaticWhite(HUE_PURPLE, 255, 255);
  else if (rainbowMode == 0 && pulsateMode == 0 && whiteMode == 0 && offMode == 1)
    TurnOff();
}

void Rainbow(uint8_t rainbowSpeed, uint8_t rainbowSaturation, uint8_t rainbowBrightness)
{
}
void Pulsate(uint8_t pulsateSpeed, uint8_t pulsateColor, uint8_t pulsateSaturation)
{
}
void StaticWhite(uint8_t staticHue, uint8_t saticSaturation, uint8_t staticBrightness)
{
}
void TurnOff()
{
}