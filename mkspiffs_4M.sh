#!/bin/sh
echo "Default serial port..."
read default_serial_port
cd mkspiffs; git submodule update --init; 
make dist CPPFLAGS="-DSPIFFS_ALIGNED_OBJECT_INDEX_TABLES=1"
./mkspiffs -c ../spiffs_image/ -b 4096 -p 256 -s 0x300000 ../spiffs.bin
cd ..
cd esptool
pip install --user -e .
./esptool.py --port $default_serial_port --before default_reset write_flash 0x100000 ../spiffs.bin
cd ..
