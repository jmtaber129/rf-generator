#include <msp430g2553.h>

#include "configuration.h"
#include "si570-controller.h"
#include "si570-register-calculator.h"
#include "si570-register-transmitter.h"
#include "lcd-controller.h"

// Initial frequency, in MHz.
const double kInitialFrequency = 50.0;

int main(void) {
  Configuration::Init();
  LcdController lcd_controller(kInitialFrequency);
  Si570RegisterCalculator si570_reg_calculator;
  Si570RegisterTransmitter si570_reg_transmitter(0x48);
  Si570Controller si570_controller(&si570_reg_calculator, &si570_reg_transmitter,
      kInitialFrequency);

  // TODO(all): Update loop.

  return 0;
}

// TODO(all): Port interrupt for encoder state changes.
