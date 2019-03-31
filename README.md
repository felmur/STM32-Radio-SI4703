# STM32-Radio-SI4703
An STM32duino library to control radio chips SI4703-HW322.

Tested on Linux & 'Arduino IDE' v 1.8.9 (installed for STM32duino).

Hardware: SI4703 HW-322.

# DESCRIPTION
This library is a fork of the original radio library by Matthias Hertel. You can find the original library here:
http://www.mathertel.de/Arduino


# PROJECT
This is an adaptation of the original STM32Duino library, with some corrections and additions. The code has also been simplified to allow the use of the SI4703 chip only. There is also a powerful example included in the source. Load the ZIP in the Arduino IDE and then load the example provided and save the sketch on your STM32Duino.
Open the Serial console with 57600 baud to interact and see the current radio information.

# Wiring
<pre>
The SI4703 board has to be connected by using the following connections:

| STM32Duino pin     | Radio chip signal  |
| -------------------| -------------------|
| 3.3V               | VCC                |
| GND                | GND                | 
| PB6                | SCLK               |
| PB7                | SDIO               |
| PB4                | RST                |
</pre>



