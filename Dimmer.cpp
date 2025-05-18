#include "Dimmer.h"

#include <Arduino.h>

namespace Dimmer
{
  bool force_off = true;
  bool force_on = false;
  bool is_on = false;

  void taskChauffage(void*)
  {
    for (;;)
    {

      delay(1000);
    }
  }

  void begin()
  {
    force_off = false;
  }
}
