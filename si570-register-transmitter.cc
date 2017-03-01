#include "si570-register-transmitter.h"
#include "msp430g2553.h"

Si570RegisterTransmitter::Si570RegisterTransmitter(
    unsigned char slave_address) {
  // TODO(jmtaber129): Consider moving this to a method in configuration.cc
  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0.
  P1SEL2 |= BIT6 + BIT7;                    // Assign I2C pins to USCI_B0.
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset.
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode.
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset.
  UCB0BR0 = 160;                            // fSCL = SMCLK/160 = ~100kHz.
  UCB0BR1 = 0;
  UCB0I2CSA = slave_address;                // Set slave address.
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation.
  IE2 |= UCB0TXIE;                          // Enable TX interrupt.

}

void Si570RegisterTransmitter::TransmitRegister(unsigned char register_address,
    unsigned char value) {
  // Ensure last transmission's stop condition was sent.
  while (UCB0CTL1 & UCTXSTP);

  // Send a start condition.
  UCB0CTL1 |= UCTR + UCTXSTT;

  // Wait for start condition to be sent and TX interrupt to be triggered.
  while (!(IFG2 & UCB0TXIFG));

  // Send 'register_address'.
  UCB0TXBUF = register_address;

  // Wait for 'register_address' to be sent.
  while (!(IFG2 & UCB0TXIFG));

  // Send 'value'.
  UCB0TXBUF = value;

  // Wait for 'value' to be sent.
  while (!(IFG2 & UCB0TXIFG));

  // Send stop condition.
  UCB0CTL1 |= UCTXSTP;

  // Clear the interrupt flag.
  IFG2 &= ~UCB0TXIFG;

  return;
}
