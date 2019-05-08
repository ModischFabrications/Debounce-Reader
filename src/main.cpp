#include <Arduino.h>
#include "debouncer.h"

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

Debouncer reader = Debouncer();

void print_state(bool reading)
{
  Serial.print(reader.read(reading));
  Serial.print("#");

  /* 
  spamming Serial-prints blocks incoming commands for the bootloader.
  If you have problems uploading:
  1. Unplug device
  2. Plug in again and press upload simultaneously
  3. Increase this delay or baud if this happens regularly
  */

  delay(200);
}

void print_event(bool reading)
{
  Debouncer::STATE curr_state = reader.read(reading);

  if (curr_state == Debouncer::STATE::ST_RISING)
  {
    Serial.print("R");
  }
  else if (curr_state == Debouncer::STATE::ST_FALLING)
  {
    Serial.print("F");
  }
  
}

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

  print_event(reading);

  if (every(5000))
  {
    Serial.print("♥");
    blink(1);
  }
}
