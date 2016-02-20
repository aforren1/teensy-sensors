### To use the app:

1. Install [R](https://www.r-project.org/), preferably the most recent
2. Install `shiny`, `shinythemes`, `serial`, `tidyr`, `dplyr`, and `ggvis` (plus dependencies) from CRAN (`install.packages('<package>')`) (with ticks or quotes!)
3. Make a directory (eg. `serialport`) and download the [app.R](https://github.com/aforren1/teensy-sensors/blob/master/R-app/serial_example/app.R)  into it.
4. Start R, and type something like `shiny::runApp('~/path/to/serialport')`. This *should* start up the web browser, and plot either real data if you have a serial device, or fake data if you don't.