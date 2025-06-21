# Hardware

## BOM

### Boards and connectors

- 70x90mm double-sided perf-board
- 20x80mm double-sided perf-board - cut at 52mm
- 3x angled goldpin pins 4-pin
- Kynar wire wrap for signals and 3V3 rail
- thicket cable for 5V rail

### MCU

- ESP32 DevKit compatible with NodeMCU-32S ("ESP32S-CP2102-TYPE C") - 38 pins, USB-C
- 2x 19-pin goldpin socket

### Fan

- 1-2x high quality 4-pin PWM **5V** computer fan (ideally Noctua, I used 120mm `NF-F12 5V PWM`)
- 2x 4-pin computer fan connector with pins (modified KF2510 work)
- large screw terminal blocks with 5.08mm raster - 2 pin
- PPTC 1.1A (e.g. `RXE X72 X110`)

### Thermometers

- 4x small screw terminal blocks with 2.54mm raster - 3 pin
- 1-4x DS18B20 in waterproof case on 1m cable (ideally with flat probe)
- 4x 4K7 pull-up resistor
- ambient temperature and humidity sensor `AHT20`

### Display

- 128x64 OLED (SSD1306) 0.96"
- JST XH2.54 4-pin cable with plug and socket (for OLED)

### Case

Case itself:

- durable filament, e.g. Hyper PLA-CF approx 55g
- optional self-adhesive rubber feet

Connecting bottom of perfboard with bottom part of the case:

- 4x M2 brass hex stand-off 5+3mm
- 4x M2x8mm flat-head screw

Connecting top of perfboard with top part of the case:

- 4x M2 brass hex stand-offs in total length of 30mm, for example:
  - 4x 20mm
  - 4x 10+3mm
- 4x M2x8mm flat-head screw

Connecting riser board with thermometer connectors to back of the case:

- 1x M2 nut
- 1x M2 brass hex stand-off 10+3mm
- 1x M2x12 flat-head screw

Mounting OLED display:

- 4x M3x12 flat-head screw
- 8x M3 nut -> it's very important to put nut between display and case not to break glass

## Pinout

I2C:

- SDA: 21
- SCL: 22

Thermometers:

- L1: 15
- L2: 04
- R1: 16
- R2: 17

Fan left:

- PWM control: 13
- tachometer: 14

Fan right:

- PWM control: 26
- tachometer: 27

## Boards

Plan done in DIYLC - [./YNC.diy](./YNC.diy). The smaller board connected to the main one using angled goldpin connectors.
