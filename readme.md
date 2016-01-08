## Teensy tiny sensors

Worked examples of sensor fusion (for the MPU-9250):

https://github.com/kriswiner/MPU-9250

And a simpler example using the MPU-9250, minus sensor fusion:

http://www.lucidarme.me/?p=5057

Photo{transistor/detector/sensor/finder/spotter}:

https://www.sparkfun.com/products/246

There's even a tutorial linked.

"Microphone" (Piezo element) tutorial:

https://www.arduino.cc/en/Tutorial/Knock


The photosensor and piezo amount to calling `analogRead()` in a loop. The more complicated accelerometer stuff might take a bit more effort.

Other things:

 - Adjusting the sampling rate (what's the max, esp. because the photosensor and piezo are so coarse anyway)
 - Hardware things -- how far away can sensors be before noise is an issue?
 - Logging, streaming, and saving data
 - Displaying data for diagnostics (downsampling ok) ([plotly](https://plot.ly/arduino/tmp36-temperature-tutorial/) would be cool)
 - Serial communication with C++ programs in a timely fashion
