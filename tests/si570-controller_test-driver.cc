// Driver for Si570Controller.

#include "../si570-controller.h"
#include "../si570-register-transmitter.h"
#include "../si570-register-calculator.h"
#include "msp430g2553.h"
#include <stdlib.h>

#define TXD BIT2
#define RXD BIT1

char rx_buffer[15];
int rx_index = 0;

double curr_freq = 50.0;

void main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;

  P1SEL |= RXD + TXD; // P1.1 = RXD, P1.2 = TXD.
  P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD.
  UCA0CTL1 |= UCSSEL_2; // SMCLK.
  UCA0BR0 = 138; // 16MHz 115200.
  UCA0BR1 = 0x00; // 16MHz 115200.
  UCA0MCTL = UCBRS_2 + UCBRS_1 + UCBRS_0; // Modulation UCBRSx = 5.
  UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine.
  IFG2 = 0;
  UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt.

  __bis_SR_register(GIE);

  Si570RegisterTransmitter transmitter(0x55);
  Si570RegisterCalculator calculator;
  // Initialize SI570 with 50.0MHz.
  Si570Controller controller(&calculator, &transmitter, 50.0);
__no_operation();
/*
  while (true) {
    __bis_SR_register(LPM0_bits);
    UCA0TXBUF = 'a';  // Send back 'a' as confirmation.
    controller.Update(curr_freq);
  }
  */
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
  if (!(IFG2 & UCA0RXIFG)) {
    return;
  }
  if (UCA0RXBUF != '\n' && UCA0RXBUF != '\r') {
    rx_buffer[rx_index] = UCA0RXBUF;
    ++rx_index;
    return;
  }

  rx_buffer[rx_index] = '\000';
  rx_index = 0;

  LPM0_EXIT;
  if (rx_buffer[0] != '\000') {
    curr_freq = atof(rx_buffer);
  }
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR() {
  if (Si570RegisterTransmitter::TxIsr()) {
    __bic_SR_register_on_exit(CPUOFF);
  }
}
