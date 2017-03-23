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

TransmitState Si570RegisterTransmitter::transmit_state_ = kTransmitRegister;

bool Si570RegisterTransmitter::busy_ = false;

unsigned char Si570RegisterTransmitter::transmit_register_ = 0x00;
unsigned char Si570RegisterTransmitter::transmit_value_ = 0x00;

void Si570RegisterTransmitter::TransmitRegister(unsigned char register_address,
    unsigned char value) {
  // Ensure last transmission's stop condition was sent.
  while (UCB0CTL1 & UCTXSTP);
  Si570RegisterTransmitter::busy_ = true;

  transmit_register_ = register_address;
  transmit_value_ = value;

  transmit_state_ = kTransmitRegister;

  // Send a start condition.
  UCB0CTL1 |= UCTR + UCTXSTT;
  __bis_SR_register(CPUOFF + GIE);
  // Wait for start condition to be sent and TX interrupt to be triggered.
  //while (!(this->tx_ie_flag_));
  //this->tx_ie_flag_ = false;

  // Send 'register_address'.
  //UCB0TXBUF = register_address;

  // Wait for 'register_address' to be sent.
  //while (!(this->tx_ie_flag_));
  //this->tx_ie_flag_ = false;

  // Send 'value'.
  //UCB0TXBUF = value;

  // Wait for 'value' to be sent.
  //while (!(this->tx_ie_flag_));
  //this->tx_ie_flag_ = false;

  // Send stop condition.
  //UCB0CTL1 |= UCTXSTP;

  // Clear the interrupt flag.
  //IFG2 &= ~UCB0TXIFG;

  //while(Si570RegisterTransmitter::busy_);

  return;
}

TransmitState buffer[5];
int buffer_index = 0;

bool Si570RegisterTransmitter::TxIsr() {
  buffer[buffer_index] = transmit_state_;
  buffer_index++;
  switch(Si570RegisterTransmitter::transmit_state_) {
    case kTransmitRegister: {
      // Send 'register_address'.
      UCB0TXBUF = transmit_register_;
      transmit_state_ = kTransmitValue;
      return false;
    }
    case kTransmitValue: {
      // Send 'value'.
      UCB0TXBUF = transmit_value_;
      transmit_state_ = kTransmitStop;
      return false;
    }
    case kTransmitStop: {
      UCB0CTL1 |= UCTXSTP;
      IFG2 &= ~UCB0TXIFG;
      return true;
    }
    default: {
      return false;
    }
  }
}
