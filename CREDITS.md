# Credits

This project was built from scratch using the following references:

## TCA9548A Datasheet
- **Source:** https://www.ti.com/lit/ds/symlink/tca9548a.pdf
- **Publisher:** Texas Instruments (SCPS207H, revised September 2024)
- **Used for:** I2C protocol behavior, control register specification, reset behavior, addressing scheme

## Wokwi Custom Chips API
- **Source:** https://docs.wokwi.com/chips-api/getting-started
- **License:** Documentation content, no code borrowed
- **Used for:** API function signatures (`i2c_init`, `pin_init`, `pin_watch`, etc.)

## wokwi-api.h Header
- **Source:** https://raw.githubusercontent.com/wokwi/wokwi-chip-api/main/include/wokwi-api.h
- **License:** MIT (Wokwi)
- **Used for:** Required API header for compilation (downloaded at build time)

No third-party code was copied into this project. The chip implementation is original work based on the TI datasheet and Wokwi API documentation.
