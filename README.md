# Inline Filament Diameter Estimator, Lowcost (InFiDEL)

<p xmlns:dct="http://purl.org/dc/terms/" xmlns:vcard="http://www.w3.org/2001/vcard-rdf/3.0#">
  <a rel="license"
     href="http://creativecommons.org/publicdomain/zero/1.0/">
    <img src="https://licensebuttons.net/p/zero/1.0/80x15.png" style="border-style: none;" alt="CC0" />
  </a>
  Originally created by Thomas Sanladerer
</p>

<b>2023-11-13, cad435 @ fablab Freising: <br>
	- Updated Readme to be up to date</b>

*A cheap, yet precise filament diameter sensor, intended to compensate for filament diameter deviations in real-time.*

The InFiDEL is a cheap (< $5) filament diameter sensor intended for use with FDM 3d printers.
The sensor can be calibrated to provide surprisingly precise filament diameter readings in real-time. 
The main idea is to use the sensor to correct for filament diameter deviations while printing.

Based on this proof-of-concept: https://www.youmagine.com/designs/filament-diameter-sensor

Detailed documention for InFiDEL: https://infidel-sensor.readthedocs.io/ <-- Outdated

## Assembly and Setup
[![Assembly and Setup Instructions on Youtube](https://img.youtube.com/vi/RYgdLPe_T0c/0.jpg)](https://www.youtube.com/watch?v=RYgdLPe_T0c)

## Wiring for Programming:
See the Pinout below on how to connect the Programmer with the sensor:<br>
Pin marked '1' is MISO, Pin marked '6' is GND 

![Pinout__](https://github.com/FabLab-Freising/infidel-sensor/assets/16453385/2d70bc79-dd25-4504-a9f8-c7134b62e4b5)



### Calibration
- Press Button and Power on Board
- LED should flash 10 times fast to indicate start of calibration --> Release button while flashing fast!
  * Led will flash once every second
  * Insert 1.4mm drillbit
  * press button
  * Led will flash once every second
  * Insert 1.7mm drillbit
  * press button
  * Led will flash once every second
  * Insert 2mm drillbit
  * press button
- Values will be stored and device will resume normal operation


## BOM

### Printed Parts
1 Block
1 Lever

#### Note:
Parts should preferably be printed in PETG, ABS or ASA as PLA may creep significantly over time.

### Electronics
There are two board variants included in this repository, either can be used to assemble the InFiDEL. 
PCBs and electronic components can be easily ordered via the [Kitspace project page](https://kitspace.org/boards/github.com/drspangle/infidel-sensor/) as it can auto-fill carts and get pricing on PCBs and electronic components from common suppliers.

Choose one:
- SMT control board (which is populated; Gerber files and component specifications are included) 
- THT control board

Also required:

- Suitable programmer for ATTiny85 (eg USBtinyISP)
- 1 SS495A linear hall effect sensor (or comparable)

### Fasteners
- 2 M3x8 screws (eg ISO 4762 M3x8)
- 1 3x24mm pin (eg DIN 7 3x24mm, for mating lever to the block)
- 1 3x16mm pin (eg DIN 7 3x16mm, for roller bearing in lever)
- 1 3x18mm pin (eg DIN 7 3x18mm, for idler bearing in block)
- 6 M3 threaded inserts
- 1 M6x5 or M6x8 grub screw (eg ISO 4026 M6x5, for pre loading spring) 

Pins may be replaced with screws or grubscrews, which may result in decreased precision.

### Other Hardware
- 1 6x2mm magnet (eg N35)
- 4 623 bearings (preferably 623-2Z / 623-ZZ)
- 1 Ballpoint pen spring (or other soft 4mm spring)
- Short length of PTFE tube

### Calibration Accessories
- 1 1.4mm drill bit
- 1 1.7mm drill bit 
- 1 2mm drill bit
- Calipers for verifying drill bit shaft diameter

Drill bits may be replaced with similar diameters. 
Measured diameter is to be written to firmware lookup table.
