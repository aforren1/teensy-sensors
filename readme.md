## Teensy tiny sensors

Get this locally with:

    git clone https://github.com/aforren1/teensy-sensors

And update with:

    git pull

The complete schematic can be found here:https://easyeda.com/normal/testproject_schem-02BzxvtPM.png

Other things:

  - How far away can sensors be before noise is an issue?
  - With the advent of the Teensy 3.5 and 3.6, what cool things can we do?
    - Better distribution across ADCs (TODO: figure out new ADC_0/ADC_1 mapping)
    - Generally faster conversion
    - Communication over the 340Mb USB
    - [Audio stimulation](https://github.com/PaulStoffregen/Audio) (though latency is USB/serial latency + audio latency itself, which might still be <5ms)
    - Digital acquisition (knobs, fMRI triggers, ...)
