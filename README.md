# Minime
STM32F042 based Minifigure shaped board.

This is a heavily-modified [MiniSam](https://github.com/bwshockley/Minifigure-SAMD21E), originally designed by Benjamin Shockley using the SAMD21E.

I used the MiniSam board outline and replaced the MCU with an **STM32F042**, replaced the USB connector with
a PCB edge connector, used an RGB LED connected to PWMs and closed one of the hands to hang in a keychain.

The firmware included dims the red LED using PWM, it should be possible to do other effects.
