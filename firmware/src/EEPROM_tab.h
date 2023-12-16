#pragma once
#include <Arduino.h>
#include "defines_globals.h"

/********************************************************************
* **** Function for EEPROM Table                              *******
*********************************************************************/

//Variables for EEPROM handling
uint16_t ee_address = 0;
uint8_t  ee_value;
uint16_t ee_chksum_adr = 8;

uint16_t DAC_min_out_voltage = 1395;  //DAC out at PWM 0 --> 1,437 V
uint16_t DAC_max_out_voltage = 2090;  //DAC out at PWM 255 --> 2,156 V


void read_eeprom_tab();
uint8_t read_eeprom_chksum();
void write_eeprom_tab();

int16_t dia_table[numdias][2] = {
  //{ADC reading in, diameter out [um]}
  //Init Values for the first start
  //After init Values are read from EEPROM
 
  { 0     , 3000 },  // safety
  { 619   , 2000 }, //2mm drill bit
  { 702   , 1800 }, //1.8mm
  { 817   , 1400 }, //1.4mm
  { 1000  , 1000 }, // 1mm --> unused!!
  { 1023  , 0000 } //safety
};


//read Values for Table from EEPROM (24 byte)
void read_eeprom_tab(){
  uint8_t *tab_ptr;
  uint8_t b1,b2,b3,b4;

  ee_address = 10;

  tab_ptr = (uint8_t*)&dia_table[0][0];
  for(uint8_t cnt_i=0;cnt_i < (numdias*4);cnt_i++){
    ee_value = EEPROM.read(ee_address);
    ee_address++;
    *tab_ptr = ee_value;
    tab_ptr++;
  }

  //Read the Calibartion Values for DAC
  b1 = EEPROM.read(ee_address++);
  b2 = EEPROM.read(ee_address++);
  b3 = EEPROM.read(ee_address++);
  b4 = EEPROM.read(ee_address++);

  DAC_min_out_voltage = (uint16_t)b1 * 256 + b2;
  DAC_max_out_voltage = (uint16_t)b3 * 256 + b4;

}

//Calculate Chksum from EEPROM
uint8_t  read_eeprom_chksum(){
  uint8_t ee_chksum = 0;
  uint8_t ee_check = 0;
  
  ee_check = EEPROM.read(ee_chksum_adr);
  ee_address = 10;

  for(uint8_t cnt_i=0;cnt_i < (numdias*4);cnt_i++){
    ee_value = EEPROM.read(ee_address);
    ee_chksum ^= ee_value;
    ee_address++;
  }

  //Read the 4 byte for DAC values
  for(uint8_t cnt_i=0;cnt_i < 4;cnt_i++){
    ee_value = EEPROM.read(ee_address);
    ee_chksum ^= ee_value;
    ee_address++;
  }

  if(ee_check == ee_chksum)
    return(true);
  else
    return(false);
}



//write Values from Table to EEPROM (24 byte)
void write_eeprom_tab(){
  byte *tab_ptr;
  uint8_t ee_chksum = 0;
  uint8_t DAC_val_ee[4];

  ee_address = 10;
  
  tab_ptr = (byte*)&dia_table[0][0];
  
  for(byte cnt_i=0;cnt_i < (numdias*4);cnt_i++){
    ee_value = (*tab_ptr++);
    ee_chksum ^= ee_value;
    EEPROM.write(ee_address, ee_value);
    ee_address++;
  }

  DAC_val_ee[0] = floor(DAC_min_out_voltage / 256);
  DAC_val_ee[1] = (DAC_min_out_voltage % 256);
  DAC_val_ee[2] = floor(DAC_max_out_voltage / 256);
  DAC_val_ee[3] = (DAC_max_out_voltage % 256);
  
  for(byte cnt_i=0;cnt_i < 4;cnt_i++){
    ee_value = DAC_val_ee[cnt_i];
    ee_chksum ^= ee_value;
    EEPROM.write(ee_address, ee_value);
    ee_address++;
  }
  
  EEPROM.write(ee_chksum_adr, ee_chksum);      //Check XOR Value
}