#include <msp430g2553.h>

#include "configuration.h"
#include "si570-controller.h"
#include "si570-register-calculator.h"
#include "si570-register-transmitter.h"
#include "lcd-controller.h"
#include "tuning-controller.h"
#include <stdlib.h>

#define TXD BIT2
#define RXD BIT1

char rx_buffer[15];
int rx_index = 0;

void delay(int delay_time);

// Initial frequency, in MHz.
const double kInitialFrequency = 15.0;
const int kInitialDigit = 3;

const unsigned char kSi570Address = 0x55;

volatile bool is_using_hardware_ui = true;

volatile double curr_freq = kInitialFrequency;

int main(void) {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;

  delay(1000);

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

  Si570RegisterTransmitter si570_transmitter(kSi570Address);
  Si570RegisterCalculator si570_calculator;
  Si570Controller si570_controller(&si570_calculator, &si570_transmitter, curr_freq);

  TuningController::Init(curr_freq, curr_digit);
  LcdController lcd_controller(curr_freq);

  while (true) {
    if (is_using_hardware_ui) {
      // Wait until either the frequency/digit is different, or if we've
      // switched to the serial UI.
      while (TuningController::CheckUpdate(curr_freq, curr_digit) || !is_using_hardware_ui) {}

      // If we got past the while loop because we switched to the serial UI,
      // send an 'S' to show that we received the command, and stop executing
      // hardware UI steps (will go to end of program loop, and stop when the
      // serial UI puts system in LPM).
      if (!is_using_hardware_ui) {
        UCA0TXBUF = 'S';
        break;
      }

      // Get the new frequencies and digits from the tuning controller, then
      // update the LCD controller and Si570 controller.
      curr_freq = TuningController::get_curr_freq();
      curr_digit = TuningController::get_curr_digit();

      si570_controller.Update(curr_freq);
      lcd_controller.Update(curr_freq, curr_digit);
    } else {
      __bis_SR_register(LPM0_bits);

      // If we woke up because we're now using the hardware UI, send an 'H' to
      // show that we received the command, and stop executing serial UI steps
      // (will go to end of program loop, and stop at tuning controller's wait
      // loop).
      if (is_using_hardware_ui) {
        UCA0TXBUF = 'H';
        break;
      }

      UCA0TXBUF = 'a';  // Send back 'a' as confirmation.
      si570_controller.Update(curr_freq);
      lcd_controller.Update(curr_freq, curr_digit);
    }

  }

}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
  if (!(IFG2 & UCA0RXIFG)) {
    return;
  }

  // If switching to hardware UI, update flag.
  if (UCA0RXBUF == 'h' || UCA0RXBUF == 'H') {
    LPM0_EXIT;
    is_using_hardware_ui = true;
    rx_index = 0;
    return;
  }

  // If switching to serial UI, update flag.
  if (UCA0RXBUF == 's' || UCA0RXBUF == 'S') {
    LPM0_EXIT;
    is_using_hardware_ui = false;
    rx_index = 0;
    return;
  }

  // If currently using hardware UI, stop processing serial command.
  if (is_using_hardware_ui == true) {
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
