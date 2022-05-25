#include <Arduino.h>
#include <Bm437_Trident_9x16.h>
#include <Button2.h>
#include <stdio.h>

// clang-format off
#include <tft_io.hpp>
#include <ssd1306.hpp>
#include <gfx_cpp14.hpp>
// clang-format on

#define PIN_BUTTON 17
#define PIN_RELAY 25

#define PIN_NUM_SDA 21
#define PIN_NUM_SCL 22
#define PIN_NUM_RST -1
#define PIN_NUM_DC -1
#define LCD_PORT 0
#define LCD_WRITE_SPEED_PERCENT 400
#define LCD_ADDRESS 0x3C
#define LCD_ROTATION 0
#define LCD_BIT_DEPTH 4
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

using namespace arduino;
using namespace gfx;

using bus_type = tft_i2c_ex<LCD_PORT, PIN_NUM_SDA, PIN_NUM_SCL>;
using lcd_type =
    ssd1306<LCD_WIDTH, LCD_HEIGHT, bus_type, LCD_ROTATION, LCD_BIT_DEPTH,
            LCD_ADDRESS, true, LCD_WRITE_SPEED_PERCENT, PIN_NUM_DC, PIN_NUM_RST,
            true>;
lcd_type lcd;

using lcd_color = color<typename lcd_type::pixel_type>;

Button2 button = Button2();

uint32_t timeoutMs;
int16_t timeoutSec;
bool relayOn;

void tap(Button2& btn) {
  if (!relayOn) {
    relayOn = true;
  }

  if (timeoutSec <= 1e3) {
    timeoutSec += 60;
    timeoutMs = millis();
  }
}

void setup() {
  timeoutMs = millis();
  timeoutSec = 0;
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
  button.begin(PIN_BUTTON);
  button.setTapHandler(tap);
}

void loop() {
  button.loop();

  if (millis() - timeoutMs > 1e3) {
    timeoutMs = millis();
    if (timeoutSec > 0) {
      --timeoutSec;
      if (timeoutSec == 0) {
        relayOn = false;
      }
    }
  }

  digitalWrite(PIN_RELAY, relayOn ? HIGH : LOW);

  lcd.suspend();

  lcd.clear(lcd.bounds());

  const font& f = Bm437_Trident_9x16_FON;
  srect16 topHalf = (srect16)lcd.bounds();
  topHalf.y2 /= 2;
  srect16 bottomHalf = (srect16)lcd.bounds();
  bottomHalf.y1 = bottomHalf.y2 / 2;

  if (relayOn) {
    const char* statusText = "ON";
    ssize16 statusTextSize =
        f.measure_text((ssize16)lcd.dimensions(), statusText);
    srect16 statusTextRect = statusTextSize.bounds().center(topHalf);
    draw::text(lcd, statusTextRect, statusText, f, lcd_color::white);
    uint8_t remainingMin = timeoutSec / 60;
    char text[16];
    if (remainingMin > 0) {
      sprintf(text, "%dm %ds", remainingMin, timeoutSec - (remainingMin * 60));
    } else {
      sprintf(text, "%ds", timeoutSec);
    }
    ssize16 textSize = f.measure_text((ssize16)lcd.dimensions(), text);
    srect16 textRect = textSize.bounds().center(bottomHalf);
    draw::text(lcd, textRect, text, f, lcd_color::white);
  } else {
    const char* statusText = "OFF";
    ssize16 statusTextSize =
        f.measure_text((ssize16)lcd.dimensions(), statusText);
    srect16 statusTextRect = statusTextSize.bounds().center(topHalf);
    draw::text(lcd, statusTextRect, statusText, f, lcd_color::white);
  }

  lcd.resume();
}
