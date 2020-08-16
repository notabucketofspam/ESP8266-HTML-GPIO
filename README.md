# ESP8266-HTML-GPIO
Simple web interface for controlling pins.

---

Currently only works with GPIO input and output control, interrupt feature is 
planned.

Important: Always use CORE_DEFAULT_CONFIG() when generating program config, and 
then modify settings from there.

Order of operations:
- `make flash`
- `./mkspiffs_xM.sh`
- `make app-flash`

SPIFFS metadata must be zero instead of four in menuconfig.
