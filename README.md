# ESP8266-HTML-GPIO
Simple web interface for controlling pins.

---

Currently only works with GPIO output control.

Planned features:

- Input / interrupts, not using `XMLHttpRequest` polling
- Persistent pin state save / load
- Remove `Page` button in HTML

Important: Always use CORE_DEFAULT_CONFIG() when generating program config, and 
then modify settings from there.

Order of operations:

- `make flash`
- `./mkspiffs_xM.sh`
- `make app-flash`

SPIFFS metadata must be zero instead of four in menuconfig, this will be 
automated in a later revision.
