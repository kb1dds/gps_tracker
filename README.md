# A small GPS recorder unit for various purposes

I'm using mine as a payload on a radio controlled model airplane.

## Parts list:
1. Arduino nano
2. Serial GPS unit
3. Antenna for GPS unit
4. SD card module on SPI bus

## Wiring Assumptions:

9600 baud serial GPS connected as follows:
* RX   :  D3
* TX   :  D4
  
SD card attached to SPI bus as follows:
*  MOSI :  D11
*  MISO :  D12
*  CLK  :  D13
*  CS   :  D10

Arduino takes power from the aircraft's receiver, since that provides a consistent source of +5 volts.  I just used an old servo cable with a large ferrite bead on it to mitigate interference from the aircraft's electric motor.

## Operation

1. Apply power
2. Wait until the GPS unit has a location fix.  This is usually indicated by an indicator LED flashing at 1 Hz.
3. The Arduino starts recording to a CSV file on the SD card.  The filename is the UTC hour and minute that the track began.
4. That's it!