// Ondine - Extension du routeur solaire RBR pour Magnac.
// https://github.com/erwan-privat/ondine-magnac

#include "EpUtil.h"
#include "Ota.h"
#include "WiFiMagnac.h"

#include <WiFi.h>

void setup()
{
  EP_SERIAL.begin(115200); // bauds
  while (!EP_SERIAL)
    yield();
  eplog("Serial ok");

  WiFiMagnac::begin();
  Ota::begin();

  eplog("Ondine !");
}

void loop()
{
  IPAddress ip = WiFi.localIP();
  eplogf("IP: %d.%d\n", ip[2], ip[3]);
  delay(1000);
}
