#include <Arduino.h>

#define ONBOARD 13

const uint8_t PIN_BTN = 2;
const uint16_t T_DEBOUNCE_MS = 50;

void blink(uint8_t number_of_blinks)
{
  for (uint8_t i = 0; i < number_of_blinks; i++)
  {
    digitalWrite(ONBOARD, HIGH);
    delay(100);
    digitalWrite(ONBOARD, LOW);
    delay(100);
  }
}

bool every(uint16_t ms)
{
  static uint32_t next = 0;

  if (next < millis())
  {
    next = millis() + ms;
    return true;
  }
  return false;
}

bool check_button_debounced(bool reading)
{
  // this function will not trigger for the first
  // flank but for the "last", which should be stable

  static unsigned long last_debounce = 0;
  static bool last_state = false;
  static bool current_state = false;

  unsigned long now = millis();

  if (reading != last_state)
  {
    // recent change means it's still bouncing or somebody clicked
    last_debounce = now;
    //return false;
  }

  if ((now - last_debounce) > T_DEBOUNCE_MS)
  {
    // value is old enough to be stable

    if (reading != current_state)
    {
      // this was a flank, act only once
      current_state = reading;

      // positive edge only
      if (current_state)
      {
        return true;
      }
    }
  }

  last_state = reading;

  return false;
}

void setup()
{
  pinMode(ONBOARD, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);

  // announce user-program
  Serial.begin(9600);
  Serial.println("Hello there fellow");
  blink(2);
}

void loop()
{
  bool reading = (digitalRead(PIN_BTN) == LOW); // inverted (pullup)
  if (check_button_debounced(reading))
  {
    Serial.print("*BTN*");
  }

  if (every(2000))
  {
    Serial.print("♥");
    blink(1);
  }
}
