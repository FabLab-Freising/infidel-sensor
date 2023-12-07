#include <Arduino.h>

/*  Firmware for the **49* (eg SS495) hall-sensor based filament diameter sensor.
    Reads analog value from the sensor and provides a mapped and filtered diameter reading over I2C (optional analog output)

    Built for ATTiny85

    Upload firmware via programmer, using internal 16 MHz clock on ATTiny

    Compact filament sensor hardware and PCB: [URL]

    Licensed CC-0 / Public Domain by Thomas Sanladerer

    EEPROM Update and Value Set over I2C by Michael Doppler (midopple)
    Standalone Calibration from midopple
*/

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "EEPROM_tab.h"
#include "Calibrate.h"
#include "defines_globals.h"


//function declarations
int16_t convert2dia(int16_t in);
void check_for_calibrate();
void flash_led(uint8_t count);
void wait_for_button_press();
uint16_t sample_AD_cal_val(uint8_t count);
void calibrate();



float smooth = 10.0; //intensity of digital lowpass filtering
float dia = 1.7;

void setup()
{
    //setup pins
    pinMode(FAULT_IO_LED, OUTPUT);
    pinMode(A_IN, INPUT);
    pinMode(A_OUT, OUTPUT);
    Monitor.begin(57600); //not faster, the attiny does not have sufficient clock stability to generate faster timing
    delay(100);
    Monitor.println("Start!");


    //blink to indicate sensor powered and ready
  digitalWrite(FAULT_IO_LED, HIGH);
  delay(400);
  digitalWrite(FAULT_IO_LED, LOW);
  delay(400);

  //Check EEPROM Chksum and when OK read Table from EEPROM, 
  //if CHKSUM is not OK write standard Values to EEPROM
  if(read_eeprom_chksum())
  {
    read_eeprom_tab();
    Monitor.println("Loaded EEPROM Values");
  }
  else
  {
    write_eeprom_tab();
    Monitor.println("Write Blank EEPROM");
  }

  //blink to indicate sensor read EEPROM
  digitalWrite(FAULT_IO_LED, HIGH);
  delay(400);
  digitalWrite(FAULT_IO_LED, LOW);
  delay(400);

  //Check at startup if the Button is pressed to start Calibrazion
  check_for_calibrate();

}

void loop()
{

    //Varibale for Analog Out Calculation
    int16_t aout_val;

    //get fresh reading
    int16_t in = analogRead(A_IN);

    Monitor.print("R:");
    Monitor.println(in);

    //lowpass filter
    //dia += (((float)convert2dia(in)/1000.0) - dia) / smooth;
    dia = convert2dia(in)/1000.0f;

    Monitor.print("D:");
    Monitor.println(dia);

    //Calculate Voltage for Analog Out --> Volate = Diameter --> 1,73 V = 1,73 mm
    int16_t help_dia_int = (int16_t)(dia*1000);
    aout_val = map(help_dia_int, DAC_min_out_voltage, DAC_max_out_voltage, 0, 255);
    aout_val = constrain(aout_val, 0, 255);


    //write PWM for analog value generation
    analogWrite(A_OUT, (uint8_t)(aout_val));

      //light LED and pull up FAULT_IO_LED if sensor saturated, button pressed or diameter low
    if (in < 3 or dia < 1.5) 
    {
        digitalWrite(FAULT_IO_LED, HIGH);
    }
    else 
    {
        digitalWrite(FAULT_IO_LED, LOW);
    }

    delay(500);

}


//Convert AD Value to Diameter
int16_t convert2dia(int16_t in) {
  
  //converts an ADC reading to diameter
  //Inspired by Sprinter / Marlin thermistor reading
 
  
  uint8_t i;
  
  for (i = 1; i < numdias; i++)
  {
    //check if we've found the appropriate row
    if (dia_table[i][0] > in)
    {
      float slope = ((float)dia_table[i][1] - dia_table[i - 1][1]) / ((float)dia_table[i][0] - dia_table[i - 1][0]);
      float indiff = ((float)in - dia_table[i - 1][0]);
      float outdiff = slope * indiff;
      int16_t out = (int16_t)outdiff + dia_table[i - 1][1];
      return (out);
      break;
    }
  }
}

