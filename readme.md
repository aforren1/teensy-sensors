## Teensy tiny sensors

Get this locally with:

    git clone https://github.com/aforren1/teensy-sensors

And update with:

    git pull

Worked examples of sensor fusion (for the MPU-9250):
https://github.com/kriswiner/MPU-9250

And a simpler example using the MPU-9250, minus sensor fusion:
http://www.lucidarme.me/?p=5057

Photo{transistor/detector/sensor/finder/spotter}:
https://www.sparkfun.com/products/246

There's even a tutorial linked.

"Microphone" (Piezo element) tutorial:
https://www.arduino.cc/en/Tutorial/Knock


The photosensor and piezo amount to calling `analogRead()` in a loop, though it might not be *quite* that easy -- see ~~[this post](https://sites.google.com/site/measuringstuff/the-arduino#TOC-Sample-Rates)~~ and [this repo](https://github.com/pedvide/ADC ) for some thoughts. The more complicated accelerometer stuff might take a bit more effort.

The complete schematic can be found here: http://www.schematics.com/project/teensy-sensors-25692/

Other things:

 - Adjusting the sampling rate (what's the max, esp. because the photosensor and piezo are so coarse anyway)
 - Hardware things -- how far away can sensors be before noise is an issue?
 - Logging, streaming, and saving data
 - Displaying data for diagnostics (downsampling ok) ([plotly](https://plot.ly/arduino/tmp36-temperature-tutorial/) would be cool)
 - Serial communication with C++ programs in a timely fashion
