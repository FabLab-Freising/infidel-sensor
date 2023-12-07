#pragma once
#include <Arduino.h>

//pins as named on PCB
#define FAULT_IO_LED 4 //on-board LED and digital output pin
#define A_IN A3 //SS495 output and push-button input (for calibration procedure)
#define A_OUT 1 //analog (filtered PWM) output, voltage proportional to filament diameter

#define numdias 6    //Tablecount 

SoftwareSerial Monitor(PB0,PB2);

//declarations
void flash_led(uint8_t count);
void wait_for_button_press();
#define LED_ON digitalWrite(FAULT_IO_LED, HIGH)
#define LED_OFF digitalWrite(FAULT_IO_LED, LOW)


//Flash the Led with the given count
void flash_led(uint8_t count){

  for(uint8_t cnt_i = 0;cnt_i < count;cnt_i++){
    digitalWrite(FAULT_IO_LED, HIGH);
    delay(120);
    digitalWrite(FAULT_IO_LED, LOW);
    delay(120);
  }
}


//Wait for press and relase the button
void wait_for_button_press(){
  int16_t in = 0;

  in = analogRead(A_IN);
  
  while(in > 20){
    in = analogRead(A_IN);
    digitalWrite(FAULT_IO_LED, HIGH);delay(80);digitalWrite(FAULT_IO_LED, LOW);delay(80);
 }
 
 while(in < 20){
   in = analogRead(A_IN);
   delay(10);
 }
}