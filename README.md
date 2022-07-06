# LVGL8 + LOVYANGFX
### Demo of LVGL Widgets + SD SPI working together
> 1. [Unexpected Maker FeatherS3](https://esp32s3.com/feathers3.html)   
> 2. [Adafruit 2.4" TFT Featherwing](https://www.adafruit.com/product/3315)   
> 3. [LovyanGFX](https://github.com/lovyan03/LovyanGFX) Display & Touch Driver   
> 4. [LVGL 8.3.0](https://github.com/lvgl/lvgl) UI Widgets   

### Demo of LVGL Widgets 
> 1. [Unexpected Maker TinyS3](https://esp32s3.com/tinys3.html)   
> 2. 1.3" TFT - ST7789 240x240
> 3. [LovyanGFX](https://github.com/lovyan03/LovyanGFX) Display & Touch Driver   
> 4. [LVGL 8.3.0](https://github.com/lvgl/lvgl) UI Widgets   

### Demo of LVGL Widgets using Wireless Tag WT32-SC01 with Capacitive Touch
> 1. [Wireless Tag WT32-SC01 (3.5" TFT Touch Display)](https://www.alibaba.com/product-detail/esp32-development-board-WT32-SC01-3_62534911683.html)   
> 3. [LovyanGFX](https://github.com/lovyan03/LovyanGFX) Display & Touch Driver   
> 4. [LVGL 8.3.0](https://github.com/lvgl/lvgl) UI Widgets   


## Get Started
> Git clone and recursively update submodule
```cmd
git clone https://github.com/sukesh-ak/LVGL8x-SDSPI-Template.git
cd LVGL8x-SDSPI-Template
git submodule update --init --recursive
```

## Separate build folder for ESP32 & ESP32-S3
> Check settings in CMakeLists.txt [here](CMakeLists.txt#L8)
### WT32-SC01 - ESP32
```cmake
# set target and build,flash,monitor
idf.py -B build-esp32 set-target esp32 build
idf.py -B build-esp32 -p COM6 flash monitor
```

### FeatherS3 - ESP32-S3
```cmake
# set target and build,flash,monitor
idf.py -B build-esp32s3 set-target esp32s3 build
idf.py -B build-esp32s3 -p COM3 app-flash monitor
```

## Setup custom lvgl config - ESP-IDF  
> Check settings in CMakeLists.txt [here](CMakeLists.txt#L15)
```cmake
#LVGL custom config file setup
idf_build_set_property(COMPILE_OPTIONS "-DLV_CONF_INCLUDE_SIMPLE=1" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-I../main" APPEND)
```

## Display Compile Time Information
Check settings in CMakeLists.txt [here](CMakeLists.txt#L24)  
```cmake
# Display Compile Time Information
message(STATUS "--------------Compile Info------------")
message(STATUS "IDF_PATH = ${IDF_PATH}")
message(STATUS "IDF_TARGET = ${IDF_TARGET}")
message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")
message(STATUS "PROJECT_DIR = ${PROJECT_DIR}")
message(STATUS "BUILD_DIR = ${BUILD_DIR}")
message(STATUS "SDKCONFIG = ${SDKCONFIG}")
message(STATUS "SDKCONFIG_DEFAULTS = ${SDKCONFIG_DEFAULTS}")
message(STATUS "CONFIG_LV_CONF_SKIP = ${CONFIG_LV_CONF_SKIP}")
message(STATUS "COMPILE_OPTIONS = ${COMPILE_OPTIONS}")
message(STATUS "---------------------------------------")
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message(STATUS "---------------------------------------")
```