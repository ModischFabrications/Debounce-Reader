#include <Arduino.h>
#include "debounce_reader.h"

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

Debounce_Reader reader = Debounce_Reader();

void setup()
{
  pinMode(ONBOARD, OUTPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);

  // faster baud means less time spend off-code
  //Serial.begin(38400);
  Serial.begin(9600);
  Serial.println("Hello there fellow");
  blink(2);
}

void loop()
{
  bool reading = (digitalRead(PIN_BTN) == LOW); // inverted (pullup)

  /* live reading
  Serial.print(reader.read(reading));
  Serial.print("#");
  delay(200);
  */

  // event only
  if (reader.read(reading) == Debounce_Reader::STATE::ST_RISING)
    Serial.print("X");

  if (every(5000))
  {
    Serial.print("♥");
    blink(1);
  }
}
