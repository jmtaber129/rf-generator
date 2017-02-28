// Register Calculator Driver used by si570-register-calculator_test.js.
// Build config should only include si570-register-calculator.cc and this file.

#include "../si570-register-transmitter.h"
#include "msp430g2553.h"

void main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;
  Si570RegisterTransmitter transmitter(0x48);
  while(true) {
    transmitter.TransmitRegister(0x04, 'a');
  }

}
