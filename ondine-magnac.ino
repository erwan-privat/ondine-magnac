// Ondine - Extension du routeur solaire RBR pour Magnac.
// https://github.com/erwan-privat/ondine-magnac

#include "EpUtil.h"
#include "Ecran.h"
#include "WiFiMagnac.h"
#include "Ota.h"
#include "Heure.h"
#include "Relais.h"

#include <WiFi.h>

void setup()
{
  EP_SERIAL.begin(115200); // bauds
  while (!EP_SERIAL)
    yield();
  eplog("Serial ok");

  Ecran::begin();
  WiFiMagnac::begin();
  Ota::begin();
  Heure::begin();
  Relais::begin();

  eplog("Ondine !");
}

void loop()
{
  IPAddress ip = WiFi.localIP();
  eplogf("IP: %d.%d; relay: %d\r", ip[2], ip[3], Relais::is_on);
  delay(1000);
}
