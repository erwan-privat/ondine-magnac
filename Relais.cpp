#include "Relais.h"
#include "Ota.h"

#include <Arduino.h>

namespace Relais
{
  bool force_off = true;
  bool force_on = false;
  bool is_on = true;

  void taskPompe(void*)
  {
    for (;;)
    {
      force_off |= Ota::updating;

      if (force_off)
        digitalWrite(pin_com, LOW);
      else if (force_on)
        digitalWrite(pin_com, HIGH);
      else 
        digitalWrite(pin_com, is_on ? HIGH : LOW);

      delay(1000);
    }
  }

  void begin()
  {
    pinMode(pin_com, OUTPUT);

    xTaskCreate(taskPompe, "task pompe", 5000, nullptr, 5, nullptr);
    force_off = false;
  }
}
