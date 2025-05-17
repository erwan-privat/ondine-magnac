#include "Ecran.h"

#include "Heure.h"
#include "Ota.h"
#include "WiFiMagnac.h"
#include <NTPClient.h>
#include <WiFi.h>

namespace Ecran
{
  constexpr auto screen_w    = 128; // px
  constexpr auto screen_h    =  32; // px
  constexpr auto normal_font = u8g2_font_gb24st_t_3;
  constexpr auto small_font  = u8g2_font_t0_11b_tf;
  constexpr auto icon_font   = u8g2_font_open_iconic_all_1x_t;
  constexpr auto ico_wifi    = 0x00f7;
  constexpr auto ico_nope    = 0x0057;
  constexpr auto ico_warn    = 0x0118;
  // constexpr auto max_switch  = 7;

  constexpr auto ota_warn_delay = 500'000u; // µs

  // U8G2_R0 pour sans rotation (paysage)
  OledScreen screen(U8G2_R0);

  char heure[] = "hh:mm:ss";
  // int switch_watts = 0;
  unsigned last_bang_micros = esp_timer_get_time();
  bool warn_toggle = false;
  
  unsigned getLength()
  {
    return screen_w * screen_h / 8;
  }

  OledScreen& getScreen()
  {
    return screen;
  }

  void progressbar(int x, int y, int w, int h, float value)
  {
    screen.drawFrame(x, y, w,  h);
    screen.drawBox(x + 2, y + 2, (w - 4) * value,  h - 4);
  }

  void drawOtaProgress()
  {
    constexpr int margin = 2;
    constexpr int height = 2;

    screen.setDrawColor(2); // XOR
    screen.setCursor(screen_w / 2 - 16, 16);
    screen.setFont(Ecran::normal_font);

    if (Ota::updating)
    {
      screen.print(Ota::progress);
      screen.print(F(" %"));
      progressbar(
        margin, screen_h - margin - height,
        screen_w - 2 * margin, height,
        Ota::progress / 100.0f);
    }
    else
    {
      screen.setDrawColor(1);
      screen.printf(F("OTA error %d"), Ota::error);
    }

    screen.setDrawColor(1);
  }

  void drawWiFiStatus()
  {
    screen.setFont(icon_font);

    auto micros = esp_timer_get_time();

    if(Ota::error != 666
        && micros - ota_warn_delay > last_bang_micros)
    {
      warn_toggle = !warn_toggle;
      last_bang_micros = micros;
    }

    // eplogf("%c\r", warn_toggle ? '!' : '-');
    if (warn_toggle)
      screen.drawGlyph(58, 12, ico_warn);

    if (WiFiMagnac::isConnected())
    {
      screen.drawGlyph(0, screen_h, ico_wifi);
      screen.setFont(small_font);
      screen.setCursor(16, screen_h);
      IPAddress ip = WiFi.localIP();
      screen.print(ip[2]);
      screen.print('.');
      screen.print(ip[3]);
      // screen.print(wifi_rssi);
      // screen.print(F(" dBm"));
    }
    else
    {
      screen.drawGlyph(0, screen_h, ico_nope);
      screen.setFont(small_font);
      screen.setCursor(16, screen_h);
      screen.print(F("WiFi"));
    }
  }

  void drawTime()
  {
    Heure::formatTime(heure);
    screen.setFont(small_font);
    u8g2_uint_t w = screen.getUTF8Width(heure);
    screen.setCursor(screen_w - w, screen_h);
    screen.print(heure);
  }

  void taskScreen(void* argv)
  {
    while (!Heure::isTimeSet())
    {
      screen.clearBuffer();
      screen.setFont(normal_font);
      screen.setCursor(0, 10);
      screen.print("Ondine");
      delay(1000);
    }

    for (;;)
    {
      // on enlève le splash screen quand l'heure est à jour

      // eplogf("task screen %u unused from 3000\r\n",
      //   uxTaskGetStackHighWaterMark(nullptr));
      screen.clearBuffer();
      screen.setDrawColor(1);

      if (Ota::updating)
      {
        drawOtaProgress();
      }
      else
      {
        drawTime();
      }
      
      screen.sendBuffer();
      delay(10);
    }
  }

  // void taskTimerSwitch(void*)
  // {
  //   for (;;) {
  //     switch_watts = (switch_watts + 1) % max_switch;
  //     delay(2000);
  //   }
  // }

  void taskDemo(void*)
  {
    int counter = 0;

    for (;;)
    {
      screen.firstPage();
      do
      {
        if (Ota::updating)
        {
          drawOtaProgress();
        }
        else
        {
          screen.setFont(normal_font);
          screen.setCursor(4, 16);
          screen.print("Ondine");
          drawTime();
        }
      } while (screen.nextPage());

      delay(1000);
      ++counter;
    }
  }

  void begin()
  {
    screen.begin();
    // xTaskCreate(taskScreen, "task screen", 
    //   3000 * 2, nullptr, 3, nullptr);

    // xTaskCreate(taskTimerSwitch, "task screen timer",
    //     3000, nullptr, 2, nullptr);

    xTaskCreate(taskDemo, "task screen demo",
       3000, nullptr, 3, nullptr);
  }
}
