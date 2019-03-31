# STM32-Radio-SI4703
An STM32duino library to control radio chips SI4703-HW322.

Tested on Linux & 'Arduino IDE' v 1.8.9 (installed for STM32duino).

Hardware: SI4703 HW-322. See here:

https://it.aliexpress.com/item/WAVGAT-Si4703-RDS-FM-Radio-Tuner-Evaluation-Breakout-Module-For-Arduino-AVR-PIC-ARM-Radio-Data/32823647114.html?spm=a2g0y.search0104.3.8.49951654tfynLG&ws_ab_test=searchweb0_0,searchweb201602_8_10065_10068_319_317_10696_10084_453_10083_454_10618_10304_10307_10820_10821_537_10302_536_10902_10843_10059_10884_10887_321_322_10103,searchweb201603_70,ppcSwitch_0&algo_expid=42397e98-eccd-4843-87c3-c8e804b10775-1&algo_pvid=42397e98-eccd-4843-87c3-c8e804b10775&transAbTest=ae803_5

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



