#pragma once
#include <Arduino.h>
#include "defines_globals.h"

//function declarations
void check_for_calibrate();
void calibrate();
uint16_t sample_AD_cal_val(uint8_t count);


//Values for ADC Mean sampling 
uint16_t ADC_mean_min = 0;
uint16_t ADC_mean_max = 0;
uint8_t ADC_mean_cnt = 0;


/********************************************************************
* **** Function for Standalone Calibrating                    *******
*********************************************************************/

//Check at Startup for Calibrate-mode
void check_for_calibrate(){
  
  //get fresh reading
  int16_t in = 0;
  in = analogRead(A_IN);

  if(in < 5){   //Button pressed
    //read 5 times more to filter
    for(uint8_t cnt_i = 0;cnt_i < 5;cnt_i++){
      in += analogRead(A_IN);
      delay(50);
    }

    if(in < 10)
      calibrate();
  }
}


/********************************************************************
* **** Calibrate Sensor with Button / Standalone Calibration *******
*********************************************************************/



#define CAL_STATE_START         0
#define CAL_STATE_DIA_1         1
#define CAL_STATE_WAIT_DIA_1    2
#define CAL_STATE_DIA_2         3
#define CAL_STATE_WAIT_DIA_2    4
#define CAL_STATE_DIA_3         5
#define CAL_STATE_WAIT_DIA_3    6
#define CAL_STATE_END           10

void calibrate(){
  
    /*  Self-calibration
    *  
    *  Press button, insert 1.4mm brill bit shaft, press to confirm reading
    *  Repeat for 1.7mm, 2mm
    *  (optional: use interpolated value based on point between 1.5 and 2mm if 1.7mm if read with implausible value
    */

    uint16_t cal_sample_AD_val = 0;
    int16_t in = 0;

    Monitor.println("Start Cal");

    //LED flash 10 times to show that calibrate is start
    flash_led(10);


    in = analogRead(A_IN);
    Monitor.println("Release btn");
    while (in < 20)
    {
        in = analogRead(A_IN);
    }

    LED_ON;
    Monitor.println("Insert 1.4mm Bit & Press Button");
    wait_for_button_press();
    LED_OFF;
    //Sample Valuse für AD to DIA Table --> 100 Samples
    cal_sample_AD_val = sample_AD_cal_val(100);   // get ADC Value     
    // write ADC Value for 1.4mm into Table
    if(cal_sample_AD_val > 0) dia_table[3][0] = cal_sample_AD_val;

    LED_ON;
    Monitor.println("Insert 1.7mm Bit & Press Button");
    wait_for_button_press();
    LED_OFF;
    //Sample Value für AD to DIA Table --> 100 Samples
    cal_sample_AD_val = sample_AD_cal_val(100);
    //write ADC Value for 1.7mm into Table
    if(cal_sample_AD_val > 0) dia_table[2][0] = cal_sample_AD_val;



    LED_ON;
    Monitor.println("Insert 2mm Bit & Press Button");
    wait_for_button_press();
    LED_OFF;
    //Sample Value für AD to DIA Table --> 100 Samples
    cal_sample_AD_val = sample_AD_cal_val(100);
    //write ADC Value for 2.0mm into Table
    if(cal_sample_AD_val > 0) dia_table[1][0] = cal_sample_AD_val;
    write_eeprom_tab();
    delay(40);
}


//Sample AD Values with ppeak rejection for calibration
// When OK return the Meanvalue / when not OK Return 0
uint16_t sample_AD_cal_val(uint8_t count){
  int16_t  in = 0;
  uint32_t ADC_sum_samples = 0;
  int16_t  return_ADC_val = 0;
  int16_t  ADC_range_val_high = 0;
  int16_t  ADC_range_val_low = 0;
  uint8_t  sample_count = 0;

  ADC_mean_min = 1024;
  ADC_mean_max = 0;

  //First 10 Sample to get Mean Value
  for(uint8_t cnt_i = 0;cnt_i < 10;cnt_i++){
    in += analogRead(A_IN);
    delay(50);
  }

  //ADC Values should in the Range from +/- 10%, --> Check for Peaks
  ADC_range_val_high = (in / 9);
  ADC_range_val_low = (in / 11);

  digitalWrite(FAULT_IO_LED, HIGH);

  for(uint8_t cnt_i = 0;cnt_i < count;cnt_i++){
    in = analogRead(A_IN);
    //Filtert out Peak Values 
    if(in < ADC_range_val_high && in > ADC_range_val_low){
      sample_count++;
      ADC_sum_samples += in;

      if(in < ADC_mean_min) ADC_mean_min = in;
      if(in > ADC_mean_max) ADC_mean_max = in;
      
      delay(50);
    }
  }

  digitalWrite(FAULT_IO_LED, LOW);

  if(sample_count > (count / 2)){
    return_ADC_val = (int16_t)(ADC_sum_samples / sample_count);

    ADC_mean_cnt = sample_count;
    
    return(return_ADC_val);
  }
  else{
    return(0);
  }
  
}