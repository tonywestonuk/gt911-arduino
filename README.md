# GT911 Lite

A lean Arduino library for the GT911 capacitive touch controller, targeting ESP32.

Forked from [TAMC_GT911](https://github.com/TAMCTec/gt911-arduino) with everything stripped out that isn't needed for basic multi-touch reading.

## Why this fork?

The original TAMC_GT911 library does more than most projects need:

- **Config reflashing on every startup** — the original writes a full config block to the chip's registers at boot, including checksum calculation. This is only needed if you're changing the chip's factory config, which most projects never do.
- **Rotation logic** — software coordinate remapping tied to screen orientation. This belongs in your UI layer, not the touch driver.
- **Interrupt handling** — the original includes ISR-based event handling. Polling `read()` in the main loop is simpler and sufficient for most use cases.
- **Multi-point read bug** — the original only read `points[0]` from a single register. This library reads each touch point from its correct register in a loop.

The result is a driver that does one thing: read touch data off the wire.

## What's kept

- I2C address auto-detection (0x5D or 0x14)
- Up to 5 simultaneous touch points
- Per-point `id`, `x`, `y`, `size`
- `isTouched`, `touches`, `down` state
- Change detection — `read()` returns `true` only when state has changed

## Installation

Copy the library folder into your Arduino `libraries` directory, or add it as a local library in your IDE.

## Usage

```cpp
#include "TAMC_GT911.h"
#include "Wire.h"

#define TOUCH_SDA 39
#define TOUCH_SCL 40

TAMC_GT911 tp;
TwoWire wire(0);

void setup() {
  wire.begin(TOUCH_SDA, TOUCH_SCL);
  Serial.begin(115200);
  tp.begin(&wire);
}

void loop() {
  if (tp.read()) {
    for (int i = 0; i < tp.touches; i++) {
      Serial.print("Touch "); Serial.print(i + 1);
      Serial.print("  x: ");    Serial.print(tp.points[i].x);
      Serial.print("  y: ");    Serial.print(tp.points[i].y);
      Serial.print("  size: "); Serial.println(tp.points[i].size);
    }
  }
}
```

## API

### `void begin(TwoWire *wire)`
Initialises the driver. Auto-detects the I2C address. Call after `Wire.begin()`.

### `bool read()`
Reads touch state from the chip. Returns `true` if anything has changed since the last call (position, size, or down/up transition). Call this in your loop.

### Public members

| Member | Type | Description |
|---|---|---|
| `isTouched` | `bool` | True if one or more fingers are currently down |
| `touches` | `uint8_t` | Number of active touch points (0–5) |
| `down` | `bool` | True while any finger is down |
| `x`, `y`, `size` | `uint16_t` | Coordinates and contact size of the first touch point |
| `last_x`, `last_y`, `last_size` | `uint16_t` | Previous values of the first touch point |
| `points[5]` | `TP_Point` | All active touch points |
| `isLargeDetect` | `uint8_t` | Large-touch flag from the chip status register |

### `TP_Point`

| Field | Type | Description |
|---|---|---|
| `id` | `uint8_t` | Touch point ID assigned by the chip |
| `x` | `uint16_t` | X coordinate |
| `y` | `uint16_t` | Y coordinate |
| `size` | `uint16_t` | Contact size |

## Notes

- INT and RST pins are not managed by this library. Wire them up as your hardware requires, but no pin control is done here.
- Coordinate rotation/mapping is not handled. Transform coordinates in your application if needed.
- Tested on ESP32 with Arduino framework.

## License

MIT
