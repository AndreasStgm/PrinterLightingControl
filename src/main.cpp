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
bool staticColorMode = 0;
bool offMode = 0;

//----------ISR----------

void SwitchModeISR(void)
{
}
void SwitchPowerISR(void)
{
}

//----------Function Defenitions----------

void EffectHandler(void);

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