#include "configuration.h"

#include <msp430g2553.h>

void Configuration::Init() {
  ConfigureClocks();

  __bis_SR_register(GIE);
}

void Configuration::ConfigureClocks() {
  WDTCTL = WDTPW + WDTHOLD;  // Stop WDT.
  DCOCTL = CALDCO_16MHZ;     // Set DCO.
  BCSCTL1 = CALBC1_16MHZ;
}
