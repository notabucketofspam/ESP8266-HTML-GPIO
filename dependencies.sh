#!/usr/bin/env bash
yes | rm -r components; mkdir components; cd components
git clone https://github.com/notabucketofspam/ESP8266-general-include.git
cp "ESP8266-general-include/Kconfig.projbuild" ../main
cp "ESP8266-general-include/sdkconfig.defaults" ..
cp "ESP8266-general-include/partitions_1M.csv" ..
cp "ESP8266-general-include/partitions_4M.csv" ..
cd ..
