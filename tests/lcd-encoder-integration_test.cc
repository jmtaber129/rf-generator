#include "../lcd-controller.h"
#include "../tuning-controller.h"

#include <msp430g2553.h>

void delay(int delay_time);

void main(void) {
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT.

  delay(1000);
  double freq = 14.234;
  TuningController::Init(freq, 3);
  LcdController lcd_controller(freq);
  __no_operation();
  while (true) {
    freq = TuningController::get_curr_freq();
    int digit = TuningController::get_curr_digit();
    lcd_controller.Update(freq, digit);
  }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2(void) {
  TuningController::UpdateTuning();
}


void delay(int delay_time) {
  for (int i = 0; i <= delay_time; i++) {
    for (int j = 0; j < 100; j++) {
      __no_operation();
    }
  }
}
