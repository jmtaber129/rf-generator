// Register Calculator Driver used by si570-register-calculator_test.js.
// Build config should only include si570-register-calculator.cc and this file.

#include "../si570-register-calculator.h"
#include "msp430g2553.h"

double test_freq = 140.0;
double rfreq;
int n1;
int hs_div;
bool isTesting = true;

void main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;
  Si570RegisterCalculator reg_calc;
  while(isTesting) {
    reg_calc.UpdateFrequency(test_freq);

    rfreq = reg_calc.get_rfreq();
    n1 = reg_calc.get_n1();
    hs_div = reg_calc.get_hs_div();
  }
}
