# Inline Filament Diameter Estimator, Lowcost (InFiDEL)

## Assembly and Setup
Mechanical assembly instructions are shown at https://www.youtube.com/watch?v=RYgdLPe_T0c

### Calibration
- Flash board with calibration.ino
- Connect a host via I2C (see provided host-example.ino)
- Insert each calibration drill bit shafts and replace the lookup table entries in driver.ino with your own measurements
  * Measure the actual diameter of the shafts and fill second column
  * Use the host's output *1000 to fill first column
- Finally, flash driver.ino with your values filled out
- Hub should now output exact diameter values

#### Note:
Analog output is currently not enabled in driver.ino (I2C and FAULT pin only)


## BOM

### Printed Parts
1 Block
1 Lever

#### Note:
Parts should preferably be printed in PETG, ABS or ASA as PLA may creep significantly over time.

### Electronics
There are two board variants included in this repository, either can be used to assemble the InFiDEL.

Choose one:
- SMT control board (which is populated; Gerber files and component specifications are included) 
- THT control board

- Suitable programmer for ATTiny85 (eg USBtinyISP)

- 1 SS495A linear hall effect sensor (or comparable)

### Fasteners
- 2 M3x8 screws (eg ISO 4762 M2x8)
- 1 3x24mm pin (eg DIN 7 3x24mm, for mating lever to the block)
- 1 3x16mm pin (eg DIN 7 3x16mm, for roller bearing in lever)
- 1 3x18mm pin (eg DIN 7 3x18mm, for idler bearing in block)
- 6 M3 threaded inserts

Pins may be replaced with screws or grubscrews, which may result in decreased precision.

### Other Hardware
- 1 6x2mm magnet (eg N35)
- 4 623 bearings (preferably 623-2Z / 623-ZZ)
- 1 Ballpoint pen spring (or other soft 4mm spring)
- Short length of PTFE tube

### Calibration Accessories
- 1 1.5mm drill bit
- 1 1.7mm or 1.8mm drill bit 
- 1 2mm drill bit
- Calipers for verifying drill bit shaft diameter

Drill bits may be replaced with similar diameters. 
Measured diameter is to be written to firmware lookup table.