#ifndef ECRAN_H
#define ECRAN_H

#include <U8g2lib.h>

namespace Ecran
{
  constexpr auto pin_scl = 22;
  constexpr auto pin_sda = 21;

  // U8G2    // nom de la bibliothèque
  // SSD1306 // contrôleur
  // 128X32  // taille en px²
  // WINSTAR // nom de l'écran
  // 1       // pages
  // HW      // Hardware, doit être branché au bon endroit
  // I2C     // protocole I2C/Wire (et non SPI)
  using OledScreen = U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C;
  OledScreen& getScreen();
  unsigned getLength();
  void begin();
}

#endif
