# TCA9548A I2C Multiplexer — Wokwi Custom Chip

A [Wokwi](https://wokwi.com/) custom chip simulating the [TCA9548A](https://www.ti.com/product/TCA9548A) 8-channel I2C multiplexer (Adafruit 2717 breakout form factor).

## Features

- Configurable 7-bit I2C address via A0, A1, A2 pins (0x70–0x77)
- 8-bit control register — each bit enables/disables one downstream channel
- Active-low hardware reset (RST pin)
- Power-on default: all channels disabled (0x00)

## Pin Table

| Pin | Side  | Description             |
|-----|-------|-------------------------|
| VIN | Left  | Power input             |
| GND | Left  | Ground                  |
| SDA | Left  | I2C data (upstream)     |
| SCL | Left  | I2C clock (upstream)    |
| RST | Left  | Active-low reset        |
| A0  | Left  | Address bit 0           |
| A1  | Left  | Address bit 1           |
| A2  | Left  | Address bit 2           |
| SD0 | Left  | Channel 0 data          |
| SC0 | Left  | Channel 0 clock         |
| SD1 | Left  | Channel 1 data          |
| SC1 | Left  | Channel 1 clock         |
| SD2 | Right | Channel 2 data          |
| SC2 | Right | Channel 2 clock         |
| SD3 | Right | Channel 3 data          |
| SC3 | Right | Channel 3 clock         |
| SD4 | Right | Channel 4 data          |
| SC4 | Right | Channel 4 clock         |
| SD5 | Right | Channel 5 data          |
| SC5 | Right | Channel 5 clock         |
| SD6 | Right | Channel 6 data          |
| SC6 | Right | Channel 6 clock         |
| SD7 | Right | Channel 7 data          |
| SC7 | Right | Channel 7 clock         |

## Usage in Wokwi

Add the chip to your `diagram.json`:

```json
{
  "type": "chip-tca9548a",
  "id": "mux1",
  "attrs": { "github": "7ax/wokwi-chip-tca9548a@1.0.0" }
}
```

Wire the upstream I2C bus (SDA/SCL) and connect A0, A1, A2 to set the address. Tie RST to VCC (or leave floating — it has an internal pull-up) for normal operation.

### Example Arduino Sketch

```cpp
#include <Wire.h>

#define TCA9548A_ADDR 0x70

void selectChannel(uint8_t channel) {
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

uint8_t readChannels() {
  Wire.requestFrom(TCA9548A_ADDR, 1);
  return Wire.read();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Enable channel 0
  selectChannel(0);
  Serial.print("Active channels: 0x");
  Serial.println(readChannels(), HEX);

  // Enable channels 2 and 5 simultaneously
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write((1 << 2) | (1 << 5));
  Wire.endTransmission();
  Serial.print("Active channels: 0x");
  Serial.println(readChannels(), HEX);

  // Disable all channels
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();
  Serial.print("Active channels: 0x");
  Serial.println(readChannels(), HEX);
}

void loop() {}
```

## Known Limitations

**No true I2C bus bridging.** The real TCA9548A is an electrical bus switch — it physically connects the upstream SDA/SCL lines to the selected downstream SDn/SCn lines. Wokwi's custom chips API does not support dynamic electrical bus bridging between pins.

This chip implements the **control interface only**: writing the channel selection register, reading it back, and hardware reset. The downstream channel pins (SD0–SD7, SC0–SC7) are declared but do not electrically bridge to the upstream bus.

**Workaround for Wokwi projects:** Wire all downstream I2C devices directly to the same Wokwi I2C bus. Since each device has its own unique I2C address, they coexist on the bus. Your firmware still writes to the TCA9548A to select channels (maintaining code compatibility with real hardware), but in simulation, the downstream devices respond regardless of which channel is selected.

## I2C Address Configuration

| A2 | A1 | A0 | Address |
|----|----|----|---------|
| 0  | 0  | 0  | 0x70    |
| 0  | 0  | 1  | 0x71    |
| 0  | 1  | 0  | 0x72    |
| 0  | 1  | 1  | 0x73    |
| 1  | 0  | 0  | 0x74    |
| 1  | 0  | 1  | 0x75    |
| 1  | 1  | 0  | 0x76    |
| 1  | 1  | 1  | 0x77    |

## Building Locally

Requirements: [WASI SDK](https://github.com/WebAssembly/wasi-sdk) installed at `/opt/wasi-sdk` (or set `WASI_SDK_PATH`).

```bash
make
```

This downloads `wokwi-api.h` if missing and produces `dist/chip.wasm` and `dist/chip.json`.

## Datasheet

[TCA9548A Datasheet (TI)](https://www.ti.com/lit/ds/symlink/tca9548a.pdf)

## License

[MIT](LICENSE)
