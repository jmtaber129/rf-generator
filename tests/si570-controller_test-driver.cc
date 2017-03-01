// Driver for Si570Controller.

#include "../si570-controller.h"
#include "../si570-register-transmitter.h"
#include "../si570-register-calculator.h"
#include "msp430g2553.h"

void main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;
  Si570RegisterTransmitter transmitter(0x48);
  Si570RegisterCalculator calculator;
  Si570Controller controller(&calculator, &transmitter, 50.0);

  // TODO(jmtaber129): Add more test cases beyond just updating through the
  // constructor.
}
