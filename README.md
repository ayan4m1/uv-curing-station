# DIY Curing Station

This is an open-source hardware project to build a UV curing station for resin models.

## BOM

| Part                        | Quantity |
| --------------------------- | -------- |
| 3.3V Relay Breakout         | 1        |
| UV LED Strip                | 1        |
| Lolin D32                   | 1        |
| Momentary Switch            | 1        |
| 128x64 SSD1306 OLED Display | 1        |

## Assembly

TODO: How to build the UV box

- Connect the momentary switch between GND and pin 17 on the D32.
- Connect the OLED display to 3.3V, GND, SDA, and SCL on the D32.
- Connect the relay input to pin 25 of the D32.

## Usage

Press the button to turn the lights on for 60 seconds. Each additional button press adds 60 seconds to the timer, up to a limit of 16 minutes.

## Roadmap

- Document the UV box build
- Make a 3D printable enclosure
- Add a turntable
