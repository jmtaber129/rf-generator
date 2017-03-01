// Driver for Si570RegisterTransmitter.
// Build config should only include si570-register-transmitter.cc and this file.

#include "../si570-register-transmitter.h"
#include "msp430g2553.h"

void main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;
  Si570RegisterTransmitter transmitter(0x48);
  transmitter.TransmitRegister(0x07, 'a');
  transmitter.TransmitRegister(0x08, 'b');
  transmitter.TransmitRegister(0x09, 'c');
  transmitter.TransmitRegister(0x0a, 'd');
  transmitter.TransmitRegister(0x0b, 'e');
  transmitter.TransmitRegister(0x0c, 'f');

}
