#ifndef DIMMER_H
#define DIMMER_H

namespace Dimmer
{
  constexpr auto pin_zc = 33;
  constexpr auto pin_com = 25;

  extern bool force_off;
  extern bool force_on;
  extern bool is_on;

  void begin();
}

#endif /* DIMMER_H */
