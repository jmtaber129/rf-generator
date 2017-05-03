#include "../tuning-controller.h"

#include <msp430g2553.h>

void main(void) {
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT.

  double initial_frequency = 14.0;

  TuningController::Init(initial_frequency, 3);

  while (true) {
    __no_operation();
  }

}

#pragma vector=PORT2_VECTOR
__interrupt void Port2(void) {
  TuningController::UpdateTuning();
}
