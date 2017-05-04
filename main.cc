#include <msp430g2553.h>

#include "configuration.h"
#include "si570-controller.h"
#include "si570-register-calculator.h"
#include "si570-register-transmitter.h"
#include "lcd-controller.h"
#include "tuning-controller.h"

#define TXD BIT2
#define RXD BIT1

void delay(int delay_time);

// Initial frequency, in MHz.
const double kInitialFrequency = 15.0;
const int kInitialDigit = 3;

const unsigned char kSi570Address = 0x55;

int main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;

  delay(1000);

  double curr_freq = kInitialFrequency;
  int curr_digit = kInitialDigit;

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

  Si570RegisterTransmitter transmitter(kSi570Address);
  Si570RegisterCalculator calculator;
  Si570Controller controller(&calculator, &transmitter, curr_freq);

  TuningController::Init(curr_freq, curr_digit);
  LcdController lcd_controller(curr_freq);

  while (true) {
    while (TuningController::CheckUpdate(curr_freq, curr_digit)) {}

    curr_freq = TuningController::get_curr_freq();
    curr_digit = TuningController::get_curr_digit();

    controller.Update(curr_freq);
    lcd_controller.Update(curr_freq, curr_digit);
  }

}
/*
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
*/

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR() {
  if (Si570RegisterTransmitter::TxIsr()) {
    __bic_SR_register_on_exit(CPUOFF);
  }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2(void) {
  TuningController::UpdateTuning();
}

void delay(int delay_time) {
  for (int i = 0; i <= delay_time; i++) {
    for (int j = 0; j < 100; j++) {
      __no_operation();
    }
  }
}
