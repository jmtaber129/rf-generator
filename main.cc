#include <msp430g2553.h>

#include "configuration.h"
#include "si570-controller.h"
#include "si570-register-calculator.h"
#include "lcd-controller.h"

const double kInitialFrequency = 50.0;

LcdController lcd_controller(kInitialFrequency);
Si570RegisterCalculator si570_reg_calculator;
Si570Controller si570_controller(&si570_reg_calculator, kInitialFrequency);

int main(void) {
  Configuration::Init();

  return 0;
}

