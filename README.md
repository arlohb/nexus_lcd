# Nexus LCD

**View Prometheus data on a small IoT display**

<img
    src"https://github.com/arlohb/nexus_lcd/blob/main/Images/Photo.jpg"
    alt="A photo of the device"
    width="100"
/>

The goal of this project is constant observability of a Kubernetes cluster. It
shows overall CPU and memory usage, and CPU usage per node. Additionally, I also
connected a LED matrix, currently used to monitor a RAID array in
OpenMediaVault. It will flash red if a drive has failed, and be steady blue if
the array is rebuilding.

FreeRTOS is used heavily to ensure asynchronous updates to each statistic
individually.

The hardware used is [this](https://www.waveshare.com/product/displays/lcd-oled/lcd-oled-3/1.28inch-touch-lcd.htm) Waveshare round display, 
[this](https://www.adafruit.com/product/3405) Adafruit HUZZAH32 ESP32 board,
and [this](https://www.adafruit.com/product/2945) Adafruit FeatherWing 4x8 RGB addon.

The data sources in this project are specific to me, but code could be reused
for other circumstances (provided you follow the AGPLv3).

