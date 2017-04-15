// Looks for the first I2C slave address that returns an ACK.

#include "msp430g2553.h"

int main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0.
  P1SEL2 |= BIT6 + BIT7;                    // Assign I2C pins to USCI_B0.
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset.
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode.
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset.
  UCB0BR0 = 160;                            // fSCL = SMCLK/160 = ~100kHz.
  UCB0BR1 = 0;
  UCB0I2CIE &= ~UCNACKIE;
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation.
  IE2 &= ~(UCB0TXIE + UCB0RXIE);            // No RX or TX interrupts.

  for (int slave_address = 0; slave_address <= 255; ++slave_address) {
    UCB0I2CSA = slave_address;
    UCB0CTL1 |= UCTR + UCTXSTT + UCTXSTP;
    while(UCB0CTL1 & UCTXSTP);
    if (!(UCB0STAT & UCNACKIFG)) {
      // Set breakpoint here.  'slave_address' will be the address of the slave.
      __no_operation();
    }
  }
}




