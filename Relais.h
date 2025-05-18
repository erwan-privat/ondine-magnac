#ifndef RELAIS_H
#define RELAIS_H

namespace Relais
{
  constexpr auto pin_com = 19;
  extern bool force_off;
  extern bool force_on;
  extern bool is_on;

  void begin();
}

#endif /* RELAIS_H */
