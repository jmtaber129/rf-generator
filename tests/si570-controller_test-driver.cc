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
  Si570Controller controller(&calculator, &transmitter, 50.0);  // Case 1.

  controller.Update(10.0);      // Case 2.
  controller.Update(15.0);      // Case 3.
  controller.Update(160.0);     // Case 4.
  controller.Update(122.35);    // Case 5.
  controller.Update(89.0);      // Case 6.
  controller.Update(142.001);   // Case 7.
  controller.Update(32.3325);   // Case 8.
  controller.Update(67.23315);  // Case 9.
  controller.Update(45.121);    // Case 10.

}
