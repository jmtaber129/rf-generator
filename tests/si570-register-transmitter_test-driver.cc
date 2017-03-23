// Driver for Si570RegisterTransmitter.
// Build config should only include si570-register-transmitter.cc and this file.

#include "../si570-register-transmitter.h"
#include "msp430g2553.h"

void main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;
  Si570RegisterTransmitter transmitter(0x48);

  __bis_SR_register(GIE);
  transmitter.TransmitRegister(0x07, 'a');
  transmitter.TransmitRegister(0x08, 'b');
  transmitter.TransmitRegister(0x09, 'c');
  transmitter.TransmitRegister(0x0a, 'd');
  transmitter.TransmitRegister(0x0b, 'e');
  transmitter.TransmitRegister(0x0c, 'f');

}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR() {
  if (Si570RegisterTransmitter::TxIsr()) {
    __bic_SR_register_on_exit(CPUOFF);
  }
}
