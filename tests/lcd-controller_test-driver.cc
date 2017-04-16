#include "../lcd-controller.h"

#include <msp430g2553.h>

void delay(int delay_time);

void main(void) {
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT.

  delay(1000);
  double freq = 14.234;
  LcdController lcd_controller(freq);
  __no_operation();
  while (true) {
    lcd_controller.Update(freq, 3);
    freq += .001;
  }
}

void delay(int delay_time) {
  for (int i = 0; i <= delay_time; i++) {
    for (int j = 0; j < 100; j++) {
      __no_operation();
    }
  }
}
