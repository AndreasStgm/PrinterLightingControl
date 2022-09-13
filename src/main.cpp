//----------Include----------

#include <Arduino.h>
#include <FastLED.h>

//----------Define----------

#define SW_MODE 2
#define SW_PWR 3

#define TRIM_PIN A0

#define DATA_PIN 4
#define NUM_LEDS 20
CRGB leds[NUM_LEDS];

//----------Variable----------

uint8_t debounceTime = 200;

bool rainbowMode = 1;
bool pulsateMode = 0;
bool whiteMode = 0;
bool offMode = 0;

//----------ISR----------

void CycleModeISR()
{
  static unsigned long last_cycle_interrupt_time = 0;
  unsigned long cycle_interrupt_time = millis();

  if (cycle_interrupt_time - last_cycle_interrupt_time > debounceTime)
  {
    if (offMode != 1)
    {
      if (rainbowMode == 1)
      {
        rainbowMode = 0;
        pulsateMode = 1;
      }
      else if (pulsateMode == 1)
      {
        pulsateMode = 0;
        whiteMode = 1;
      }
      else if (whiteMode == 1)
      {
        whiteMode = 0;
        rainbowMode = 1;
      }
    }
  }
  last_cycle_interrupt_time = cycle_interrupt_time;
}
void SwitchPowerISR()
{
  static unsigned long last_power_interrupt_time = 0;
  unsigned long power_interrupt_time = millis();

  if (power_interrupt_time - last_power_interrupt_time > debounceTime)
  {
    if (offMode == 0)
    {
      offMode = 1;
      rainbowMode = 0;
      pulsateMode = 0;
      whiteMode = 0;
    }
    else if (offMode == 1)
    {
      offMode = 0;
      rainbowMode = 0;
      pulsateMode = 0;
      whiteMode = 1;
    }
  }
  last_power_interrupt_time = power_interrupt_time;
}

//----------Function Definitions----------

void EffectHandler();
int ReadTrimPot();
void Rainbow(uint8_t rainbowSaturation, uint8_t rainbowBrightness);
void Pulsate(uint8_t pulsateSpeed, uint8_t pulsateSaturation);
void StaticWhite();
void TurnOff();

//----------Setup----------

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000); // 2 second power on delay for safety/sanity
  digitalWrite(LED_BUILTIN, LOW);

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  pinMode(SW_MODE, INPUT_PULLUP);
  pinMode(SW_PWR, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SW_MODE), CycleModeISR, FALLING);
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
    Rainbow(255, 255);
  else if (rainbowMode == 0 && pulsateMode == 1 && whiteMode == 0 && offMode == 0)
    Pulsate(10, 255);
  else if (rainbowMode == 0 && pulsateMode == 0 && whiteMode == 1 && offMode == 0)
    StaticWhite();
  else if (rainbowMode == 0 && pulsateMode == 0 && whiteMode == 0 && offMode == 1)
    TurnOff();
}

int ReadTrimPot()
{
  int readValue = analogRead(TRIM_PIN) * 256 / 1024;

  if (readValue == 0 || readValue == 255)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  return readValue;
}

void Rainbow(uint8_t rainbowSaturation, uint8_t rainbowBrightness)
{
  for (int rainbowHue = 0; rainbowHue < 256; rainbowHue++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CHSV(rainbowHue, rainbowSaturation, rainbowBrightness);

    if (rainbowMode == 0 && (pulsateMode == 1 || whiteMode == 1 || offMode == 1))
      break;

    delay(ReadTrimPot());
    FastLED.show();
  }
}
void Pulsate(uint8_t pulsateSpeed, uint8_t pulsateSaturation)
{
  for (int brightnessPulse = 0; brightnessPulse < 512; brightnessPulse++)
  {
    if (brightnessPulse < 256)
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(ReadTrimPot(), pulsateSaturation, brightnessPulse);
    else
      for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CHSV(ReadTrimPot(), pulsateSaturation, 511 - brightnessPulse);

    if (pulsateMode == 0 && (rainbowMode == 1 || whiteMode == 1 || offMode == 1))
      break;

    delay(pulsateSpeed);
    FastLED.show();
  }
}
void StaticWhite()
{
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV(0, 0, ReadTrimPot());
  FastLED.show();
}
void TurnOff()
{
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB::Black;
  FastLED.show();
}